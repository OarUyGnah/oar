#ifndef __OAR_EVENTLOOP_H__
#define __OAR_EVENTLOOP_H__

// #include "net/Epoll.h"
namespace oar {
class Channel;
class Poller;
class Epoll;
class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);
    void removeChannel(Channel*);
    void stop();

private:
    // Epoll* _poller;
    Poller* _poller;
    bool _stop;
};

}

#endif