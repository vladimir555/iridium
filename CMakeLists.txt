cmake_minimum_required(VERSION 3.5)

# clang or gcc
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(iridium)

set(CMAKE_CXX_STANDARD 17)

# release or debug
#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")

# components
set(CONFIG_POSTGRES 1)
set(CONFIG_OPENSSL  1)

if (CONFIG_POSTGRES)
    add_definitions(-DBUILD_FLAG_POSTGRES)
    set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/include")
    find_package(PostgreSQL REQUIRED)
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

include_directories(
    /usr/local/include
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/ssl/include)

# deb package
# todo: dependencies postgres deb
# postgresql-all openssl libssl-dev
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME lib${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION    "${PROJECT_NAME} C++ framework without dependencies")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER     "Bulaev Vladimir <bulaev_vladimir@mail.ru>")
#set(CPACK_DEBIAN_PACKAGE_DEBUG "YES")
include(CPack)

# static or shared
#add_library(${PROJECT_NAME} STATIC ${SOURCE})
add_library(${PROJECT_NAME} SHARED ${SOURCE})

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

add_executable(${PROJECT_TEST_NAME} ${SOURCE_TEST})

target_link_libraries(
    ${PROJECT_TEST_NAME}
    ${PROJECT_NAME}
    ${OPENSSL_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)
