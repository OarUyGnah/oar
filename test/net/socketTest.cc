#include "../../oar/net/InetAddress.h"
#include "../../oar/net/Socket.h"
#include <cstdio>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
/** 
 nc -v -l 127.0.0.1 9999
 g++ socketTest.cc ../../oar/net/InetAddress.cc ../../oar/net/Socket.cc ../../oar/net/socketapi.cc
*/
int main() {
    // int fd = oar::socket(PF_INET, SOCK_STREAM, 0);
    oar::InetAddress ia("127.0.0.1",9999);
    oar::Socket sock;
    oar::connect(sock.fd(), (sockaddr*)ia.addrPtr());
    char c;
    // dup2(sock.fd(), STDIN_FILENO);
    char buf[64];

    while (true) {
        int n = oar::read(sock.fd(), buf, sizeof(buf));
        printf("read from server => %s\n",buf);
        if (n == -1) {
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(buf, sizeof(buf));
        scanf("%s",buf);
        oar::write(sock.fd(), buf, strlen(buf));
    }

    return 0;
}