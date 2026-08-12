# Copyright © 2019 Bulaev Vladimir.
# Contacts: <bulaev_vladimir@mail.ru>
# License: https://www.gnu.org/licenses/lgpl-3.0

include_guard(GLOBAL)

macro(iridium_detect_project)
    cmake_parse_arguments(IRIDIUM "" "CONTACT;HOMEPAGE_URL;DESCRIPTION" "" ${ARGN})

    if(NOT IRIDIUM_CONTACT)
        message(WARNING "iridium_detect_project argument CONTACT is missing")
    endif()
    if(NOT IRIDIUM_HOMEPAGE_URL)
        message(WARNING "iridium_detect_project argument HOMEPAGE_URL is missing")
    endif()
    if(NOT IRIDIUM_DESCRIPTION)
        message(WARNING "iridium_detect_project argument DESCRIPTION is missing")
    endif()

    set(CPACK_PACKAGE_CONTACT "${IRIDIUM_CONTACT}")
    set(CPACK_PACKAGE_HOMEPAGE_URL "${IRIDIUM_HOMEPAGE_URL}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${IRIDIUM_DESCRIPTION}")

    # ----- detect project name
    if(DEFINED CONAN_PROJECT_NAME)
        set(PROJECT_NAME "${CONAN_PROJECT_NAME}")
        message(STATUS "Project name set from Conan: ${PROJECT_NAME}")
    else()
        file(GLOB SUBDIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/source/library/*")
        foreach(dir IN LISTS SUBDIRS)
            get_filename_component(name "${dir}" NAME)
            if(IS_DIRECTORY "${dir}")
                set(PROJECT_NAME "${name}")
                break()
            endif()
        endforeach()

        if(NOT PROJECT_NAME)
            message(FATAL_ERROR "No library folder found in source/library/")
        endif()
    endif()

    set(IRIDIUM_NAME "iridium")

    set(LIBRARY_TARGET_NAME
        "${PROJECT_NAME}")
    set(LIBRARY_SOURCE_DIRECTORY
        "${CMAKE_CURRENT_SOURCE_DIR}/source/library")

    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/test")
        set(TEST_TARGET_NAME
            "${PROJECT_NAME}-test")
        set(TEST_SOURCE_DIRECTORY
            "${CMAKE_CURRENT_SOURCE_DIR}/source/test")
    endif()

    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/application")
        set(APPLICATION_TARGET_NAME
            "${PROJECT_NAME}-application")
        set(APPLICATION_SOURCE_DIRECTORY
            "${CMAKE_CURRENT_SOURCE_DIR}/source/application")
    endif()
    # -----

    # ----- detect project version
    set(VERSION_HEADER "${LIBRARY_SOURCE_DIRECTORY}/${PROJECT_NAME}/version.h")

    if(NOT EXISTS "${VERSION_HEADER}")
        message(
            FATAL_ERROR
            "version.h not found: ${VERSION_HEADER}\n"
            "Run: script/iridium-update-project-version.sh to generate it")
    endif()

    file(READ "${VERSION_HEADER}" version_content)

    string(TOUPPER "${PROJECT_NAME}" PROJECT_NAME_CXX)
    string(REPLACE "-" "_" PROJECT_NAME_CXX "${PROJECT_NAME_CXX}")

    # extract version components — robust to optional '=' and semicolon
    string(REGEX MATCH "${PROJECT_NAME_CXX}_VERSION_MAJOR[^0-9]*([0-9]+)" _ "${version_content}")
    set(PROJECT_VERSION_MAJOR "${CMAKE_MATCH_1}")

    string(REGEX MATCH "${PROJECT_NAME_CXX}_VERSION_MINOR[^0-9]*([0-9]+)" _ "${version_content}")
    set(PROJECT_VERSION_MINOR "${CMAKE_MATCH_1}")

    string(REGEX MATCH "${PROJECT_NAME_CXX}_VERSION_PATCH[^0-9]*([0-9]+)" _ "${version_content}")
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

    # ----- output paths
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

    # multi-config Xcode, VS, Ninja Multi
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIG} CONFIG_UPPER)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/lib")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${CMAKE_BINARY_DIR}/lib")
    endforeach()
    # -----

    # ----- project definition
    if(NOT DEFINED CMAKE_CXX_STANDARD)
        set(CMAKE_CXX_STANDARD 17)
    endif()
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    # enable_language(CXX)
    project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)
    # -----

    # ----- build type configuration
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
            add_compile_options(-O3 -DNDEBUG -flto -mcpu=native)
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

    # ----- source scanning
    # library target
    file(GLOB_RECURSE LIBRARY_SOURCE CONFIGURE_DEPENDS
        "${LIBRARY_SOURCE_DIRECTORY}/*.h"
        "${LIBRARY_SOURCE_DIRECTORY}/*.cpp"
    )
    source_group(TREE "${LIBRARY_SOURCE_DIRECTORY}" FILES ${LIBRARY_SOURCE})

    find_package(Threads REQUIRED)

    if(WIN32)
        add_library(${LIBRARY_TARGET_NAME} STATIC ${LIBRARY_SOURCE})
    else()
        add_library(${LIBRARY_TARGET_NAME} SHARED ${LIBRARY_SOURCE})
    endif()

    get_cmake_property(_all_vars VARIABLES)
    foreach(_var IN LISTS _all_vars)
        if(_var MATCHES "^BUILD_FLAG_" AND NOT _var MATCHES "^CMAKE_")
            if(DEFINED ${_var} AND ${_var})
                target_compile_definitions(${LIBRARY_TARGET_NAME} PUBLIC ${_var})
            endif()
        endif()
    endforeach()

    target_include_directories(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${LIBRARY_SOURCE_DIRECTORY}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source/external>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    target_link_libraries(
        ${LIBRARY_TARGET_NAME}
        PUBLIC
        Threads::Threads
    )

    if (NOT PROJECT_NAME STREQUAL IRIDIUM_NAME)
        target_link_libraries(
            ${LIBRARY_TARGET_NAME}
            PUBLIC
            ${IRIDIUM_NAME}::${IRIDIUM_NAME}
        )
    endif()

    # test target
    if (DEFINED TEST_TARGET_NAME)
        file(GLOB_RECURSE TEST_SOURCE CONFIGURE_DEPENDS
            "${TEST_SOURCE_DIRECTORY}/*.h"
            "${TEST_SOURCE_DIRECTORY}/*.cpp"
        )
        source_group(TREE "${TEST_SOURCE_DIRECTORY}" FILES ${TEST_SOURCE})

        add_executable(
            ${TEST_TARGET_NAME}
            ${TEST_SOURCE})

        if(APPLE AND DEFINED CONAN_PROJECT_NAME)
            set_target_properties(
                ${TEST_TARGET_NAME}
                PROPERTIES
                INSTALL_RPATH "@executable_path/../lib"
                BUILD_WITH_INSTALL_RPATH TRUE
            )
        endif()

        target_link_libraries(
            ${TEST_TARGET_NAME}
            PRIVATE
            ${LIBRARY_TARGET_NAME}
        )

        # ctest integration
        enable_testing()

        set(TEST_CASE_PATHS "")

        foreach(TEST_SOURCE_FILE_PATH IN LISTS TEST_SOURCE)
            file(RELATIVE_PATH TEST_SOURCE_FILE_RELATIVE_PATH "${TEST_SOURCE_DIRECTORY}" "${TEST_SOURCE_FILE_PATH}")
            string(REPLACE "\\" "/" TEST_SOURCE_FILE_RELATIVE_PATH "${TEST_SOURCE_FILE_RELATIVE_PATH}")
            list(APPEND TEST_CASE_PATHS "/${TEST_SOURCE_FILE_RELATIVE_PATH}")
        endforeach()

        foreach(TEST_CASE_PATH IN LISTS TEST_CASE_PATHS)
            add_test(
                NAME
                    "${TEST_CASE_PATH}"
                COMMAND
                    ${TEST_TARGET_NAME} run --print-result=ctest-vscode "${TEST_CASE_PATH}"
            )
            set_tests_properties("${TEST_CASE_PATH}" PROPERTIES TIMEOUT 10)
        endforeach()
    endif()
    # -----

    # application target
    if (DEFINED APPLICATION_TARGET_NAME)
        file(GLOB_RECURSE APPLICATION_SOURCE CONFIGURE_DEPENDS
            "${APPLICATION_SOURCE_DIRECTORY}/*.h"
            "${APPLICATION_SOURCE_DIRECTORY}/*.cpp"
        )
        source_group(TREE "${APPLICATION_SOURCE_DIRECTORY}" FILES ${APPLICATION_SOURCE})

        add_executable(${APPLICATION_TARGET_NAME} ${APPLICATION_SOURCE})

        set_target_properties(
            ${APPLICATION_TARGET_NAME}
            PROPERTIES OUTPUT_NAME
            "${PROJECT_NAME}"
        )

        if(APPLE AND DEFINED CONAN_PROJECT_NAME)
            set_target_properties(
                ${APPLICATION_TARGET_NAME}
                PROPERTIES
                INSTALL_RPATH "@executable_path/../lib"
                BUILD_WITH_INSTALL_RPATH TRUE
            )
        endif()

        target_link_libraries(
            ${APPLICATION_TARGET_NAME}
            PRIVATE
            ${LIBRARY_TARGET_NAME}
        )

        if (NOT PROJECT_NAME STREQUAL IRIDIUM_NAME)
            target_link_libraries(
                ${APPLICATION_TARGET_NAME}
                PRIVATE
                ${IRIDIUM_NAME}::${IRIDIUM_NAME}
            )
        endif()
    endif()
    # -----

    # ----- detect arch
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64)")
        set(ARCH "arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64)")
        set(ARCH "amd64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i?86|x86)")
        set(ARCH "i386")
    else()
        set(ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
    # -----

    # ----- detect OS
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(OS_NAME "linux")
        if(EXISTS "/etc/os-release")
            file(READ "/etc/os-release" content LIMIT 4096)
            string(REGEX MATCH "[Ii][Dd][ \t]*=[ \t]*[\"']?([a-zA-Z0-9\\-]+)[\"']?" matched "${content}")
            if(matched)
                string(TOLOWER "${CMAKE_MATCH_1}" detected_id)
                set(OS_NAME "${detected_id}")
            endif()
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        set(OS_NAME "darwin")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        set(OS_NAME "freebsd")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(OS_NAME "windows")
    else()
        set(OS_NAME "unknown")
    endif()
    # -----

    # ----- make install
    include(GNUInstallDirs)

    set_target_properties(
        ${LIBRARY_TARGET_NAME}
        PROPERTIES VERSION
            ${PROJECT_VERSION}
        SOVERSION
            ${PROJECT_VERSION_MAJOR}
    )

    set_target_properties(
        ${APPLICATION_TARGET_NAME}
        PROPERTIES VERSION
            ${PROJECT_VERSION}
        SOVERSION
            ${PROJECT_VERSION_MAJOR}
    )

    set(_CONFIG_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in")
    if(NOT EXISTS "${_CONFIG_TEMPLATE}")
        set(_AUTO_IN "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in")
        file(WRITE "${_AUTO_IN}"
            "@PACKAGE_INIT@\n"
            "include(\"\${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@-targets.cmake\")\n"
            "check_required_components(@PROJECT_NAME@)\n"
        )
        set(_CONFIG_TEMPLATE "${_AUTO_IN}")
    endif()

    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        "${_CONFIG_TEMPLATE}"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        INSTALL_DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        VERSION
            "${PROJECT_VERSION}"
        COMPATIBILITY
            SameMinorVersion
    )

    # runtime, dev, dbg
    install(
        TARGETS
            ${LIBRARY_TARGET_NAME}
        EXPORT
            ${PROJECT_NAME}-targets
        LIBRARY DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
        COMPONENT
            runtime
        ARCHIVE DESTINATION
            ${CMAKE_INSTALL_LIBDIR}
        COMPONENT
            runtime
        RUNTIME DESTINATION
            ${CMAKE_INSTALL_BINDIR}
        COMPONENT
            runtime
    )
    install(
        DIRECTORY
            ${LIBRARY_SOURCE_DIRECTORY}/${PROJECT_NAME}/
        COMPONENT
            dev
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        FILES_MATCHING PATTERN
            "*.h"
    )
    install(
        EXPORT
            ${PROJECT_NAME}-targets
        COMPONENT
            dev
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE
            ${PROJECT_NAME}::
    )
    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        COMPONENT
            dev
        DESTINATION
            ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    )
    if (DEFINED APPLICATION_TARGET_NAME)
        install(
            TARGETS
                ${APPLICATION_TARGET_NAME}
            RUNTIME DESTINATION
                ${CMAKE_INSTALL_BINDIR}
            COMPONENT
                runtime
        )
    endif()
    # -----

    # ----- package metadata
    set(CPACK_PACKAGE_NAME
        "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION
        "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_FILE_NAME
        "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}+${OS_NAME}_${ARCH}")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
    # -----

    # ----- CPack Configuration (3-component split: runtime / dev / dbg)
    unset(CPACK_GENERATOR)
    find_program(DPKG_EXECUTABLE dpkg)
    find_program(RPM_EXECUTABLE rpm)

    set(CPACK_STRIP_FILES FALSE)

    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CPACK_STRIP_FILES ON)
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
        set(CPACK_GENERATOR "FREEBSD")
        set(CPACK_FREEBSD_PACKAGE_LICENSE "LGPL-3.0")
        set(CPACK_FREEBSD_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
        set(CPACK_FREEBSD_PACKAGE_CATEGORIES "devel")
        set(CPACK_FREEBSD_PACKAGE_ORIGIN "devel/${PROJECT_NAME}")
        set(CPACK_FREEBSD_PACKAGE_COMMENT "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
        set(CPACK_FREEBSD_PACKAGE_WWW "${CPACK_PACKAGE_HOMEPAGE_URL}")
    elseif(DPKG_EXECUTABLE)
        set(CPACK_GENERATOR "DEB")
        set(CPACK_DEB_COMPONENT_INSTALL ON)
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
        set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
        set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "${IN_PACKAGE_DEPENDENCIES}")
        set(CPACK_DEBIAN_DEBUGINFO_PACKAGE ON)
        set(CPACK_DEBIAN_DEV_PACKAGE_DEPENDS "${PROJECT_NAME} (= ${PROJECT_VERSION})")
        set(CPACK_DEBIAN_DBG_PACKAGE_DEPENDS "${PROJECT_NAME} (= ${PROJECT_VERSION})")
        set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "${PROJECT_NAME}")
        set(CPACK_DEBIAN_DEV_PACKAGE_NAME "${PROJECT_NAME}-dev")
    elseif(RPM_EXECUTABLE)
        set(CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_RELEASE "1")
        set(CPACK_RPM_PACKAGE_GROUP "System Environment/Libraries")
        set(CPACK_RPM_PACKAGE_AUTOREQ ON)
        set(CPACK_RPM_PACKAGE_AUTOPROV ON)
        set(CPACK_RPM_COMPONENT_INSTALL ON)
    else()
        set(CPACK_GENERATOR "TGZ")
    endif()

    include(CPack)

    cpack_add_component(
        runtime
        DISPLAY_NAME
            "${PROJECT_NAME}"
        DESCRIPTION
            "${PROJECT_NAME} runtime library"
        REQUIRED
    )
    cpack_add_component(
        dev
        DISPLAY_NAME
            "${PROJECT_NAME}-dev"
        DESCRIPTION
            "${PROJECT_NAME} headers and CMake config"
        DEPENDS
            runtime
    )
    # -----
endmacro()
