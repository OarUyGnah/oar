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
#include <memory>
#include <oar/net/socketapi.h>
#include <string>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>
namespace oar {

TcpConnection::TcpConnection(EventLoop* loop, Socket* sock)
    : _ch()
    , _sock(sock)
    , _state(Invalid)
    , _rd_buf(new StringBuffer)
    , _wr_buf(new StringBuffer)
{
    _sock->set_nonblocking();
    _ch = std::make_unique<Channel>(loop, sock->fd());
    _ch->enableRd();
    // 没有则一直接受TcpServer::onRecv，持续TcpConnection::read
    _ch->etMode(true);
    _state = State::Connected;
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::setDeleteConnectionCallback(DeleteConnectionCallback cb)
{
    _delete_cb = cb;
}

void TcpConnection::setOnRecvCallback(OnRecvCallback cb)
{
    _on_recv_cb = std::move(cb);
    _ch->setReadCallback(std::bind(&TcpConnection::defaultRecvCallback, this));
}

// void TcpConnection::onRecv(OnRecvCallback cb)
// {
//     setOnRecvCallback(cb);
// }
// void TcpConnection::onConnect(OnConnectionCallback cb)
// {
//     setOnConnectCallback(cb);
// }

void TcpConnection::close()
{
    _delete_cb(_sock.get());
}

void TcpConnection::read()
{
    if (_state != State::Connected) {
        printf("curr state is %d Connection is not connected, can not read\n", _state);
    }
    assert(_state == State::Connected);
    _rd_buf->clear();
    if (_sock->isNonBlocking())
        nonblockingRead();
    else
        blockingRead();
}

void TcpConnection::write()
{
    if (_state != State::Connected) {
        printf("curr state is %d Connection is not connected, can not write\n", _state);
    }
    assert(_state == State::Connected);
    if (_sock->isNonBlocking())
        nonblockingWrite();
    else
        blockingWrite();
    _wr_buf->clear();
}

void TcpConnection::send(std::string s)
{
    _wr_buf->append(s.c_str(), s.size());
    write();
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
    _rd_buf->clear();
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
            break;
        } else if (rd_bytes == 0) {
            printf("nonblocking EOF, client fd %d disconnected\n", fd);
            _state = State::Closed;
            // close();
            break;
        } else {
            printf("Connection reset by peer\n");
            _state = State::Closed;
            close();
            break;
        }
    }
}

void TcpConnection::blockingWrite()
{
    if (oar::write(_sock->fd(), _wr_buf->begin(), _wr_buf->size()) == -1) {
        _state = State::Closed;
        unix_error("Other error on blocking client fd %d\n", _sock->fd());
    }
}

void TcpConnection::nonblockingWrite()
{
    int fd = _sock->fd();
    char buf[_wr_buf->size()];
    memcpy(buf, _wr_buf->rBegin(), _wr_buf->readableBytes());
    printf("nonblockingWrite : %s totalsize = %d\n", buf, _wr_buf->readableBytes());
    int totalSize = _wr_buf->readableBytes();
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

void TcpConnection::defaultRecvCallback()
{
    if (_state == State::Connected) {
        // read();
        _on_recv_cb(this);
        // } else if (_state == State::Closed) {
        // close();
        // printf("defaultRecvCallback connect closed\n");
    }
}
}