#include "net/EventLoop.h"
#include "net/Channel.h"
#include "net/Poller.h"
#include <algorithm>
#include <cstdio>
#include <sys/epoll.h>
#include <vector>
namespace oar {

EventLoop::EventLoop()
    : _poller(new Poller)
    , _stop(false)
{
}

EventLoop::~EventLoop()
{
    stop();
    delete _poller;
}

void EventLoop::loop()
{
    std::vector<Channel*> channels;
    while (!_stop) {
        channels = _poller->poll();
        for (Channel* ch : channels) {
            ch->processEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch)
{
    _poller->updateChannel(ch);
}

void EventLoop::removeChannel(Channel* ch)
{
    _poller->removeChannel(ch);
}

void EventLoop::stop()
{
    _stop = true;
}
}