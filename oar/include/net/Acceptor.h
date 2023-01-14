#ifndef __OAR_ACCEPTOR_H__
#define __OAR_ACCEPTOR_H__

#include <functional>

namespace oar {
class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
public:
    using NewConnectionCallback = std::function<void(Socket*)>;
    Acceptor(EventLoop* loop, const InetAddress& listenaddr);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(NewConnectionCallback cb);

private:
    EventLoop* _loop;
    Socket* _sock;
    InetAddress* _addr;
    Channel* _accept_channel;

    NewConnectionCallback _new_connection_cb;
};
}

#endif