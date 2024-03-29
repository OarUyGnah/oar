#include "Epoll.h"
#include "Channel.h"
#include "socketapi.h"
#include <errno.h>
#include <stdio.h>
#include <strings.h>
#include <sys/epoll.h>
namespace oar {

Epoll::Epoll()
    : _epfd(epoll_create(1))
// , _events(new epoll_event[MAX_EPOLL_EVENTS])
{
    if (_epfd == -1) {
        unix_error("Epoll::Epoll error");
        // delete[] _events;
    }
    _epoll_events.reserve(MAX_EPOLL_EVENTS);
    // bzero(_events, sizeof(epoll_event) * MAX_EPOLL_EVENTS);
}

Epoll::~Epoll()
{
    if (_epfd != -1) {
        oar::close(_epfd);
        _epfd = -1;
    }
    // delete[] _events;
}

void Epoll::addfd(int fd, uint32_t events)
{
    epoll_event ev;
    bzero(&ev, sizeof(epoll_event));
    ev.data.fd = fd;
    ev.events = events;
    if (epoll_ctl(_epfd, OP::ADD, fd, &ev) < 0) {
        int errnum = errno;
        unix_error("Epoll::addfd errno is %d", errnum);
    }
}

void Epoll::updateChannel(Channel* ch)
{
    int fd = ch->fd();
    epoll_event ev;
    bzero(&ev, sizeof(epoll_event));
    ev.data.ptr = ch;
    ev.events = ch->events();
    if (!ch->inEpoll()) {
        if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
            int errnum = errno;
            unix_error("Epoll::updateChannel OP::ADD error, errno is %d", errnum);
        }
        ch->setInEpoll();
        // printf("already in epoll\n");
    } else {
        if (epoll_ctl(_epfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
            int errnum = errno;
            unix_error("Epoll::updateChannel OP::MOD error, errno is %d", errnum);
        }
    }
}

std::vector<Channel*> Epoll::poll(int timeout)
{
    // printf("start poll......\n");
    int nfds = epoll_wait(_epfd, &_epoll_events[0], MAX_EPOLL_EVENTS, timeout);
    if (nfds == -1) {
        unix_error("Epoll::poll error");
    }
    // printf("nfds is %d\n", nfds);
    std::vector<oar::Channel*> channels;
    for (int i = 0; i < nfds; ++i) {
        Channel* c = static_cast<Channel*>(_epoll_events[i].data.ptr);
        c->setRevents(_epoll_events[i].events);
        channels.emplace_back(c);
        // printf("channel %d", c->fd());
    }
    // printf("\nend poll......\n");
    return channels;
}

}