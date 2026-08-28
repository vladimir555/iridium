# Copyright © 2019 Bulaev Vladimir.
# Contacts: <bulaev_vladimir@mail.ru>
# License: https://www.gnu.org/licenses/lgpl-3.0

cmake_minimum_required(VERSION 3.23)
project("")
include("cmake/iridium.cmake")

iridium_detect_project(
    CONTACT
        "Vladimir Bulaev <bulaev_vladimir@mail.ru>"
    HOMEPAGE_URL
        "https://vladimir555.github.io/iridium/index.html"
    DESCRIPTION
        "Iridium C++ library"
)

# ----- optional components
if (NOT DEFINED IRIDIUM_BUILD_FLAG_POSTGRES)
    set(IRIDIUM_BUILD_FLAG_POSTGRES 0)
endif()
if (NOT DEFINED IRIDIUM_BUILD_FLAG_MYSQL)
    set(IRIDIUM_BUILD_FLAG_MYSQL 0)
endif()
if (NOT DEFINED IRIDIUM_BUILD_FLAG_OPENSSL)
    set(IRIDIUM_BUILD_FLAG_OPENSSL 0)
endif()
# -----

if(IRIDIUM_BUILD_FLAG_POSTGRES)
    find_package(PostgreSQL REQUIRED)

    target_compile_definitions(
        ${LIBRARY_TARGET_NAME}
        PRIVATE
        IRIDIUM_BUILD_FLAG_POSTGRES
    )

    target_link_libraries(
        ${LIBRARY_TARGET_NAME}
        PRIVATE
        PostgreSQL::PostgreSQL
    )
endif()

if(IRIDIUM_BUILD_FLAG_MYSQL)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        set(_MYSQL_PKG_NAMES "mysqlclient" "libmariadb" "mariadb" "mariadbclient")
        foreach(_pkg IN LISTS _MYSQL_PKG_NAMES)
            pkg_check_modules(MYSQL_CLIENT QUIET IMPORTED_TARGET ${_pkg})
            if(TARGET PkgConfig::MYSQL_CLIENT)
                message(STATUS "Found MySQL/MariaDB via pkg-config: ${_pkg}")
                target_link_libraries(
                    ${LIBRARY_TARGET_NAME}
                    PRIVATE
                    PkgConfig::MYSQL_CLIENT
                )
                break()
            endif()
        endforeach()

        if(NOT TARGET PkgConfig::MYSQL_CLIENT)
            message(FATAL_ERROR "MySQL/MariaDB client not found. Tried: ${_MYSQL_PKG_NAMES}")
        endif()

        target_compile_definitions(
            ${LIBRARY_TARGET_NAME}
            PRIVATE
            IRIDIUM_BUILD_FLAG_MYSQL
        )
    else()
        find_package(libmysqlclient REQUIRED CONFIG)
        target_link_libraries(
            ${LIBRARY_TARGET_NAME}
            PRIVATE
            libmysqlclient::libmysqlclient
        )
    endif()
endif()

if(IRIDIUM_BUILD_FLAG_OPENSSL)
    find_package(OpenSSL REQUIRED)

    target_include_directories(
        ${LIBRARY_TARGET_NAME}
        PRIVATE
        ${OPENSSL_INCLUDE_DIR}
    )

    target_link_libraries(
        ${LIBRARY_TARGET_NAME}
        PRIVATE
        ${OPENSSL_LIBRARIES}
    )
endif()

install(
    FILES
        cmake/iridium.cmake
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    COMPONENT
        dev
)

install(
    PROGRAMS
        script/iridium-update-project-version.sh
    DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    COMPONENT
        dev
)
