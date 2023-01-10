#include "../../oar/net/Socket.h"
#include "../../oar/net/socketapi.h"
#include <cstdio>
#include <string.h>
#include <strings.h>
using namespace oar;
// g++ clientTest.cc ../../oar/net/Socket.cc ../../oar/net/socketapi.cc ../../oar/net/InetAddress.cc -o client
// error!
int main()
{

    Socket sock;
    InetAddress ia("127.0.0.1", 9999);
    int connfd = oar::connect(sock.fd(), (sockaddr*)ia.addrPtr());
    char buf[2048];
    printf("sockfd = %d\n", sock.fd());
    while (true) {
        bzero(buf, sizeof(buf));

        scanf("%s", buf);
        printf("scanf %s\n", buf);
        int writebytes = oar::write(sock.fd(), buf, sizeof(buf));
        printf("strlen = %d write bytes = %d\n", strlen(buf), writebytes);

        bzero(buf, sizeof(buf));
        auto n = oar::read(sock.fd(), buf, sizeof(buf));
        if (n > 0) {
            printf("recv msg : %s", buf);
        } else {
            printf("read bytes %d\n", n);
            break;
        }
    }
    return 0;
}