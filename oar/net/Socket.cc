#include "Socket.h"
#include "InetAddress.h"
#include "socketapi.h"
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <wchar.h>

namespace oar {
Socket::Socket() : _fd(oar::socket(PF_INET))
{
    if (_fd == -1) {
        unix_error("Socket::Socket error");
    }
}

Socket::Socket(uint16_t fd)
    : _fd(fd)
{
}

Socket::~Socket()
{
    oar::close(_fd);
}

void Socket::set_nonblocking() {
    oar::set_nonblocking(_fd);
}


void Socket::bind(const InetAddress& addr)
{
    oar::bind(_fd, (sockaddr*)(addr.addrPtr()));
}

void Socket::listen()
{
    oar::listen(_fd);
}

int Socket::accept(InetAddress& peer)
{
    sockaddr_in addr {};
    int connfd;
    if ((connfd = oar::accept(_fd, &addr)) >= 0) {
        peer.setaddr(addr);
    }
    return connfd;
}

void Socket::shutdownWR() {
    oar::shutdownWR(_fd);
}


void Socket::reuseaddr(bool on)
{
    int val = on ? 1 : 0;
    ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &val, static_cast<socklen_t>(sizeof(val)));
}

void Socket::reuseport(bool on)
{
    int val = on ? 1 : 0;
    int ret = ::setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &val, static_cast<socklen_t>(sizeof(val)));
    if(ret < 0 && on) {
        unix_error("Socket::reuseport error");
    }
}
}