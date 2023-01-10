#ifndef __OAR_TCPCONNECTION_H__
#define __OAR_TCPCONNECTION_H__

#include "InetAddress.h"
#include <functional>
namespace oar {
class EventLoop;
class Channel;
class Socket;
class StringBuffer;
class TcpConnection {
    using DeleteConnectionCallback = std::function<void(Socket*)>;

public:
    TcpConnection(EventLoop* _loop, Socket* _sock);
    ~TcpConnection();

    void setDeleteConnectionCallback(DeleteConnectionCallback cb) { _delete_cb = cb; }
    void echo(int fd);

private:
    EventLoop* _loop;
    Channel* _ch;
    Socket* _sock;
    InetAddress* _addr;
    DeleteConnectionCallback _delete_cb;
    // StringBuffer* _in_buf;
    StringBuffer* _buf;
};
}

#endif