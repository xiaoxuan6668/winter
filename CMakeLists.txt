cmake_minimum_required(VERSION 3.22)
project(winter)

# makefile has bug
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")
# -Wno-builtin-macro-redefined

include_directories(.)
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

# find_library(YAMLCPP yaml-cpp)
# message("***", ${YAMLCPP})

set(LIB_SRC
    winter/address.cc
    winter/log.cc
    winter/util.cc
    winter/config.cc
    winter/fd_manager.cc
    winter/fiber.cc
    winter/hook.cc
    winter/scheduler.cc
    winter/iomanager.cc
    winter/thread.cc
    winter/timer.cc
    )

add_library(winter SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(winter) #__FILE__
#add_library(winter_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(winter_static PROPERTIES OUTPUT_NAME "winter")

set(LIBS
    winter
    pthread
    dl
    yaml-cpp)

add_executable(test tests/test.cc)
add_dependencies(test winter)
force_redefine_file_macro_for_sources(test) #__FILE__
target_link_libraries(test ${LIBS})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config winter)
force_redefine_file_macro_for_sources(test_config) #__FILE__
target_link_libraries(test_config ${LIBS})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread winter)
force_redefine_file_macro_for_sources(test_thread) #__FILE__
target_link_libraries(test_thread ${LIBS})

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util winter)
force_redefine_file_macro_for_sources(test_util) #__FILE__
target_link_libraries(test_util ${LIBS})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber winter)
force_redefine_file_macro_for_sources(test_fiber) #__FILE__
target_link_libraries(test_fiber ${LIBS})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler winter)
force_redefine_file_macro_for_sources(test_scheduler) #__FILE__
target_link_libraries(test_scheduler ${LIBS})

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager winter)
force_redefine_file_macro_for_sources(test_iomanager) #__FILE__
target_link_libraries(test_iomanager ${LIBS})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook winter)
force_redefine_file_macro_for_sources(test_hook) #__FILE__
target_link_libraries(test_hook ${LIBS})

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address winter)
force_redefine_file_macro_for_sources(test_address) #__FILE__
target_link_libraries(test_address ${LIBS})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)