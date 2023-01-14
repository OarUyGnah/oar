#ifndef __OAR_EPOLL_H__
#define __OAR_EPOLL_H__
#include "oar/utils/noncopyable.h"
#include <sys/epoll.h>
#include <vector>

namespace oar {
#define MAX_EPOLL_EVENTS 1000

class Channel;
class Epoll : oar::Noncopyable {
public:
    enum OP {
        ADD = 1,
        DEL,
        MOD
    };

    Epoll();
    ~Epoll();

    void addfd(int fd, uint32_t events);
    void updateChannel(Channel*);
    void removeChannel(Channel*);

    std::vector<Channel*> poll(int timeout = -1);

private:
    int _epfd;
    // epoll_event* _events;
    std::vector<struct epoll_event> _epoll_events;
};
}

#endif