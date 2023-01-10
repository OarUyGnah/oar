#include "../../oar/net/socketapi.h"
using namespace oar;
int main() {
    // unix_error("%s","123");
    unix_error("%d %ld %s",100,(long)12321231312,"oarrrr");
    return 0;
}