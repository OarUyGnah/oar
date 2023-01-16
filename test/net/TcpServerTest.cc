#include "oar/net/TcpServer.h"
#include "oar/net/EventLoop.h"
#include <iostream>
#include <oar/StringBuffer.h>
#include <oar/net/InetAddress.h>
#include <oar/net/Socket.h>
#include <oar/net/TcpConnection.h>
using namespace oar;
// clang++ TcpServerTest.cc -loar_net -loar_utils -Wall -g3 -fsanitize=address
int main()
{
    EventLoop* loop = new EventLoop();
    TcpServer* server = new TcpServer(loop);
    server->setOnConnectCallback([](TcpConnection* conn) {
        std::cout << "new connection from fd " << conn->socket()->fd() << std::endl;
    });
    // channel 每次processEvent默认调用onRecv设置的ReadCB
    server->onRecv([](TcpConnection* conn) {
        // std::cout << "on recv" << std::endl;
        conn->read();
        if (conn->state() == TcpConnection::State::Closed) {
            conn->close();
            return;
        }
        // conn->read(); // 不用，自动read
        // std::cout << &conn->addr() << std::endl;
        std::cout << "Message from client " << conn->socket()->fd() << " ["
                  << conn->addr().ip().c_str() << ":" << conn->addr().port() << "] : "
                  << conn->readBuffer()->rBegin();
        // std::cout << "sock " << conn->socket()->sockaddr() << std::endl;
        // std::cout << "Message from client : "
        //           //   << conn->socket()->sockaddr()->ip() << ":" << conn->socket()->sockaddr()->port() << "] : "
        //           << conn->readBuffer()->rBegin();
        // conn->readBuffer()->clear();
    });
    loop->loop();
    return 0;
}