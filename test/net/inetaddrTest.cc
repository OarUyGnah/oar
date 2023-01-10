#include "../../oar/net/InetAddress.h"
#include <iostream>
#include <vector>
using namespace oar;
/**
 g++ inetaddrTest.cc   ../../oar/net/InetAddress.cc ../../oar/net/socketapi.cc -W
*/
int main() {
    InetAddress ia("127.0.0.1",9999);
    std::cout << ia.ip() << std::endl;
    std::cout << ia.port() << std::endl;
    std::cout << ia.family() << std::endl;
    InetAddress ia2 = ia;
    std::cout << ia2.ip() << std::endl;
    std::cout << ia2.port() << std::endl;
    std::cout << ia2.family() << std::endl;

    std::vector<char> vec(2048);
    std::cout << vec.size() << std::endl;
    // std::cout << ia.addrPtr() << std::endl;
    return 0;
}
