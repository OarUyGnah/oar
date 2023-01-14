#ifndef __OAR_EVENTLOOP_H__
#define __OAR_EVENTLOOP_H__

#include "oar/net/Epoll.h"
namespace oar {
class Channel;
class Epoll;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel*);

private:
    Epoll* _epoll;
    bool _quit;
};

}

#endif