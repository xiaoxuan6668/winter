# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Produce verbose output by default.
VERBOSE = 1

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ghx/MyProjects/cpp/winter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ghx/MyProjects/cpp/winter/build

# Include any dependencies generated for this target.
include CMakeFiles/test_config.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/test_config.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/test_config.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/test_config.dir/flags.make

CMakeFiles/test_config.dir/tests/test_config.cc.o: CMakeFiles/test_config.dir/flags.make
CMakeFiles/test_config.dir/tests/test_config.cc.o: ../tests/test_config.cc
CMakeFiles/test_config.dir/tests/test_config.cc.o: CMakeFiles/test_config.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ghx/MyProjects/cpp/winter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/test_config.dir/tests/test_config.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_config.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/test_config.dir/tests/test_config.cc.o -MF CMakeFiles/test_config.dir/tests/test_config.cc.o.d -o CMakeFiles/test_config.dir/tests/test_config.cc.o -c /home/ghx/MyProjects/cpp/winter/tests/test_config.cc

CMakeFiles/test_config.dir/tests/test_config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_config.dir/tests/test_config.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_config.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ghx/MyProjects/cpp/winter/tests/test_config.cc > CMakeFiles/test_config.dir/tests/test_config.cc.i

CMakeFiles/test_config.dir/tests/test_config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_config.dir/tests/test_config.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) -D__FILE__=\"tests/test_config.cc\" $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ghx/MyProjects/cpp/winter/tests/test_config.cc -o CMakeFiles/test_config.dir/tests/test_config.cc.s

# Object files for target test_config
test_config_OBJECTS = \
"CMakeFiles/test_config.dir/tests/test_config.cc.o"

# External object files for target test_config
test_config_EXTERNAL_OBJECTS =

../bin/test_config: CMakeFiles/test_config.dir/tests/test_config.cc.o
../bin/test_config: CMakeFiles/test_config.dir/build.make
../bin/test_config: ../lib/libwinter.so
../bin/test_config: CMakeFiles/test_config.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ghx/MyProjects/cpp/winter/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_config"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_config.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/test_config.dir/build: ../bin/test_config
.PHONY : CMakeFiles/test_config.dir/build

CMakeFiles/test_config.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/test_config.dir/cmake_clean.cmake
.PHONY : CMakeFiles/test_config.dir/clean

CMakeFiles/test_config.dir/depend:
	cd /home/ghx/MyProjects/cpp/winter/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ghx/MyProjects/cpp/winter /home/ghx/MyProjects/cpp/winter /home/ghx/MyProjects/cpp/winter/build /home/ghx/MyProjects/cpp/winter/build /home/ghx/MyProjects/cpp/winter/build/CMakeFiles/test_config.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/test_config.dir/depend

