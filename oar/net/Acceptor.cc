#include "Acceptor.h"
#include "Channel.h"
#include "InetAddress.h"
#include "Socket.h"
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
    _accept_channel->setCallback(std::bind(&Acceptor::acceptConnection, this));
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
    _new_connection_cb(_sock);
}

void Acceptor::setNewConnectionCallback(NewConnectionCallback cb) {
    _new_connection_cb = cb;
}

}