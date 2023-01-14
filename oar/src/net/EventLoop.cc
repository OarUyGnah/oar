#include "oar/net/EventLoop.h"
#include "oar/net/Channel.h"
#include <algorithm>
#include <cstdio>
#include <sys/epoll.h>
#include <vector>

namespace oar {

EventLoop::EventLoop()
    : _epoll(new Epoll())
    , _quit(false)
{
}

EventLoop::~EventLoop()
{
    delete _epoll;
}

void EventLoop::loop()
{
    std::vector<Channel*> channels;
    while (!_quit) {
        channels = _epoll->poll();
        for (Channel* ch : channels) {
            // printf("fd %d\n", ch->fd());
            ch->processEvent();
        }
    }
}

void EventLoop::updateChannel(Channel* ch)
{
    _epoll->updateChannel(ch);
}
}