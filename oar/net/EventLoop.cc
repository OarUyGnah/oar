#include "EventLoop.h"
#include "Channel.h"
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
        // printf("before processevent\n");
        // std::for_each(channels.begin(), channels.end(), [&](Channel* ch) {
        //     printf("in foreach\n");
        //     if(ch->events() & EPOLLIN) {
        //         printf("ch->events() & EPOLLIN\n");
        //     }
        //     ch->processEvent();
        // });
        // printf("====EventLoop::loop  print channels begin====\n");
        for (Channel* ch : channels) {
            // printf("fd %d\n", ch->fd());
            ch->processEvent();
        }
        // printf("====EventLoop::loop  print channels end====\n");
    }
}

void EventLoop::updateChannel(Channel* ch)
{
    _epoll->updateChannel(ch);
}
}