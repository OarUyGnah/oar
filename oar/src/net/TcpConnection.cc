#include "net/TcpConnection.h"
#include "StringBuffer.h"
#include "net/Channel.h"
#include "net/InetAddress.h"
#include "net/Socket.h"
#include "net/socketapi.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <functional>
#include <oar/net/socketapi.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
namespace oar {
TcpConnection::TcpConnection(EventLoop* loop, Socket* sock)
    : _loop(loop)
    , _ch(nullptr)
    , _sock(sock)
    , _addr(new InetAddress())
    , _state(Connected)
    , _rd_buf(new StringBuffer)
    , _wr_buf(new StringBuffer)
{
    // _sock = new Socket(sock->accept(*_addr));
    _sock->set_nonblocking();
    _ch = new Channel(_loop, _sock->fd());
    // _ch->setCallback(std::bind(&TcpConnection::echo, this, _sock->fd()));
    // printf("TcpConnection::TcpConnection client_addr : %s:%d\n", _addr->ip().c_str(), _addr->port());
    _ch->enableReading();
}

TcpConnection::~TcpConnection()
{
    delete _ch;
    delete _sock;
    delete _wr_buf;
    delete _addr;
}

void TcpConnection::setDeleteConnectionCallback(DeleteConnectionCallback cb)
{
    _delete_cb = cb;
}
void TcpConnection::setOnConnectCallback(OnConnectionCallback cb)
{
    _on_connect_cb = cb;
    _ch->setReadCallback([this]() { _on_connect_cb(this); });
}

void TcpConnection::close()
{
    _delete_cb(_sock);
}

void TcpConnection::read()
{
    assert(_state == State::Connected);
    _rd_buf->clear();
    if (_sock->isNonBlocking())
        nonblockingRead();
    else
        blockingRead();
}

void TcpConnection::write()
{
    assert(_state == State::Connected);
    if (_sock->isNonBlocking())
        nonblockingWrite();
    else
        blockingWrite();
    _wr_buf->clear();
}

void TcpConnection::blockingRead()
{
    int fd = _sock->fd();
    uint rcvSize { 0 };
    socklen_t len = sizeof(rcvSize);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvSize, &len);
    char buf[rcvSize];
    ssize_t rd_bytes = oar::read(fd, buf, sizeof(buf));
    if (rd_bytes > 0) {
        _rd_buf->append(buf, rd_bytes);
    } else if (rd_bytes == 0) {
        printf("read EOF, blocking client fd %d disconnected\n", fd);
        _state = State::Closed;
    } else if (rd_bytes == -1) {
        printf("Other error on blocking client fd %d\n", fd);
        _state = State::Closed;
    }
}

void TcpConnection::nonblockingRead()
{
    int fd = _sock->fd();
    ssize_t rd_bytes = 0;
    char buf[1024];
    // printf("TcpConnection::nonblockingRead\n");
    while (true) {
        bzero(buf, sizeof(buf));
        rd_bytes = oar::read(fd, buf, sizeof(buf));
        if (rd_bytes > 0) {
            _rd_buf->append(buf, sizeof buf);
        } else if (rd_bytes == -1 && errno == EINTR) {
            printf("continue reading\n");
            continue;
        } else if (rd_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // 数据全部读取完毕
            // printf("message from -1 %s %d : %s", _addr->ip().c_str(), _addr->port(), _wr_buf->begin());
            // oar::write(fd, _wr_buf->begin(), _wr_buf->readableBytes());
            // _wr_buf->retrieveAll();
            // _wr_buf->clear();
            break;
        } else if (rd_bytes == 0) {
            printf("EOF, client fd %d disconnected\n", fd);
            _state = State::Closed;
            break;
        } else {
            printf("Connection reset by peer\n");
            _state = State::Closed;
            break;
        }
    }
}

void TcpConnection::blockingWrite()
{
    if (oar::write(_sock->fd(), _wr_buf->begin(), _wr_buf->size()) == -1) {
        unix_error("Other error on blocking client fd %d\n", _sock->fd());
        _state = State::Closed;
    }
}

void TcpConnection::nonblockingWrite()
{
    int fd = _sock->fd();
    char buf[_wr_buf->size()];
    memcpy(buf, _wr_buf->begin(), _wr_buf->size());
    int totalSize = _wr_buf->size();
    int leftSize = totalSize;
    ssize_t write_bytes = 0;
    while (leftSize > 0) {
        write_bytes = oar::write(fd, buf + totalSize - leftSize, leftSize);
        if (write_bytes == -1 && errno == EINTR) {
            printf("continue writing\n");
            continue;
        } else if (write_bytes == -1 && errno == EAGAIN) {
            break;
        } else if (write_bytes == -1) {
            printf("Other error on client fd %d\n", fd);
            _state = State::Closed;
            break;
        }
        leftSize -= write_bytes;
    }
}
}