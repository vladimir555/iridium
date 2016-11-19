# Install script for directory: /Users/volodja/workspace/github/websocketpp/websocketpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/base64" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/base64/base64.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/client.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/close.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/asio.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/asio_ssl.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/chrono.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/connection_hdl.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/cpp11.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/functional.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/md5.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/memory.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/network.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/platforms.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/random.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/regex.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/stdint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/system_error.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/thread.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/time.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/common" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/common/type_traits.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/concurrency" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/concurrency/basic.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/concurrency" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/concurrency/none.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/asio.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/asio_client.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/asio_no_tls.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/asio_no_tls_client.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/boost_config.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/core.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/core_client.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/debug.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/debug_asio.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/debug_asio_no_tls.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/minimal_client.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/config" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/config/minimal_server.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/connection.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/connection_base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/endpoint_base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/error.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/extensions" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/extensions/extension.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/extensions/permessage_deflate" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/extensions/permessage_deflate/disabled.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/extensions/permessage_deflate" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/extensions/permessage_deflate/enabled.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/frame.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/constants.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http/impl" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/impl/parser.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http/impl" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/impl/request.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http/impl" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/impl/response.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/parser.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/request.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/http" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/http/response.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/impl" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/impl/connection_impl.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/impl" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/impl/endpoint_impl.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/impl" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/impl/utilities_impl.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/logger" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/logger/basic.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/logger" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/logger/levels.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/logger" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/logger/stub.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/logger" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/logger/syslog.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/message_buffer" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/message_buffer/alloc.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/message_buffer" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/message_buffer/message.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/message_buffer" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/message_buffer/pool.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/processors" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/processors/base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/processors" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/processors/hybi00.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/processors" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/processors/hybi07.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/processors" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/processors/hybi08.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/processors" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/processors/hybi13.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/processors" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/processors/processor.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/random" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/random/none.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/random" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/random/random_device.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/roles" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/roles/client_endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/roles" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/roles/server_endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/server.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/sha1" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/sha1/sha1.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/asio" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/asio/base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/asio" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/asio/connection.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/asio" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/asio/endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/asio/security" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/asio/security/base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/asio/security" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/asio/security/none.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/asio/security" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/asio/security/tls.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/base" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/base/connection.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/base" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/base/endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/debug" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/debug/base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/debug" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/debug/connection.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/debug" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/debug/endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/iostream" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/iostream/base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/iostream" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/iostream/connection.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/iostream" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/iostream/endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/stub" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/stub/base.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/stub" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/stub/connection.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp/transport/stub" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/transport/stub/endpoint.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/uri.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/utf8_validator.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/utilities.hpp")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/websocketpp" TYPE FILE FILES "/Users/volodja/workspace/github/websocketpp/websocketpp/version.hpp")
endif()

