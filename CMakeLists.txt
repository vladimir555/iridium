cmake_minimum_required(VERSION 3.25)


# -----
# Project name
if(DEFINED CONAN_PROJECT_NAME)
    set(PROJECT_NAME "${CONAN_PROJECT_NAME}")
    message(STATUS "Project name set from Conan: ${PROJECT_NAME}")
endif()

if(NOT PROJECT_NAME)
    get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    message(STATUS "Project name auto-detected from directory: ${PROJECT_NAME}")
endif()


# -----
# Read version from source/iridium/version.h
set(VERSION_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/version.h")

if(NOT EXISTS "${VERSION_HEADER}")
    message(
        FATAL_ERROR
        "version.h not found: ${VERSION_HEADER}\n"
        "Run: ./shell/update-version.sh to generate it")
endif()

file(READ "${VERSION_HEADER}" version_content)

# Extract version components — robust to whitespace
string(REGEX MATCH "PROJECT_VERSION_MAJOR[ \t]*=[ \t]*([0-9]+)" _ "${version_content}")
set(PROJECT_VERSION_MAJOR "${CMAKE_MATCH_1}")

string(REGEX MATCH "PROJECT_VERSION_MINOR[ \t]*=[ \t]*([0-9]+)" _ "${version_content}")
set(PROJECT_VERSION_MINOR "${CMAKE_MATCH_1}")

string(REGEX MATCH "PROJECT_VERSION_PATCH[ \t]*=[ \t]*([0-9]+)" _ "${version_content}")
set(PROJECT_VERSION_PATCH "${CMAKE_MATCH_1}")

# Validate (0 is valid, empty is not)
if("${PROJECT_VERSION_MAJOR}" STREQUAL "")
    message(FATAL_ERROR "Failed to parse PROJECT_VERSION_MAJOR")
endif()
if("${PROJECT_VERSION_MINOR}" STREQUAL "")
    message(FATAL_ERROR "Failed to parse PROJECT_VERSION_MINOR")
endif()
if("${PROJECT_VERSION_PATCH}" STREQUAL "")
    message(FATAL_ERROR "Failed to parse PROJECT_VERSION_PATCH")
endif()

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
message(STATUS "Project: ${PROJECT_NAME} ${PROJECT_VERSION}")


# -----
# Project definition
project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----
# Build type configuration

if (NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type: Debug, Release, RelWithDebInfo, MinSizeRel" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-O0 -g -Wall -Wextra)
    message(STATUS "Debug mode enabled: -O0 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3 -DNDEBUG -flto)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    message(STATUS "Release mode enabled: -O3 -DNDEBUG -flto")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-O2 -g -DNDEBUG)
    message(STATUS "RelWithDebInfo mode enabled: -O2 -g -DNDEBUG")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_compile_options(-Os -DNDEBUG)
    message(STATUS "MinSizeRel mode enabled: -Os -DNDEBUG")
else()
    message(WARNING "Unknown CMAKE_BUILD_TYPE: '${CMAKE_BUILD_TYPE}'")
endif()


# -----
# Optional components
set(CONFIG_POSTGRES 0)
set(CONFIG_MYSQL    0)
set(CONFIG_OPENSSL  0)

if(CONFIG_POSTGRES)
    add_definitions(-DBUILD_FLAG_POSTGRES)
    find_package(PostgreSQL REQUIRED)
endif()

if(CONFIG_MYSQL)
    add_definitions(-DBUILD_FLAG_MYSQL)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)
    set(MySQL_INCLUDE_DIR ${LIBMYSQLCLIENT_INCLUDE_DIRS})
    link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})
endif()

if(CONFIG_OPENSSL)
    add_definitions(-DBUILD_FLAG_OPENSSL)
    find_package(OpenSSL REQUIRED)
endif()


find_package(Threads REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${PostgreSQL_INCLUDE_DIR}
    ${MySQL_INCLUDE_DIR}
    /usr/local/include
    /usr/local/ssl/include
)

file(GLOB_RECURSE SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME} FILES ${SOURCE})


# -----
# Build shared library
add_library(${PROJECT_NAME} SHARED ${SOURCE})


# -----
# Install rules
include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT ${PROJECT_NAME}
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/
    COMPONENT ${PROJECT_NAME}-dev
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)

# -----
# Packaging: Auto-detect generator and arch
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)")
    set(ARCH "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)")
    set(ARCH "amd64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i?86|x86)")
    set(ARCH "i386")
else()
    set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

find_program(DPKG_EXECUTABLE dpkg)
find_program(RPM_EXECUTABLE rpm)
unset(CPACK_GENERATOR)

if(DPKG_EXECUTABLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    if(IN_PACKAGE_DEPENDENCIES)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "${IN_PACKAGE_DEPENDENCIES}")
    endif()
    set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
elseif(RPM_EXECUTABLE)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_AUTOREQ ON)
    set(CPACK_RPM_PACKAGE_AUTOPROV ON)
    if(IN_PACKAGE_DEPENDENCIES)
        set(CPACK_RPM_PACKAGE_REQUIRES "${IN_PACKAGE_DEPENDENCIES}")
    endif()
    set(CPACK_RPM_PACKAGE_RELEASE "1")
    set(CPACK_RPM_PACKAGE_GROUP "System Environment/Libraries")
else()
    set(CPACK_GENERATOR "TGZ")
endif()

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "Vladimir Bulaev <bulaev_vladimir@mail.ru>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} C++ library")

# -----
# OS detection for package naming
function(detect_os_name)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OS_NAME "linux" PARENT_SCOPE)
        if(EXISTS "/etc/os-release")
            file(READ "/etc/os-release" content LIMIT 4096)
            string(REGEX MATCH "[Ii][Dd][ \t]*=[ \t]*[\"']?([a-zA-Z0-9\\-]+)[\"']?" matched "${content}")
            if(matched)
                string(TOLOWER "${CMAKE_MATCH_1}" detected_id)
                set(OS_NAME "${detected_id}" PARENT_SCOPE)
            endif()
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(OS_NAME "darwin" PARENT_SCOPE)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(OS_NAME "windows" PARENT_SCOPE)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        set(OS_NAME "freebsd" PARENT_SCOPE)
    else()
        set(OS_NAME "unknown" PARENT_SCOPE)
    endif()
endfunction()

detect_os_name()

if(NOT CPACK_PACKAGE_FILE_NAME)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}+${OS_NAME}_${ARCH}")
endif()

set(CPACK_COMPONENTS_ALL ${PROJECT_NAME} ${PROJECT_NAME}-dev)
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_SET_DESTDIR ON)

include(CPack)

cpack_add_component(${PROJECT_NAME}
    DISPLAY_NAME "${PROJECT_NAME}"
    DESCRIPTION "${PROJECT_NAME} shared library"
    REQUIRED
)

cpack_add_component(${PROJECT_NAME}-dev
    DISPLAY_NAME "${PROJECT_NAME}-dev"
    DESCRIPTION "${PROJECT_NAME} development headers and library"
    DEPENDS ${PROJECT_NAME}
)

# -----
# Testing
set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    /opt/homebrew/include
)

file(GLOB_RECURSE SOURCE_TEST
    ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}-test/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}-test/*.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}-test FILES ${SOURCE_TEST})

add_executable(${PROJECT_TEST_NAME} ${SOURCE_TEST})

target_link_libraries(
    ${PROJECT_TEST_NAME}
    PRIVATE
    ${PROJECT_NAME}
    ${OPENSSL_LIBRARIES}
    ${PostgreSQL_LIBRARIES}
    ${LIBMYSQLCLIENT_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

enable_testing()
add_test(
    NAME ${PROJECT_TEST_NAME}
    COMMAND ${PROJECT_TEST_NAME}
)
