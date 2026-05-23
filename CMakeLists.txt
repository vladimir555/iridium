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
if (NOT DEFINED CONFIG_POSTGRES)
    set(CONFIG_POSTGRES 0)
endif()
if (NOT DEFINED CONFIG_MYSQL)
    set(CONFIG_MYSQL 0)
endif()
if (NOT DEFINED CONFIG_OPENSSL)
    set(CONFIG_OPENSSL 0)
endif()
# -----

if(CONFIG_POSTGRES)
    find_package(PostgreSQL REQUIRED)

    target_compile_definitions(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        BUILD_FLAG_POSTGRES
    )

    target_link_libraries(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        PostgreSQL::PostgreSQL
    )
endif()

if(CONFIG_MYSQL)
    find_package(PkgConfig)
    if(NOT PkgConfig_FOUND)
        message(FATAL_ERROR "pkg-config not found. Install: sudo apt install pkg-config")
    endif()

    set(_MYSQL_PKG_NAMES "mysqlclient" "libmariadb" "mariadb" "mariadbclient")
    foreach(_pkg IN LISTS _MYSQL_PKG_NAMES)
        pkg_check_modules(MYSQL_CLIENT QUIET IMPORTED_TARGET ${_pkg})
        if(TARGET PkgConfig::MYSQL_CLIENT)
            message(STATUS "Found MySQL/MariaDB via pkg-config: ${_pkg}")
            target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC PkgConfig::MYSQL_CLIENT)
            break()
        endif()
    endforeach()

    if(NOT TARGET PkgConfig::MYSQL_CLIENT)
        message(FATAL_ERROR "MySQL/MariaDB client not found. Tried: ${_MYSQL_PKG_NAMES}")
    endif()

    target_compile_definitions(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        BUILD_FLAG_MYSQL
    )

endif()

if(CONFIG_OPENSSL)
    find_package(OpenSSL REQUIRED)

    target_include_directories(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        ${OPENSSL_INCLUDE_DIR}
    )

    target_link_libraries(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        ${OPENSSL_LIBRARIES}
    )

    target_compile_definitions(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        BUILD_FLAG_OPENSSL
    )
endif()

install(
    PROGRAMS
        script/iridium-update-project-version.sh
    DESTINATION
        ${CMAKE_INSTALL_BINDIR}
    COMPONENT
        dev
)
