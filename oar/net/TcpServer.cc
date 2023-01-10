#include "TcpServer.h"
#include "Acceptor.h"
#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "Socket.h"
#include "TcpConnection.h"
#include <algorithm>
#include <array>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <string>
#include <strings.h>
#include <thread>
#include <unistd.h>
#include <utility>
namespace oar {

#define READ_BUFFER_SIZE 2048

TcpServer::TcpServer(EventLoop* loop, const InetAddress& addr, const std::string& name)
    : _mainloop(loop)
    , _accpetor(new Acceptor(_mainloop, addr))
    , _threadpool(new Threadpool(std::thread::hardware_concurrency() / 2))
{
    _accpetor->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
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
        _subloops.push_back(new EventLoop);
    }
    _threadpool->start();
    for (int i = 0; i < std::thread::hardware_concurrency() / 2; ++i) {
        printf("??\n");
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, _subloops[i]);
        _threadpool->runTask(sub_loop);
    }
}

TcpServer::~TcpServer()
{
    delete _accpetor;
    delete _threadpool;
}

void TcpServer::processReadEvent(int sock)
{
    char buf[128];
    while (true) {
        // std::for_each(buf.begin(), buf.end(), [&](char& c) { c = 0; });
        bzero(buf, sizeof(buf));
        auto n = oar::read(sock, buf, sizeof(buf));
        printf("read bytes is %d\n", n);
        if (n > 0) {
            printf("recv msg from client fd = %d, msg : %s\n", sock, &buf[0]);
            write(sock, &buf[0], READ_BUFFER_SIZE);
        } else if (n == -1) {
            if (errno == EINTR) {
                printf("continue reading\n");
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("finish reading once, errno: %d\n", errno);
                break;
            }
        } else if (n == 0) { // EOF
            printf("EOF, client fd %d disconnected\n", sock);
            oar::close(sock);
            break;
        }
    }
}

void TcpServer::newConnection(Socket* sock)
{
    // InetAddress newaddr {};  // error!
    // InetAddress *newaddr = new InetAddress();
    // Socket *newsock = new Socket(sock->accept(*newaddr));
    // newsock->set_nonblocking();
    // printf("newsock %d\n",newsock->fd());
    // Channel *client_channel =  new Channel(_loop,newsock->fd());
    // std::function<void()> cb = std::bind(&TcpServer::processReadEvent,this, newsock->fd());
    // client_channel->setCallback(cb);
    // client_channel->enableReading();
    // printf("new connection! client channel : %d\n",client_channel->fd());
    TcpConnection* conn = new TcpConnection(_subloops[sock->fd() % _subloops.size()], sock);
    // TcpConnection* conn = new TcpConnection(_mainloop, sock);
    conn->setDeleteConnectionCallback(std::bind(&TcpServer::deleteConncetion, this, std::placeholders::_1));
    // _conns.insert(std::make_pair(sock->fd(), conn));
    _conns[sock->fd()] = conn;
}

void TcpServer::deleteConncetion(Socket* sock)
{
    if (sock->fd() == -1)
        return;
    auto it = _conns.find(sock->fd());
    if (it != _conns.end()) {
        TcpConnection* del = _conns[sock->fd()];
        _conns.erase(sock->fd());
        delete del;
        oar::close(sock->fd()); // 没有则段错误
    }
}
}