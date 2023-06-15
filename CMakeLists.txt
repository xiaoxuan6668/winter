cmake_minimum_required(VERSION 3.22)
project(winter)

# makefile has bug
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")
# -Wno-builtin-macro-redefined

include_directories(.)
include_directories(/usr/include)
include_directories(/usr/include/jsoncpp)
include_directories(/usr/local/openssl/include)
link_directories(/usr/local/openssl/lib)
link_directories(/usr/lib/x86_64-linux-gnu)

option(TEST "ON for complile test" OFF)
# find_library(YAMLCPP yaml-cpp)
# message("***", ${YAMLCPP})

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# find_package(OpenSSL REQUIRED)
# if(OPENSSL_FOUND)
#     include_directories(${OPENSSL_INCLUDE_DIR})
#     link_directories(/usr/lib/x86_64-linux-gnu)
# endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

set(LIB_SRC
    winter/address.cc
    winter/application.cc
    winter/bytearray.cc
    winter/config.cc
    winter/daemon.cc
    winter/env.cc
    winter/fd_manager.cc
    winter/fiber.cc
    winter/http/http.cc
    winter/http/http_connection.cc
    winter/http/http_parser.cc
    winter/http/http_session.cc
    winter/http/http_server.cc
    winter/http/servlet.cc
    winter/http/ws_session.cc
    winter/http/ws_servlet.cc
    winter/http/ws_server.cc
    winter/http/ws_connection.cc
    winter/hook.cc
    winter/iomanager.cc
    winter/log.cc
    winter/library.cc
    winter/module.cc
    winter/mutex.cc
    winter/scheduler.cc
    winter/socket.cc
    winter/stream.cc
    winter/streams/socket_stream.cc
    winter/streams/zlib_stream.cc
    winter/tcp_server.cc
    winter/timer.cc
    winter/thread.cc
    winter/util.cc
    winter/util/hash_util.cc
    winter/util/json_util.cc
    winter/worker.cc
    )

ragelmaker(winter/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/winter/http)
ragelmaker(winter/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/winter/http)
ragelmaker(winter/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/winter)

add_library(winter SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(winter) #__FILE__
#add_library(winter_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(winter_static PROPERTIES OUTPUT_NAME "winter")
add_library(lib-crypto SHARED IMPORTED)
set_target_properties(lib-crypto PROPERTIES IMPORTED_LOCATION
                        /usr/local/openssl/lib/libcrypto.so)
add_library(lib-openssl SHARED IMPORTED)
set_target_properties(lib-openssl PROPERTIES IMPORTED_LOCATION
                        /usr/local/openssl/lib/libssl.so)

set(LIBS
    winter
    dl
    pthread
    z
    yaml-cpp
    ${JSONCPP_LIBRARIES}
    # ${OPENSSL_LIBRARIES})
    lib-crypto
    lib-openssl)

# add_executable(test tests/test.cc)
# add_dependencies(test winter)
# force_redefine_file_macro_for_sources(test) #__FILE__
# target_link_libraries(test ${LIBS})

if(TEST)
winter_add_executable(test1 "tests/test.cc" winter "${LIBS}")
winter_add_executable(test_config "tests/test_config.cc" winter "${LIBS}")
winter_add_executable(test_thread "tests/test_thread.cc" winter "${LIBS}")
winter_add_executable(test_util "tests/test_util.cc" winter "${LIBS}")
winter_add_executable(test_fiber "tests/test_fiber.cc" winter "${LIBS}")
winter_add_executable(test_scheduler "tests/test_scheduler.cc" winter "${LIBS}")
winter_add_executable(test_iomanager "tests/test_iomanager.cc" winter "${LIBS}")
winter_add_executable(test_hook "tests/test_hook.cc" winter "${LIBS}")
winter_add_executable(test_address "tests/test_address.cc" winter "${LIBS}")
winter_add_executable(test_socket "tests/test_socket.cc" winter "${LIBS}")
winter_add_executable(test_bytearray "tests/test_bytearray.cc" winter "${LIBS}")
winter_add_executable(test_http "tests/test_http.cc" winter "${LIBS}")
winter_add_executable(test_http_parser "tests/test_http_parser.cc" winter "${LIBS}")
winter_add_executable(test_tcp_server "tests/test_tcp_server.cc" winter "${LIBS}")
winter_add_executable(echo_server "examples/echo_server.cc" winter "${LIBS}")
winter_add_executable(test_http_server "tests/test_http_server.cc" winter "${LIBS}")
winter_add_executable(test_uri "tests/test_uri.cc" winter "${LIBS}")
winter_add_executable(my_http_server "examples/my_http_server.cc" winter "${LIBS}")

winter_add_executable(test_daemon "tests/test_daemon.cc" winter "${LIBS}")
winter_add_executable(test_env "tests/test_env.cc" winter "${LIBS}")
winter_add_executable(test_ws_server "tests/test_ws_server.cc" winter "${LIBS}")
winter_add_executable(test_ws_client "tests/test_ws_client.cc" winter "${LIBS}")

winter_add_executable(test_application "tests/test_application.cc" winter "${LIBS}")

add_library(test SHARED tests/test_module.cc)
endif()

winter_add_executable(test_http_connection "tests/test_http_connection.cc" winter "${LIBS}")
winter_add_executable(test_lru "tests/test_lru.cc" winter "${LIBS}")
winter_add_executable(test_timed_cache "tests/test_timed_cache.cc" winter "${LIBS}")
winter_add_executable(test_timed_lru_cache "tests/test_timed_lru_cache.cc" winter "${LIBS}")
winter_add_executable(test_zlib_stream "tests/test_zlib_stream.cc" winter "${LIBS}")

winter_add_executable(bin_winter "winter/main.cc" winter "${LIBS}")
set_target_properties(bin_winter PROPERTIES OUTPUT_NAME "winter")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)