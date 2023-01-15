#include "net/Channel.h"

#include <cstdio>
#include <sys/epoll.h>

namespace oar {
Channel::Channel(EventLoop* loop, int fd)
    : _inpoll(false)
    , _fd(fd)
    , _events(0)
    , _revents(0)
    , _loop(loop)
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
    _events |= EPOLLIN | EPOLLPRI;
    _loop->updateChannel(this);
}

void Channel::processEvent()
{
    if (_revents & (EPOLLIN | EPOLLPRI)) {
        _rd_cb();
    }
    if (_revents & EPOLLOUT) {
        _wr_cb();
    }
}

void Channel::etMode(bool on)
{
    if (on)
        _events |= EPOLLET;
    else
        _events &= ~EPOLLET;
    _loop->updateChannel(this);
}
}