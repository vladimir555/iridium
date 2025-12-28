cmake_minimum_required(VERSION 3.25)

# -----
# Project name
if(DEFINED CONAN_PROJECT_NAME)
    set(PROJECT_NAME "${CONAN_PROJECT_NAME}")
    message(STATUS "Project name set from Conan: ${PROJECT_NAME}")
else()
    file(GLOB SUBDIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/source/*")
    foreach(dir ${SUBDIRS})
        get_filename_component(name "${dir}" NAME)
        if(IS_DIRECTORY "${dir}"
            AND NOT name MATCHES "-test$"
            AND NOT name MATCHES "-binary$")
            set(PROJECT_NAME "${name}")
            break()
        endif()
    endforeach()

    if(NOT PROJECT_NAME)
        message(FATAL_ERROR "No source/<name>/ folder found (not ending with -test or -binary)")
    endif()
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

string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_UPPER)

# Extract version components — robust to optional '=' and semicolon
string(REGEX MATCH "${PROJECT_NAME_UPPER}_VERSION_MAJOR[^0-9]*([0-9]+)" _ "${version_content}")
set(PROJECT_VERSION_MAJOR "${CMAKE_MATCH_1}")

string(REGEX MATCH "${PROJECT_NAME_UPPER}_VERSION_MINOR[^0-9]*([0-9]+)" _ "${version_content}")
set(PROJECT_VERSION_MINOR "${CMAKE_MATCH_1}")

string(REGEX MATCH "${PROJECT_NAME_UPPER}_VERSION_PATCH[^0-9]*([0-9]+)" _ "${version_content}")
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
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O0 -g -Wall -Wextra)

        # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
        # set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

        message(STATUS "Debug mode enabled: -O0 -g -Wall -Wextra")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Od /RTC1 /W4)  # /Od = disable optimizations, /RTC1 = runtime checks, /W4 = maximum warnings
        message(STATUS "Debug mode enabled: /Od /RTC1 /W4")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O3 -DNDEBUG -flto)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "Release mode enabled: -O3 -DNDEBUG -flto")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/O2 /DNDEBUG /GL)  # /O2 = maximum speed optimization, /GL = program-level optimization (LTO analog)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)  # enables /LTCG when linking
        message(STATUS "Release mode enabled: /O2 /DNDEBUG /GL")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-O2 -g -DNDEBUG)
        message(STATUS "RelWithDebInfo mode enabled: -O2 -g -DNDEBUG")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/O2 /DNDEBUG /Zi)  # /Zi = debug information
        message(STATUS "RelWithDebInfo mode enabled: /O2 /DNDEBUG /Zi")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-Os -DNDEBUG)
        message(STATUS "MinSizeRel mode enabled: -Os -DNDEBUG")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/Os /DNDEBUG)  # /Os = optimization for size
        message(STATUS "MinSizeRel mode enabled: /Os /DNDEBUG")
    endif()
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
# # auto export symbols for windows
# if(WIN32)
#     set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# endif()
# # Build shared library
# add_library(${PROJECT_NAME} SHARED ${SOURCE})
if(WIN32)
    add_library(${PROJECT_NAME} STATIC ${SOURCE})
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCE})
endif()


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

set(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/${PROJECT_NAME}-test")

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${TEST_SOURCE_DIR}/*.cpp"
)

set(TEST_CASE_PATHS "")
foreach(SRC IN LISTS TEST_SOURCES)
    file(RELATIVE_PATH REL_PATH "${TEST_SOURCE_DIR}" "${SRC}")
    string(REPLACE "\\" "/" REL_PATH "${REL_PATH}")
    list(APPEND TEST_CASE_PATHS "/${REL_PATH}")
endforeach()

list(REMOVE_DUPLICATES TEST_CASE_PATHS)

set(TEST_BIN "$<TARGET_FILE:${PROJECT_TEST_NAME}>")

foreach(TEST_PATH IN LISTS TEST_CASE_PATHS)
    add_test(
        NAME   "${TEST_PATH}" # "test_${SAFE_NAME}"
        COMMAND ${TEST_BIN} run "${TEST_PATH}"
    )
endforeach()
