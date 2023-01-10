#ifndef __OAR_TCPSERVER_H__
#define __OAR_TCPSERVER_H__
#include "../Threadpool.h"
// #include "../tpool.h"
#include "../utils/noncopyable.h"
#include "Acceptor.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include <map>
#include <vector>
namespace oar {
class Socket;
class EventLoop;

class TcpServer : Noncopyable {
public:
    TcpServer(EventLoop* loop, const InetAddress& addr = { "127.0.0.1", 9999 }, const std::string& name = "Default_Server_Name");
    ~TcpServer();

    void processReadEvent(int sock);
    void newConnection(Socket* sock);
    void deleteConncetion(Socket* sock);

private:
    EventLoop* _mainloop;
    Acceptor* _accpetor;
    std::map<int, TcpConnection*> _conns;
    std::vector<EventLoop*> _subloops;
    Threadpool* _threadpool;
};
}

#endif