#include "net/Acceptor.h"
#include "oar/net/Channel.h"
#include "oar/net/InetAddress.h"
#include "oar/net/Socket.h"
#include <algorithm>
#include <cstdio>
#include <functional>

namespace oar {
Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenaddr)
    : _loop(loop)
    , _sock(new Socket)
    , _addr(new InetAddress(listenaddr))
{
    _sock->bind(*_addr);
    _sock->listen();
    _sock->set_nonblocking();
    _accept_channel = new Channel(_loop, _sock->fd());
    // _accept_channel->setCallback(std::bind(&Acceptor::acceptConnection, this));
    _accept_channel->setReadCallback(std::bind(&Acceptor::acceptConnection, this));
    _accept_channel->enableReading();
}

Acceptor::~Acceptor()
{
    delete _sock;
    delete _addr;
    delete _accept_channel;
    // for (auto* sock : _accepted_socks) {
    //     delete sock;
    // }
}

void Acceptor::acceptConnection()
{
    InetAddress* client_addr = new InetAddress;
    Socket* client_sock = new Socket(_sock->accept(*client_addr));
    // Socket client_sock(_sock->accept(*client_addr));
    printf("======= new client from fd %d %s:%d =======\n", client_sock->fd(), client_addr->ip().c_str(), client_addr->port());
    client_sock->set_nonblocking();
    // client_sock->setInetAddress(*client_addr);
    // printf("======= new client from fd %d %s:%d =======\n", client_sock->fd(), client_sock->sockaddr()->ip().c_str(), client_sock->sockaddr()->port());
    _new_connection_cb(client_sock, client_addr);
    _accepted_socks.emplace_back(client_sock, client_addr);
    // delete client_addr;
}

void Acceptor::setNewConnectionCallback(NewConnectionCallback cb)
{
    _new_connection_cb = cb;
}

void Acceptor::deleteSock(int fd)
{
    auto it = std::find_if(_accepted_socks.begin(), _accepted_socks.end(), [&](std::pair<Socket*, InetAddress*> pair) {
        return pair.first->fd() == fd;
    });
    if (it != _accepted_socks.end()) {
        // Socket* del = *it;
        delete it->second;
        _accepted_socks.erase(it);

        // delete del;
    }
}
}