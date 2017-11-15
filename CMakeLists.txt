cmake_minimum_required(VERSION 3.5)
project(utility)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

#set(CMAKE_BUILD_TYPE "Release")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -O0")

#BUILD

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/obj CACHE PATH "Single Directory for all")

add_compile_options("-std=c++14")
add_compile_options("-Wall")

file(GLOB_RECURSE SOURCE 
    ${CMAKE_SOURCE_DIR}/source/utility/*.h
    ${CMAKE_SOURCE_DIR}/source/utility/*.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/source ${CMAKE_SOURCE_DIR}/include /usr/local/ssl/include)

#add_executable(${PROJECT_NAME} ${SOURCE})

#find_package(Boost COMPONENTS system REQUIRED)
#set(Poco_LIBRARIES libPocoFoundation.so libPocoUtil.so libPocoNet.so libPocoJSON.so)
find_package(Threads)

add_library(${PROJECT_NAME} STATIC ${SOURCE})

#TEST
enable_testing()
find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)
#find_package(PostgreSQL REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/utility/source)

set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)

#add_dependencies( ${PROJECT_TEST_NAME} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libutility.a)

# source files for test
file(GLOB_RECURSE SOURCE_TEST
    ${CMAKE_SOURCE_DIR}/source/utility_test/*.h
    ${CMAKE_SOURCE_DIR}/source/utility_test/*.cpp
)

add_executable(${PROJECT_TEST_NAME} ${SOURCE_TEST})

# Link test executable against gtest & gtest_main
target_link_libraries(
    ${PROJECT_TEST_NAME} 
    ${PROJECT_NAME}
    ${GTEST_LIBRARY} 
    ${GTEST_MAIN_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
#    ${PostgreSQL_LIBRARIES}
)

add_test(NAME ${PROJECT_TEST_NAME} COMMAND ${PROJECT_TEST_NAME})
