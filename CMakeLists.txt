cmake_minimum_required(VERSION 3.5)
# clang or gcc
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(iridium)

# release or debug
#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
#set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")

# build components
#set(BUILD_FLAG_POSTGRESQL 1)

if (BUILD_FLAG_POSTGRESQL)
    add_compile_definitions(BUILD_FLAG_POSTGRESQL)
    set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/include")
endif()


#set(OPENSSL_CRYPTO_LIBRARY "")
#set(OPENSSL_INCLUDE_DIR "")
#set(OPENSSL_ROOT_DIR "/usr/local/ssl")
#set(OPENSSL_LIBRARIES "/usr/local/ssl/lib")


#BUILD

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/obj CACHE PATH "Single Directory for all")

#add_compile_options("-std=c++14")
#add_compile_options("-Wall")

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE 
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
)

include_directories(
    /usr/local/include
    ${CMAKE_SOURCE_DIR}/source
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/ssl/include)

#add_executable(${PROJECT_NAME} ${SOURCE})

#find_package(Boost COMPONENTS system REQUIRED)
#set(Poco_LIBRARIES libPocoFoundation.so libPocoUtil.so libPocoNet.so libPocoJSON.so)
find_package(Threads)

#set(CMAKE_FIND_FRAMEWORK LAST)

# deb package
# todo: dependencies postgres deb
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME lib${PROJECT_NAME})
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION    "${PROJECT_NAME} C++ framework without dependencies")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER     "Bulaev Vladimir <bulaev_vladimir@mail.ru>")
#set(CPACK_DEBIAN_PACKAGE_DEBUG "YES")
include(CPack)

#add_library(${PROJECT_NAME} STATIC ${SOURCE})
add_library(${PROJECT_NAME} SHARED ${SOURCE})

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/lib)
install(
    DIRECTORY "${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}"
    DESTINATION "include"
    FILES_MATCHING PATTERN "*.h"
)

#TEST
set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
enable_testing()

find_package(GTest REQUIRED)

#find_package(OpenSSL REQUIRED)
if (BUILD_FLAG_POSTGRESQL)
    find_package(PostgreSQL REQUIRED)
endif()

include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source)

# source files for test
file(GLOB_RECURSE SOURCE_TEST
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}_test/*.h
    ${CMAKE_SOURCE_DIR}/source/${PROJECT_NAME}_test/*.cpp
)

add_executable(${PROJECT_TEST_NAME} ${SOURCE_TEST})

# Link test executable against gtest & gtest_main
target_link_libraries(
    ${PROJECT_TEST_NAME} 
    ${PROJECT_NAME}
    ${GTEST_LIBRARY} 
    ${GTEST_MAIN_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
#    ${OPENSSL_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
)

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})
