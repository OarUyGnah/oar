#include "../oar/Endian.h"
#include <arpa/inet.h>
#include <stdio.h>

// x86 linux都是小端
int main()
{

    printf("Current BYTE ORDER IS %s\n", OAR_BYTE_ORDER == 1 ? "BIG_ENDIAN" : "LITTLE_ENDIAN");

    uint16_t a = 1000;
    uint32_t b = 2000;
    uint64_t c = 1;
    printf("%d\n", oar::byteswapOnLittleEndian(a));
    printf("htons(a) == %d\n", htons(a));
    printf("ntohs(a) == %d\n", ntohs(59395)); // 1000

    printf("hton(a) => %d\n", oar::hton(a));
    printf("hton(b) => %d\n", oar::hton32(b));
    printf("hton(c) => %ld\n", oar::hton(c));

    printf("%d\n", oar::ntoh(b));

    int d = 1000;
    printf("hton(d) => %d\n", oar::hton(d));
    printf("ntoh(hton(d)) => %d\n", oar::ntoh(oar::hton(d)));
    return 0;
}
