#include "TcpConnection.h"
#include "../StringBuffer.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
#include "socketapi.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cstdio>
#include <cstring>
#include <functional>
#include <string>
#include <strings.h>

namespace oar {
TcpConnection::TcpConnection(EventLoop* loop, Socket* sock)
    : _loop(loop)
    , _addr(new InetAddress())
    , _sock(nullptr)
    , _ch(nullptr)
    , _buf(new StringBuffer)
{
    _sock = new Socket(sock->accept(*_addr));
    _sock->set_nonblocking();
    _ch = new Channel(_loop, _sock->fd());
    _ch->setCallback(std::bind(&TcpConnection::echo, this, _sock->fd()));
    printf("TcpConnection::TcpConnection client_addr : %s:%d\n", _addr->ip().c_str(), _addr->port());
    _ch->enableReading();
}

TcpConnection::~TcpConnection()
{
    delete _ch;
    delete _sock;
    delete _buf;
}

void TcpConnection::echo(int fd)
{
    // int times = 0;
    char buf[1024];
    while (true) {
        bzero(buf, sizeof(buf));
        auto read_bytes = oar::read(fd, buf, sizeof(buf) + 1);
        // printf("times = %d\n", ++times);
        if (read_bytes > 0) {
            // printf("message from %s:%d : %s", _addr->ip().c_str(), _addr->port(), buf);
            // printf("read bytes is %d\n", read_bytes);
            // printf("message from %s %d : %s", _addr->ip().c_str(), _addr->port(), buf);

            _buf->append(buf, sizeof buf);
            // printf("_buf.size() = %d\n", _buf->readableBytes());
        } else if (read_bytes == -1 && errno == EINTR) {
            printf("continue reading");
            continue;
        } else if (read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            printf("message from -1 %s %d : %s", _addr->ip().c_str(), _addr->port(), _buf->begin());

            // oar::write(fd, _buf->begin(), _buf->size());
            oar::write(fd, _buf->begin(), _buf->readableBytes());
            _buf->retrieveAll();
            _buf->clear();
            break;
        } else if (read_bytes == 0) {
            printf("EOF, client fd %d disconnected\n", fd);
            _delete_cb(_sock);
            break;
        } else {
            printf("Connection reset by peer\n");
            _delete_cb(_sock);
            break;
        }
    }
}
}