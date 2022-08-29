#include "ThisThread.h"
namespace oar {
  namespace ThisThread {
    __thread int  threadId = 0;
    __thread char threadIdString[32];
    __thread int  threadIdstringLen = 6;
    __thread const char* threadName = "unknown";
    
  }
}
