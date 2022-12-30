#include "../../oar/net/InetAddress.h"
#include <iostream>
using namespace oar;
/**
 g++ inetaddrTest.cc   ../../oar/net/InetAddress.cc ../../oar/net/socketapi.cc -W
*/
int main() {
    InetAddress ia("127.0.0.1",9999);
    std::cout << ia.ip() << std::endl;
    std::cout << ia.port() << std::endl;
    std::cout << ia.family() << std::endl;
    // std::cout << ia.addrPtr() << std::endl;
    return 0;
}
