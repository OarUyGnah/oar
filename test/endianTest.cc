#include "../oar/Endian.h"
#include <stdio.h>
#include <arpa/inet.h>

//x86 linux都是小端
int main() {

  printf("Current BYTE ORDER IS %s\n",OAR_BYTE_ORDER == 1 ? "BIG_ENDIAN":"LITTLE_ENDIAN");

  uint16_t a = 1000;
  printf("%d\n",oar::byteswapOnLittleEndian(a));
  printf("htons(a) == %d\n",htons(a));
  printf("ntohs(a) == %d\n",ntohs(59395)); // 1000
  return 0;
}
