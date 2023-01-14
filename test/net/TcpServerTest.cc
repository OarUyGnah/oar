#include "oar/net/TcpServer.h"
#include "oar/net/EventLoop.h"
#include <iostream>
#include <oar/StringBuffer.h>
#include <oar/net/Socket.h>
#include <oar/net/TcpConnection.h>
using namespace oar;

// g++ TcpServerTest.cc ../../oar/net/EventLoop.cc ../../oar/net/socketapi.cc ../../oar/net/Epoll.cc ../../oar/net/Channel.cc ../../oar/net/Socket.cc ../../oar/net/InetAddress.cc ../../oar/net/TcpServer.cc ../../oar/net/TcpConnection.cc ../../oar/net/Acceptor.cc -o server

int main()
{
    EventLoop* loop = new EventLoop();
    TcpServer* server = new TcpServer(loop);
    server->setOnConnectCallback([](TcpConnection* conn) {
        conn->read();
        if (conn->state() == TcpConnection::State::Closed) {
            conn->close();
            return;
        }
        std::cout << "Message from client " << conn->socket()->fd() << " : "
                  << conn->readBuffer()->rBegin();
        conn->readBuffer()->clear();
    });
    loop->loop();
    return 0;
}