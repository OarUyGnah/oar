#ifndef __OAR_INETADDRESS_H__
#define __OAR_INETADDRESS_H__

#include "../utils/copyable.h"
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace oar {

class InetAddress : oar::copyable {

public:
    InetAddress(std::string ip,uint16_t port);
    InetAddress(const sockaddr_in& addr);

    sa_family_t family() const;
    std::string ip() const;
    int port() const;
    const sockaddr_in* addrPtr() const;
    void setaddr(const sockaddr_in &addr);
private:
    sockaddr_in _addr;
};
}

#endif