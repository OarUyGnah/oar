#ifndef __OAR_CHANNEL_H__
#define __OAR_CHANNEL_H__
#include "net/Epoll.h"
#include "net/EventLoop.h"
#include "utils/noncopyable.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <sys/epoll.h>

namespace oar {

class Channel : oar::Noncopyable {
public:
    // using EventCallback = std::function<void()>;
    using ReadCallback = std::function<void()>;
    using WriteCallback = std::function<void()>;

    enum EVENT {
        RD = 1,
        WR = 4,
        ET = 1u << 31
    };

    Channel(EventLoop* loop, int fd);
    ~Channel();
    int fd() const { return _fd; }
    uint32_t events() const { return _events; }
    uint32_t revents() const { return _revents; }
    void setRevents(uint32_t revents);

    bool inEpoll() { return _inpoll; }
    void setInEpoll(bool in) { _inpoll = in; }

    void enableRd();
    void enableWr();
    void processEvent();
    void etMode(bool on);

    void setReadCallback(ReadCallback cb)
    {
        _rd_cb = std::move(cb);
    }
    void setWriteCallback(WriteCallback cb) { _wr_cb = std::move(cb); }

#ifdef OS_LINUX
    bool isET()
    {
        return _events & EPOLLET;
    }
#endif
private:
    bool _inpoll;
    int _fd;
    uint32_t _events; // required events
    uint32_t _revents; // received events from epoll

    // EventCallback _cb;
    ReadCallback _rd_cb;
    WriteCallback _wr_cb;

    EventLoop* _loop;
    // EventCallback writeCallback_;
    // EventCallback closeCallback_;
    // EventCallback errorCallback_;
};

}

#endif