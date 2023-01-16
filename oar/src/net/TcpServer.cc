#include "net/TcpServer.h"
#include "net/Acceptor.h"
#include "oar/net/Channel.h"
#include "oar/net/Epoll.h"
#include "oar/net/EventLoop.h"
#include "oar/net/InetAddress.h"
#include "oar/net/Socket.h"
#include "oar/net/TcpConnection.h"
#include <algorithm>
#include <array>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <strings.h>
#include <thread>
#include <unistd.h>
#include <utility>

namespace oar {

#define READ_BUFFER_SIZE 2048

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr, const std::string& name)
    : _mainloop(loop)
    , _accpetor(new Acceptor(loop, addr))
    , _threadpool(new Threadpool(std::thread::hardware_concurrency() / 2))
{
    _accpetor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
    // Socket* server_sock = new Socket();
    // InetAddress server_addr = addr;
    // printf("server_addr ip:%s port:%d\n",server_addr.ip().c_str(),server_addr.port());
    // server_sock->bind(server_addr);
    // server_sock->listen();
    // server_sock->set_nonblocking();
    // Channel* server_channel = new Channel(_loop, server_sock->fd());
    // printf("server channel ===== %d",server_channel->fd());
    // server_channel->setCallback(std::bind(&TcpServer::newConnection, this, server_sock));
    // server_channel->enableReading();

    for (int i = 0; i < std::thread::hardware_concurrency() / 2; ++i) {
        _subloops.emplace_back(new EventLoop);
    }
    _threadpool->start();
    for (int i = 0; i < std::thread::hardware_concurrency() / 2; ++i) {
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, _subloops[i]);
        _threadpool->runTask(sub_loop);
    }
}

TcpServer::~TcpServer()
{
    // delete _accpetor;
    // delete _threadpool;
}

void TcpServer::newConnection(Socket* sock, InetAddress* addr)
{
    std::unique_ptr<TcpConnection> conn = std::make_unique<TcpConnection>(_subloops[sock->fd() % _subloops.size()], sock);
    // TcpConnection* conn = new TcpConnection(_mainloop.get(), sock);
    conn->setDeleteConnectionCallback(std::bind(&TcpServer::deleteConncetion, this, std::placeholders::_1));
    conn->setPeerAddr(*addr);
    conn->setOnRecvCallback(_on_recv_cb);
    _conns[sock->fd()] = std::move(conn);
    if (_on_connect_cb) {
        _on_connect_cb(_conns[sock->fd()].get());
    }
}

void TcpServer::deleteConncetion(Socket* sock)
{
    if (sock->fd() == -1)
        return;
    auto it = _conns.find(sock->fd());
    if (it != _conns.end()) {
        _conns.erase(sock->fd());
        // oar::close(sock->fd()); // 有则error
    }
}

void TcpServer::setOnConnectCallback(ConnectCallBack cb)
{
    _on_connect_cb = std::move(cb);
}

void TcpServer::onRecv(RecvCallBack cb)
{
    _on_recv_cb = std::move(cb);
}

void TcpServer::onConnect(ConnectCallBack cb)
{
    _on_connect_cb = std::move(cb);
}

}
