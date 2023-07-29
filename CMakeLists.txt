cmake_minimum_required(VERSION 3.21)

# clang or gcc
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
#set(CMAKE_CXX_COMPILER_TARGET, "wasm32-unknown-wasi")
#set(CMAKE_C_COMPILER "/usr/bin/gcc")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(iridium)


set(CMAKE_CXX_STANDARD 14)

# release
# set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")

# debug
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0 -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0 -fsanitize=thread")
  
#remove_definitions(-DNANO_PREALLOCATE_BAND_VM)

# components
set(CONFIG_POSTGRES 1)
set(CONFIG_MYSQL 1)
set(CONFIG_OPENSSL 0)
#if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
#    set(CONFIG_OPENSSL 1)
#endif()

if (CONFIG_POSTGRES)
    add_definitions(-DBUILD_FLAG_POSTGRES)
    #set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/include")
    find_package(PostgreSQL REQUIRED)
    message(${PostgreSQL_INCLUDE_DIR})
endif()

if (CONFIG_MYSQL)
    add_definitions(-DBUILD_FLAG_MYSQL)
    #set(MYSQL_TYPE_INCLUDE_DIR "/usr/include")
    include(FindPkgConfig)
    pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)
    foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
    endforeach()
    set(MySQL_INCLUDE_DIR ${LIBMYSQLCLIENT_INCLUDE_DIRS})
    link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})
endif()

if (CONFIG_OPENSSL)
    add_definitions(-DBUILD_FLAG_OPENSSL)
    find_package(OpenSSL REQUIRED)
endif()

find_package(Threads)

# iridium library
file(GLOB_RECURSE SOURCE 
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
)

source_group(TREE ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME} FILES ${SOURCE})
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(
    ${PostgreSQL_INCLUDE_DIR}
    ${MySQL_INCLUDE_DIR}
    /usr/local/include
    /usr/local/ssl/include
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/include)

# deb package
# todo: dependencies postgres deb
# postgresql-all openssl libssl-dev
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME lib${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION    "${PROJECT_NAME} C++ lightweight framework")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER     "Bulaev Vladimir <bulaev_vladimir@mail.ru>")
#set(CPACK_DEBIAN_PACKAGE_DEBUG "YES")
include(CPack)

# static or shared
add_library(${PROJECT_NAME} STATIC ${SOURCE})
#add_library(${PROJECT_NAME} SHARED ${SOURCE})

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/lib)
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

# iridium test
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

include_directories(${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source)

file(GLOB_RECURSE SOURCE_TEST
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}_test/*.h
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}_test/*.cpp
)

source_group(TREE ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}_test FILES ${SOURCE_TEST})

add_executable(${PROJECT_TEST_NAME} ${SOURCE_TEST})

target_link_libraries(
    ${PROJECT_TEST_NAME}
    ${PROJECT_NAME}
    ${OPENSSL_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    ${MySQL_LIBRARIES}
    ${LIBMYSQLCLIENT_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
