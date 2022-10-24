set(OAR_SRCS
  Thread.cc
  ThisThread.cc
  Threadpool.cc
  tools.cc
  Time.cc
  Semaphore.cc
  log.cc
  utils/FileUtil.cc
  )
add_library(oar_base ${OAR_SRCS})
target_link_libraries(oar_base pthread rt)

install(TARGETS oar_base DESTINATION lib)

set(OAR_HEADERS
  Thread.h
  ThisThread.h
  Threadpool.h
  tools.h
  Time.h
  Semaphore.h
  log.h
  utils/FileUtil.h
  )
install(FILES ${OAR_HEADERS} DESTINATION include/oar/base)
