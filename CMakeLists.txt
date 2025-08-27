cmake_minimum_required(VERSION 3.25)

# ----------------------------
# Read version from version.txt BEFORE project()
# ----------------------------
file(
    READ "${CMAKE_SOURCE_DIR}/version.txt"
    PROJECT_VERSION_STRING
)
string(
    STRIP
    "${PROJECT_VERSION_STRING}" PROJECT_VERSION_STRING
)

# parse major.minor.patch
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" _ UNUSED "${PROJECT_VERSION_STRING}")
set(PROJECT_VERSION_MAJOR "${CMAKE_MATCH_1}")
set(PROJECT_VERSION_MINOR "${CMAKE_MATCH_2}")
set(PROJECT_VERSION_PATCH "${CMAKE_MATCH_3}")
set(PROJECT_VERSION "${PROJECT_VERSION_STRING}")
message(STATUS "Project version from version.txt: ${PROJECT_VERSION}")

# ----------------------------
# Project
# ----------------------------
project(
    iridium
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_options(-O0 -Wall -Wextra -g)
endif()

# Components flags
set(CONFIG_POSTGRES 0)
set(CONFIG_MYSQL    0)
set(CONFIG_OPENSSL  0)

if (CONFIG_POSTGRES)
    add_definitions(-DBUILD_FLAG_POSTGRES)
    find_package(PostgreSQL REQUIRED)
endif()

if (CONFIG_MYSQL)
    add_definitions(-DBUILD_FLAG_MYSQL)
    include(FindPkgConfig)
    pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)
    set(MySQL_INCLUDE_DIR ${LIBMYSQLCLIENT_INCLUDE_DIRS})
    link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})
endif()

if (CONFIG_OPENSSL)
    add_definitions(-DBUILD_FLAG_OPENSSL)
    find_package(OpenSSL REQUIRED)
endif()

find_package(Threads)

# Library sources
file(GLOB_RECURSE SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/*.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME} FILES ${SOURCE})

include_directories(
    ${PostgreSQL_INCLUDE_DIR}
    ${MySQL_INCLUDE_DIR}
    /usr/local/include
    /usr/local/ssl/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
    ${CMAKE_CURRENT_SOURCE_DIR}/external)

# Build shared library
add_library(${PROJECT_NAME} SHARED ${SOURCE})

# ----------------------------
# Install rules
# ----------------------------
include(GNUInstallDirs)

# Library (iridium)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT iridium
)

# Headers (iridium-dev)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}/
    COMPONENT iridium-dev
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.h"
)

# ----------------------------
# Packaging: Auto-detect generator and arch
# ----------------------------

# Determine architecture
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)")
    set(ARCH "arm64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)")
    set(ARCH "amd64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i?86|x86)")
    set(ARCH "i386")
else()
    set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Detect available packaging tools
find_program(DPKG_EXECUTABLE dpkg)
find_program(RPM_EXECUTABLE rpm)

# Clear CPACK_GENERATOR
unset(CPACK_GENERATOR)

# Auto-detect generator based on available tools
if(DPKG_EXECUTABLE)
    set(CPACK_GENERATOR "DEB")
    # Auto-detect shared lib deps
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
    # Fallback: TGZ or ZIP
    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(CPACK_GENERATOR "ZIP")
    else()
        set(CPACK_GENERATOR "TGZ")
    endif()
endif()

# Basic package metadata
set(CPACK_PACKAGE_NAME "iridium")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_CONTACT "Vladimir Bulaev <bulaev_vladimir@mail.ru>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Iridium C++ library")

function(detect_os_name)
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OS_NAME "linux" PARENT_SCOPE)  # fallback

        if(EXISTS "/etc/os-release")
            file(READ "/etc/os-release" content LIMIT 4096)
            # find ID=..., ID="...", ID='...'
            string(REGEX MATCH "[Ii][Dd][ \t]*=[ \t]*[\"']?([a-zA-Z0-9\\-]+)[\"']?" matched "${content}")
            if(matched)
                set(detected_id "${CMAKE_MATCH_1}")
                string(TOLOWER "${detected_id}" detected_id)
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

# Set package name
if(NOT CPACK_PACKAGE_FILE_NAME)
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}+${OS_NAME}_${ARCH}")
endif()

# Components
set(CPACK_COMPONENTS_ALL iridium iridium-dev)


# Install prefix inside package
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
set(CPACK_SET_DESTDIR ON)

# Include CPack at the end
include(CPack)

cpack_add_component(iridium
    DISPLAY_NAME "iridium"
    DESCRIPTION "Iridium shared library"
    REQUIRED
)

cpack_add_component(iridium-dev
    DISPLAY_NAME "iridium-dev"
    DESCRIPTION "Iridium development headers and library"
    DEPENDS iridium
)

# ----------------------------
# Testing
# ----------------------------
set(PROJECT_TEST_NAME ${PROJECT_NAME}-test)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}/source
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
