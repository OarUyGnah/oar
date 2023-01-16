#ifndef __OAR_TCPCONNECTION_H__
#define __OAR_TCPCONNECTION_H__

#include "oar/net/InetAddress.h"
#include <functional>
#include <memory>
namespace oar {
class EventLoop;
class Channel;
class Socket;
class StringBuffer;

class TcpConnection {
public:
    using DeleteConnectionCallback = std::function<void(Socket*)>;
    using OnConnectionCallback = std::function<void(TcpConnection*)>;
    using OnRecvCallback = std::function<void(TcpConnection*)>;
    enum State {
        Connected,
        Closed,
        Connecting,
        Invalid
    };

public:
    TcpConnection(EventLoop* _loop, Socket* _sock);
    ~TcpConnection();

    void setDeleteConnectionCallback(DeleteConnectionCallback cb);
    // void setOnConnectCallback(OnConnectionCallback cb);
    void setOnRecvCallback(OnRecvCallback cb);
    // TODO
    // void onRecv(OnRecvCallback);
    // void onConnect(OnConnectionCallback);

    State state() { return _state; }
    void close();
    StringBuffer* readBuffer() const { return _rd_buf.get(); }
    StringBuffer* writeBuffer() const { return _wr_buf.get(); }
    Socket* socket() const { return _sock.get(); }

    void read();
    void write();
    const InetAddress& addr() { return _addr; }
    void setPeerAddr(const InetAddress& addr) { _addr = addr; }

    void send(std::string s);

private:
    void blockingRead();
    void nonblockingRead();
    void blockingWrite();
    void nonblockingWrite();
    void defaultRecvCallback();

private:
    std::unique_ptr<Channel> _ch;
    std::unique_ptr<Socket> _sock;
    InetAddress _addr;
    DeleteConnectionCallback _delete_cb;
    // OnConnectionCallback _on_connect_cb;
    OnRecvCallback _on_recv_cb;
    State _state;
    std::unique_ptr<StringBuffer> _rd_buf;
    std::unique_ptr<StringBuffer> _wr_buf;
};
}

#endif