#ifndef __OAR_CHANNEL_H__
#define __OAR_CHANNEL_H__
#include "../utils/noncopyable.h"
#include "Epoll.h"
#include "EventLoop.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <sys/epoll.h>

namespace oar {

class Channel : oar::Noncopyable {
public:
    using EventCallback = std::function<void()>;

    Channel(EventLoop* loop, int fd);
    ~Channel();
    int fd() const { return _fd; }
    uint32_t events() const { return _events; }
    uint32_t revents() const { return _revents; }
    void setRevents(uint32_t revents);

    bool inEpoll() { return _inpoll; }
    void setInEpoll() { _inpoll = true; }

    void setCallback(EventCallback cb) { _cb = cb; }
    void enableReading();
    void processEvent();

private:
    bool _inpoll;
    int _fd;
    uint32_t _events; // required events
    uint32_t _revents; // received events from epoll
    EventLoop* _loop;
    EventCallback _cb;
    // EventCallback writeCallback_;
    // EventCallback closeCallback_;
    // EventCallback errorCallback_;
};

}

#endif