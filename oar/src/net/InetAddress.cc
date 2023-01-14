#include "oar/net/InetAddress.h"
#include "oar/Endian.h"
#include "oar/net/socketapi.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
namespace oar {
InetAddress::InetAddress(std::string ip, uint16_t port)
{
    memset(&_addr, 0, sizeof(sockaddr_in));
    oar::setaddr(ip.c_str(), port, _addr);
}
InetAddress::InetAddress(const sockaddr_in& addr)
    : _addr(addr)
{
}

InetAddress::InetAddress(const InetAddress& addr)
{
    _addr = *addr.addrPtr();
}
InetAddress::InetAddress(InetAddress&& addr)
{
    _addr = std::move(*addr.addrPtr());
}

sa_family_t InetAddress::family() const
{
    return _addr.sin_family;
}
std::string InetAddress::ip() const
{
    char buf[64];
    ::inet_ntop(AF_INET, &_addr.sin_addr, buf, static_cast<socklen_t>(sizeof(buf)));
    return buf;
}
int InetAddress::port() const
{
    return ntoh(_addr.sin_port);
}
const struct sockaddr_in* InetAddress::addrPtr() const
{
    return &_addr;
}

void InetAddress::setaddr(const sockaddr_in& addr)
{
    _addr = addr;
}

InetAddress& InetAddress::operator=(const InetAddress& addr)
{
    _addr = *addr.addrPtr();
    return *this;
}
InetAddress& InetAddress::operator=(InetAddress&& addr)
{
    _addr = std::move(*addr.addrPtr());
    return *this;
}
}