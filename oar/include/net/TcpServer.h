#ifndef __OAR_TCPSERVER_H__
#define __OAR_TCPSERVER_H__
#include "oar/Threadpool.h"
// #include "../tpool.h"
#include "oar/net/Acceptor.h"
#include "oar/net/EventLoop.h"
#include "oar/net/InetAddress.h"
#include "oar/net/TcpConnection.h"
#include "oar/utils/noncopyable.h"
#include <functional>
#include <map>
#include <vector>
namespace oar {
class Socket;
class EventLoop;

class TcpServer : Noncopyable {
public:
    using ConnectCallBack = std::function<void(TcpConnection*)>;
    TcpServer(EventLoop* loop, const InetAddress& addr = { "127.0.0.1", 9999 }, const std::string& name = "Default_Server_Name");
    ~TcpServer();

    // void processReadEvent(int sock);
    void newConnection(Socket* sock, InetAddress* addr);
    void deleteConncetion(Socket* sock);
    void setOnConnectCallback(ConnectCallBack cb);

private:
    EventLoop* _mainloop;
    Acceptor* _accpetor;
    std::map<int, TcpConnection*> _conns;
    std::vector<EventLoop*> _subloops;
    Threadpool* _threadpool;
    ConnectCallBack _on_connect_cb;
};
}

#endif