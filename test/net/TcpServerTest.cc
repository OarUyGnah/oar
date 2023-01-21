#include "oar/net/TcpServer.h"
#include "oar/net/EventLoop.h"
#include <cstdio>
#include <iostream>
#include <oar/StringBuffer.h>
#include <oar/net/InetAddress.h>
#include <oar/net/Socket.h>
#include <oar/net/TcpConnection.h>
#include <sstream>
#include <string>
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
    std::string s;
    server->onRecv([&s](TcpConnection* conn) {
        // std::cout << "on recv" << std::endl;
        conn->read();
        if (conn->state() == TcpConnection::State::Closed) {
            conn->close();
            return;
        }
        // conn->read(); // 不用，自动read
        std::cout << "Message from client " << conn->socket()->fd() << " ["
                  << conn->addr().ip().c_str() << ":" << conn->addr().port() << "] : "
                  << conn->readBuffer()->rBegin() << std::endl;

        std::getline(std::cin, s);
        if (s != "") {
            conn->send(s);
        }
        std::cout << s << std::endl;
        s.clear();
    });
    loop->loop();
    return 0;
}