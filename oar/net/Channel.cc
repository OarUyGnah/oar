#include "Channel.h"

#include <cstdio>
#include <sys/epoll.h>

namespace oar {
Channel::Channel(EventLoop* loop, int fd)
    : _loop(loop)
    , _fd(fd)
    , _events(0)
    , _revents(0)
    , _inpoll(false)
{
}

Channel::~Channel()
{
}

void Channel::setRevents(uint32_t revents)
{
    _revents = revents;
}

void Channel::enableReading()
{
    _events = EPOLLIN | EPOLLET;
    // _events = EPOLLIN;
    _loop->updateChannel(this);
    // printf("Channel::enableReading\n");
}

void Channel::processEvent()
{
    // printf("Channel::processEvent()\n");
    _cb();
}

}