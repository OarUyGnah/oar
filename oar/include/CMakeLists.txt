set(OAR_BASE_HEADERS
    BlockingQueue.h
    Condition.h
    CountDownLatch.h
    Endian.h
    Exception.h
    log.h
    Mutex.h
    Semaphore.h
    Singleton.h
    StringBuffer.h
    ThisThread.h
    Thread.h
    Threadpool.h
    Time.h
    tools.h
    Types.h
)

install(FILES ${OAR_BASE_HEADERS} DESTINATION include/oar)

# add_subdirectory(net)
# add_subdirectory(utils)
file(GLOB OAR_NET_HEADERS "net/*.h")
install(FILES ${OAR_NET_HEADERS} DESTINATION include/oar/net)

file(GLOB OAR_UTILS_HEADERS "utils/*.h")
message("${OAR_UTILS_HEADERS} == " ${OAR_UTILS_HEADERS}w)
install(FILES ${OAR_UTILS_HEADERS} DESTINATION include/oar/utils)