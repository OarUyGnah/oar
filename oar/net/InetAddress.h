#ifndef __OAR_INETADDRESS_H__
#define __OAR_INETADDRESS_H__

#include "../utils/copyable.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

namespace oar {

class InetAddress : oar::copyable {

public:
    InetAddress() = default;
    InetAddress(std::string ip, uint16_t port);
    InetAddress(const sockaddr_in& addr);
    InetAddress(const InetAddress& addr);
    InetAddress(InetAddress&& addr);
    sa_family_t family() const;
    std::string ip() const;
    int port() const;
    const struct sockaddr_in* addrPtr() const;
    void setaddr(const sockaddr_in& addr);

    InetAddress& operator=(const InetAddress& addr);
    InetAddress& operator=(InetAddress&& addr);

private:
    sockaddr_in _addr;
};
}

#endif