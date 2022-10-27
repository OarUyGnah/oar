set(OAR_SRCS
  CountDownLatch.cc

  # Date.cc
  log.cc
  Semaphore.cc
  ThisThread.cc
  Thread.cc
  Threadpool.cc
  Time.cc
  tools.cc
  utils/FileUtil.cc
  utils/strutil.cc
)
add_library(oar_base ${OAR_SRCS})
target_link_libraries(oar_base pthread)

install(TARGETS oar_base DESTINATION lib)

set(OAR_HEADERS
  Condition.h
  CountDownLatch.h

  # Date.h
  Endian.h
  Exception.h
  log.h
  Mutex.h
  Semaphore.h
  Singleton.h
  ThisThread.h
  Thread.h
  Threadpool.h
  Time.h
  tools.h
  utils/FileUtil.h
  utils/strutil.h
  utils/copyable.h
  utils/noncopyable.h
)
install(FILES ${OAR_HEADERS} DESTINATION include/oar/base)
