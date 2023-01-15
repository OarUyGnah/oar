#pragma once
#include "oar/net/InetAddress.h"
#include "oar/net/socketapi.h"
#include <cstdint>
#ifndef __OAR_SOCKET_H__

#define __OAR_SOCKET_H__

// #include "InetAddress.h"
#include "oar/utils/copyable.h"

namespace oar {
class Socket : oar::copyable {
public:
    Socket();
    Socket(uint16_t fd);
    ~Socket();
    int fd() const { return _fd; }
    void set_nonblocking();
    bool isNonBlocking() { return (fcntl(_fd, F_GETFL) & O_NONBLOCK) != 0; }
    void bind(const InetAddress& addr);
    void listen();
    int accept(InetAddress& peer);
    void shutdownWR();
    void reuseaddr(bool on);
    void reuseport(bool on);
    // void setInetAddress(const InetAddress& addr) { *_addr = addr; }
    // const InetAddress* sockaddr() const { return _addr; }
    // const InetAddress* sockaddr() const { return _addr; }

private:
    int _fd;
    // InetAddress* _addr;
};
}

#endif