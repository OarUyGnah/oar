#include "net/Poller.h"
#include "net/Channel.h"
#include "net/socketapi.h"
#include <errno.h>
#include <stdio.h>
#include <strings.h>

namespace oar {
#ifdef OS_LINUX
Poller::Poller()
    : _fd(epoll_create(1))
{
    if (_fd == -1) {
        unix_error("Epoll::Epoll error");
    }
    _events.reserve(MAX_POLLER_EVENTS);
}

Poller::~Poller()
{
    if (_fd != -1) {
        oar::close(_fd);
        _fd = -1;
    }
}

void Poller::addfd(int fd, uint32_t events)
{
    epoll_event ev;
    bzero(&ev, sizeof(epoll_event));
    ev.data.fd = fd;
    ev.events = events;
    if (epoll_ctl(_fd, OP::ADD, fd, &ev) < 0) {
        int errnum = errno;
        unix_error("Poller::addfd errno is %d", errnum);
    }
}

void Poller::updateChannel(Channel* ch)
{
    int fd = ch->fd();
    epoll_event ev;
    bzero(&ev, sizeof(epoll_event));
    ev.data.ptr = ch;
    if (ch->events() & Channel::EVENT::RD) {
        ev.events |= EPOLLIN | EPOLLPRI;
    }
    if (ch->events() & Channel::EVENT::WR) {
        ev.events |= EPOLLOUT;
    }
    if (ch->events() & Channel::EVENT::ET) {
        ev.events |= EPOLLET;
    }
    if (!ch->inEpoll()) {
        if (epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
            int errnum = errno;
            unix_error("Poller::updateChannel OP::ADD error, errno is %d", errnum);
        }
        ch->setInEpoll(true);
    } else {
        if (epoll_ctl(_fd, EPOLL_CTL_MOD, fd, &ev) == -1) {
            int errnum = errno;
            unix_error("Poller::updateChannel OP::MOD error, errno is %d", errnum);
        }
    }
}

void Poller::removeChannel(Channel* ch)
{
    if (epoll_ctl(_fd, EPOLL_CTL_DEL, ch->fd(), nullptr) == -1) {
        unix_error("Epoll::removeChannel error");
    }
    ch->setInEpoll(false);
}

std::vector<Channel*> Poller::poll(int timeout)
{
    int nfds = epoll_wait(_fd, &_events[0], MAX_POLLER_EVENTS, timeout);
    if (nfds == -1) {
        unix_error("Epoll::poll error");
    }
    std::vector<oar::Channel*> channels;
    for (int i = 0; i < nfds; ++i) {
        Channel* c = static_cast<Channel*>(_events[i].data.ptr);
        auto events = _events[i].events;
        if (events & EPOLLIN) {
            // printf("EPOLLIN\n");
            c->setRevents(Channel::EVENT::RD);
        }
        if (events & EPOLLOUT) {
            // printf("EPOLLOUT\n");
            c->setRevents(Channel::EVENT::WR);
        }
        if (events & EPOLLET) {
            // printf("EPOLLET\n");
            c->setRevents(Channel::EVENT::ET);
        }
        channels.emplace_back(c);
    }
    return channels;
}
#endif

#ifdef OS_MACOS

Poller::Poller()
    : _fd(kevent())
{
    if (_fd == -1) {
        unix_error("Poller::Poller kevent error");
    }
    _events.reserve(MAX_POLLER_EVENTS);
}

Poller::~Poller()
{
    if (_fd != -1) {
        oar::close(_fd);
        _fd = -1;
    }
}

void Poller::addfd(int fd, uint32_t events)
{
    // TODO
}

void Poller::updateChannel(Channel* ch)
{
    struct kevent ev[2];
    memset(ev, 0, sizeof(*ev) * 2);
    int n = 0;
    int fd = ch->fd();
    int op = EV_ADD;
    if (ch->events() & Channel::EVENT::ET) {
        op |= EV_CLEAR;
    }
    if (ch->events() & Channel::EVENT::READ) {
        EV_SET(&ev[n++], fd, EVFILT_READ, op, 0, 0, ch);
    }
    if (ch->events() & Channel::EVENT::WRITE) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, op, 0, 0, ch);
    }
    int r = kevent(_fd, ev, n, NULL, 0, NULL);
    if (r == -1) {
        printf("kqueue add event error\n");
        exit(-1);
    }
}

void Poller::removeChannel(Channel* ch)
{
    struct kevent ev[2];
    int n = 0;
    int fd = ch->fd();
    if (ch->events() & Channel::EVENT::READ) {
        EV_SET(&ev[n++], fd, EVFILT_READ, EV_DELETE, 0, 0, ch);
    }
    if (ch->events() & Channel::EVENT::WRITE) {
        EV_SET(&ev[n++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, ch);
    }
    int r = kevent(fd_, ev, n, NULL, 0, NULL);
    if (r == -1) {
        printf("kqueue delete event error\n");
        exit(-1);
    }
    ch->setInEpoll(false);
}

std::vector<Channel*> Poller::poll(int timeout)
{
    std::vector<Channel*> channels;
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));
    if (timeout != -1) {
        ts.tv_sec = timeout / 1000;
        ts.tv_nsec = (timeout % 1000) * 1000 * 1000;
    }
    int nfds = 0;
    if (timeout == -1) {
        nfds = kevent(_fd, nullptr, 0, _events, MAX_EVENTS, nullptr);
    } else {
        nfds = kevent(_fd, nullptr, 0, _events, MAX_EVENTS, &ts);
    }
    for (int i = 0; i < nfds; ++i) {
        Channel* ch = (Channel*)_events[i].udata;
        int events = _events[i].filter;
        if (events == EVFILT_READ) {
            ch->setRevents(Channel::EVENT::READ | Channel::EVENT::ET);
        }
        if (events == EVFILT_WRITE) {
            ch->setRevents(Channel::EVENT::WRITE | Channel::EVENT::ET);
        }
        channels.push_back(ch);
    }
    return channels;
}
#endif
}