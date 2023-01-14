set(OAR_BASE_SRCS
    CountDownLatch.cc
    log.cc
    Semaphore.cc
    StringBuffer.cc
    ThisThread.cc
    Thread.cc
    Threadpool.cc
    Time.cc
    tools.cc
)
add_library(oar_base ${OAR_BASE_SRCS})
target_link_libraries(oar_base pthread)
install(TARGETS oar_base DESTINATION lib)

aux_source_directory(./utils OAR_UTILS_SRCS)
add_library(oar_utils SHARED ${OAR_UTILS_SRCS})
install(TARGETS oar_utils DESTINATION lib)
message("OAR_UTILS_SRCS == " ${OAR_UTILS_SRCS})

# add_subdirectory(net)
aux_source_directory(./net OAR_NET_SRCS)
add_library(oar_net ${OAR_NET_SRCS})
target_link_libraries(oar_net oar_base)
install(TARGETS oar_net DESTINATION lib)