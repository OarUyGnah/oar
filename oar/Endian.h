#ifndef __OAR_ENDIAN_H__
#define __OAR_ENDIAN_H__

#include <cstdint>
#define OAR_BIG_ENDIAN 1
#define OAR_LITTLE_ENDIAN 2

#include <byteswap.h>
#include <endian.h>
#include <stdint.h>
#include <stdio.h>
#include <type_traits>
namespace oar {

#if __BYTE_ORDER == __BIG_ENDIAN
#define OAR_BYTE_ORDER OAR_BIG_ENDIAN
#else
#define OAR_BYTE_ORDER OAR_LITTLE_ENDIAN
#endif

//  8字节类型的字节序转化
template <class T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
byteswap(T value)
{
    return (T)bswap_64((uint64_t)value);
}

//  4字节类型的字节序转化
template <class T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
byteswap(T value)
{
    return (T)bswap_32((uint32_t)value);
}

//  4字节类型的字节序转化
template <class T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
byteswap(T value)
{
    return (T)bswap_16((uint16_t)value);
}

/* 只在小端机器上执行byteswap, 在大端机器上什么都不做 */
#if OAR_BYTE_ORDER == OAR_BIG_ENDIAN

template <class T>
T byteswapOnLittleEndian(T t)
{
    return t;
}

template <class T>
T byteswapOnBigEndian(T t)
{
    return byteswap(t);
}

template <class T>
T byteswapToNBO(T t)
{
    return t;
}

#else // OAR_LITTLE_ENDIAN

template <class T>
T byteswapOnLittleEndian(T t)
{
    return byteswap(t);
}

template <class T>
T byteswapOnBigEndian(T t)
{
    return t;
}
// -> BIG_ENDIAN   NBO network byte order(BIG)
template <class T>
T byteswapToNBO(T t)
{
    return byteswap(t);
}

#endif

inline uint64_t hton64(uint64_t host64)
{
    return htobe64(host64);
}

inline uint32_t hton32(uint32_t host32)
{
    return htobe32(host32);
}

inline uint16_t hton16(uint16_t host16)
{
    return htobe16(host16);
}

template <typename T>
T hton(T host)
{
    switch (sizeof(host)) {
    case sizeof(uint16_t):
        return htobe16(host);
    case sizeof(uint32_t):
        return htobe32(host);
    case sizeof(uint64_t):
        return htobe64(host);
    default:
        return htobe64(host);
    }
}

inline uint64_t ntoh64(uint64_t net64)
{
    return be64toh(net64);
}

inline uint32_t ntoh32(uint32_t net32)
{
    return be32toh(net32);
}

inline uint16_t ntoh16(uint16_t net16)
{
    return be16toh(net16);
}

template <typename T>
T ntoh(T net)
{
    switch (sizeof(net)) {
    case sizeof(uint16_t):
        return be16toh(net);
    case sizeof(uint32_t):
        return be32toh(net);
    case sizeof(uint64_t):
        return be64toh(net);
    default:
        return be64toh(net);
    }
}
}

#endif
