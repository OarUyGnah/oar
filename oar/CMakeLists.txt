cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(oar
  VERSION 0.1
  DESCRIPTION "oar"
  LANGUAGES C CXX
)

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT)

if(EXISTS "${PATH_TO_CMAKELISTS_TXT}")
  message(FATAL_ERROR "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake .. \" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif()

set(OAR_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(OAR_CLANG_SEARCH_PATH "/usr/bin")

# test
enable_testing()

# clang-format
if(NOT DEFINED CLANG_FORMAT_BIN)
  # attempt to find the binary if user did not specify
  find_program(CLANG_FORMAT_BIN
    NAMES clang-format clang-format-8
    HINTS ${OAR_CLANG_SEARCH_PATH})
endif()

if("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
  message(WARNING "oar/main couldn't find clang-format.")
else()
  message(STATUS "oar/main found clang-format at ${CLANG_FORMAT_BIN}")
endif()

# clang-tidy
if(NOT DEFINED CLANG_TIDY_BIN)
  # attempt to find the binary if user did not specify
  find_program(CLANG_TIDY_BIN
    NAMES clang-tidy clang-fidy-8
    HINTS ${OAR_CLANG_SEARCH_PATH})
endif()

if("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
  message(WARNING "oar/main couldn't find clang-tidy.")
else()
  # Output compile_commands.json
  set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
  message(STATUS "oar/main found clang-fidy at ${CLANG_TIDY_BIN}")
endif()

# cpplint
find_program(CPPLINT_BIN
  NAMES cpplint cpplint.py
  HINTS ${OAR_BUILD_SUPPORT_DIR})

if("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
  message(WARNING "oar/main couldn't find cpplint.")
else()
  message(STATUS "oar/main found cpplint at ${CPPLINT_BIN}")
endif()

# Compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -std=c++17 -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-attributes") # TODO: remove
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fPIC")

set(GCC_COVERAGE_LINK_FLAGS "-fPIC")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND CXX_FLAGS "-Wno-null-dereference")
  list(APPEND CXX_FLAGS "-Wno-sign-conversion")
  list(APPEND CXX_FLAGS "-Wno-unused-local-typedef")
  list(APPEND CXX_FLAGS "-Wthread-safety")
  list(REMOVE_ITEM CXX_FLAGS "-rdynamic")
endif()

# Output directory.
# message("CMAKE_BINARY_DIR " ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
message("PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
set(OAR_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(OAR_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
include_directories(${OAR_SRC_INCLUDE_DIR} ${OAR_TEST_INCLUDE_DIR})

add_subdirectory(include)
add_subdirectory(src)
