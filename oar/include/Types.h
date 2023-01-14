#ifndef __OAR_TYPES_H__
#define __OAR_TYPES_H__

#include <stdint.h>
#include <string.h> // memset
#include <string>

#ifndef NDEBUG
#include <assert.h>
#endif

namespace oar {

inline void mem0(void* p, size_t n)
{
    memset(p, 0, n);
}

}

#endif // __OAR_TYPES_H__
