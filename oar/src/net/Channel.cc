#include "net/Channel.h"

#include <cstdio>
#include <memory>
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
    // _revents = revents;
    _revents = 0;
    if (revents & Channel::EVENT::RD) {
        _revents |= Channel::EVENT::RD;
    }
    if (revents & Channel::EVENT::WR) {
        _revents |= Channel::EVENT::WR;
    }
    if (revents & Channel::EVENT::ET) {
        _revents |= Channel::EVENT::ET;
    }
}

void Channel::enableRd()
{
    _events |= (EPOLLIN | EPOLLPRI);
    _loop->updateChannel(this);
}

void Channel::enableWr()
{
    _events |= EPOLLOUT;
    _loop->updateChannel(this);
}

void Channel::processEvent()
{
    if (_revents & (Channel::EVENT::RD | EPOLLPRI)) {
        _rd_cb();
    }
    if (_revents & Channel::EVENT::WR) {
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