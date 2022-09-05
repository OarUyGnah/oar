#ifndef __OAR_ENDIAN_H__
#define __OAR_ENDIAN_H__

#define OAR_BIG_ENDIAN 1
#define OAR_LITTLE_ENDIAN 2

#include <byteswap.h>
#include <stdint.h>
#include <endian.h>
#include <type_traits>
#include <stdio.h>
namespace oar {

#if __BYTE_ORDER == __BIG_ENDIAN
#define OAR_BYTE_ORDER OAR_BIG_ENDIAN
#else
#define OAR_BYTE_ORDER OAR_LITTLE_ENDIAN
#endif

  
  
  //  8字节类型的字节序转化
  template<class T>
    typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
    byteswap(T value) {
    return (T)bswap_64((uint64_t)value);
  }

  //  4字节类型的字节序转化  
  template<class T>
    typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
    byteswap(T value) {
    return (T)bswap_32((uint32_t)value);
  }

  //  4字节类型的字节序转化
  template<class T>
    typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
    byteswap(T value) {
    return (T)bswap_16((uint16_t)value);
  }

  
  /* 只在小端机器上执行byteswap, 在大端机器上什么都不做 */
#if OAR_BYTE_ORDER == OAR_BIG_ENDIAN

  template<class T>
  T byteswapOnLittleEndian(T t) {
    return t;
  }

  template<class T>
  T byteswapOnBigEndian(T t) {
    return byteswap(t);
  }

  template<class T>
  T byteswapToNBO(T t) {
    return t;
  }
  
#else // OAR_LITTLE_ENDIAN

  template<class T>
  T byteswapOnLittleEndian(T t) {
    return byteswap(t);
  }

  template<class T>
  T byteswapOnBigEndian(T t) {
    return t;
  }
  // -> BIG_ENDIAN   NBO network byte order(BIG)
  template<class T>
  T byteswapToNBO(T t) { 
    return byteswap(t);
  }
  
#endif
  
}

#endif
