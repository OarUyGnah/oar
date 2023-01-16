#ifndef __OAR_ACCEPTOR_H__
#define __OAR_ACCEPTOR_H__

#include <functional>
#include <memory>
#include <utility>

namespace oar {
class EventLoop;
class Socket;
class InetAddress;
class Channel;

class Acceptor {
public:
    using NewConnectionCallback = std::function<void(Socket*, InetAddress*)>;
    Acceptor(EventLoop* loop, const InetAddress& listenaddr);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(NewConnectionCallback cb);
    void deleteSock(int fd);

private:
    // EventLoop* _loop;
    // Socket* _sock;
    std::unique_ptr<Socket> _sock;
    std::unique_ptr<InetAddress> _addr;
    std::unique_ptr<Channel> _accept_channel;
    std::shared_ptr<EventLoop> _loop;
    // InetAddress* _addr;

    // Channel* _accept_channel;

    NewConnectionCallback _new_connection_cb;
    std::vector<std::pair<Socket*, InetAddress*>> _accepted_socks; // 放到conn中
};
}

#endif