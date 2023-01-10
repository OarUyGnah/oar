#pragma once
#include "InetAddress.h"
#include "socketapi.h"
#include <cstdint>
#ifndef __OAR_SOCKET_H__

#define __OAR_SOCKET_H__

// #include "InetAddress.h"
#include "../utils/copyable.h"

namespace oar {
class Socket : oar::copyable {
public:
    Socket();
    Socket(uint16_t fd);
    ~Socket();
    int fd() const { return _fd; }
    void set_nonblocking();
    void bind(const InetAddress& addr);
    void listen();
    int accept(InetAddress& peer);
    void shutdownWR();
    void reuseaddr(bool on);
    void reuseport(bool on);

private:
    int _fd;
};
}

#endif