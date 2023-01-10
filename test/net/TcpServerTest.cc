#include "../../oar/net/TcpServer.h"
#include "../../oar/net/EventLoop.h"
using namespace oar;

// g++ TcpServerTest.cc ../../oar/net/EventLoop.cc ../../oar/net/socketapi.cc ../../oar/net/Epoll.cc ../../oar/net/Channel.cc ../../oar/net/Socket.cc ../../oar/net/InetAddress.cc ../../oar/net/TcpServer.cc ../../oar/net/TcpConnection.cc ../../oar/net/Acceptor.cc -o server 

int main() {
    EventLoop *loop = new EventLoop();
    TcpServer *server = new TcpServer(loop);
    loop->loop();
    return 0;
}