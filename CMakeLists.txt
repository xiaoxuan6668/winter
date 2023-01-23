cmake_minimum_required(VERSION 3.22)
project(winter)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

include_directories(.)
# include_directories(/usr/local/include)
# link_directories(/usr/local/lib)

# find_library(YAMLCPP yaml-cpp)
# message("***", ${YAMLCPP})

set(LIB_SRC
    winter/log.cc
    winter/util.cc
    winter/config.cc
    )

add_library(winter SHARED ${LIB_SRC})
#add_library(winter_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(winter_static PROPERTIES OUTPUT_NAME "winter")

add_executable(test tests/test.cc)
add_dependencies(test winter)
target_link_libraries(test winter yaml-cpp)

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config winter)
target_link_libraries(test_config winter yaml-cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)