#ifndef __OAR_TCPCONNECTION_H__
#define __OAR_TCPCONNECTION_H__

#include "oar/net/InetAddress.h"
#include <functional>
namespace oar {
class EventLoop;
class Channel;
class Socket;
class StringBuffer;

class TcpConnection {
public:
    using DeleteConnectionCallback = std::function<void(Socket*)>;
    using OnConnectionCallback = std::function<void(TcpConnection*)>;
    enum State {
        Connected,
        Closed,
        Failed,
        Handshaking,
        Invalid
    };

public:
    TcpConnection(EventLoop* _loop, Socket* _sock);
    ~TcpConnection();

    void setDeleteConnectionCallback(DeleteConnectionCallback cb);
    void setOnConnectCallback(OnConnectionCallback cb);
    State state() { return _state; }
    void close();
    StringBuffer* readBuffer() const { return _rd_buf; }
    StringBuffer* writeBuffer() const { return _wr_buf; }
    Socket* socket() const { return _sock; }
    void echo(int fd);

    void read();
    void write();

private:
    void blockingRead();
    void nonblockingRead();
    void blockingWrite();
    void nonblockingWrite();

private:
    EventLoop* _loop;
    Channel* _ch;
    Socket* _sock;
    InetAddress* _addr;
    DeleteConnectionCallback _delete_cb;
    OnConnectionCallback _on_connect_cb;
    State _state;
    StringBuffer* _rd_buf;
    StringBuffer* _wr_buf;
};
}

#endif