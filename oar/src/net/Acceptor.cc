#include "oar/net/Acceptor.h"
#include "oar/net/Channel.h"
#include "oar/net/InetAddress.h"
#include "oar/net/Socket.h"
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
}

void Acceptor::acceptConnection()
{
    InetAddress* client_addr = new InetAddress;
    Socket* client_sock = new Socket(_sock->accept(*client_addr));
    printf("new client from fd %d %s:%d\n", client_sock->fd(), client_addr->ip().c_str(), client_addr->port());
    client_sock->set_nonblocking();
    _new_connection_cb(client_sock);
    delete client_addr;
}

void Acceptor::setNewConnectionCallback(NewConnectionCallback cb)
{
    _new_connection_cb = cb;
}

}