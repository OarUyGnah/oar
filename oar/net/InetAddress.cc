#include "InetAddress.h"
#include "../Endian.h"
#include "socketapi.h"
#include <arpa/inet.h>
#include <cstring>
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
const sockaddr_in* InetAddress::addrPtr() const
{
    return &_addr;
}

void InetAddress::setaddr(const sockaddr_in &addr) 
{
    _addr = addr;
}
}