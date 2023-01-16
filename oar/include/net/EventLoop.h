#ifndef __OAR_EVENTLOOP_H__
#define __OAR_EVENTLOOP_H__

// #include "net/Epoll.h"
#include <memory>
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
    // Poller* _poller;
    std::unique_ptr<Poller> _poller;
    bool _stop;
};

}

#endif