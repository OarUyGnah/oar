
#ifdef OS_LINUX
#include <sys/epoll.h>
#endif
#include "utils/noncopyable.h"
#include <vector>
#ifdef OS_MACOS
#include <sys/event.h>
#endif

namespace oar {
#define MAX_POLLER_EVENTS 1000

class Channel;
class Poller : oar::Noncopyable {
public:
    enum OP {
        ADD = 1,
        DEL,
        MOD
    };

    Poller();
    ~Poller();

    void addfd(int fd, uint32_t events);
    void updateChannel(Channel*);
    void removeChannel(Channel*);

    std::vector<Channel*> poll(int timeout = -1);

private:
    int _fd;
// epoll_event* _events;
#ifdef OS_LINUX
    std::vector<struct epoll_event> _events;
#endif
#ifdef OS_MACOS
    std::vector<struct kevent> _events;
#endif
};
}