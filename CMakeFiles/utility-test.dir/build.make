# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.6.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.6.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/volodja/workspace/qtcreator/utility

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/volodja/workspace/qtcreator/utility

# Include any dependencies generated for this target.
include CMakeFiles/utility-test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/utility-test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/utility-test.dir/flags.make

CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o: CMakeFiles/utility-test.dir/flags.make
CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o: utility-test/source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/volodja/workspace/qtcreator/utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o -c /Users/volodja/workspace/qtcreator/utility/utility-test/source/main.cpp

CMakeFiles/utility-test.dir/utility-test/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utility-test.dir/utility-test/source/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/volodja/workspace/qtcreator/utility/utility-test/source/main.cpp > CMakeFiles/utility-test.dir/utility-test/source/main.cpp.i

CMakeFiles/utility-test.dir/utility-test/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utility-test.dir/utility-test/source/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/volodja/workspace/qtcreator/utility/utility-test/source/main.cpp -o CMakeFiles/utility-test.dir/utility-test/source/main.cpp.s

CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.requires:

.PHONY : CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.requires

CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.provides: CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/utility-test.dir/build.make CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.provides.build
.PHONY : CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.provides

CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.provides.build: CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o


CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o: CMakeFiles/utility-test.dir/flags.make
CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o: utility-test/source/utility/networking/server/socket/socket-test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/volodja/workspace/qtcreator/utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o -c /Users/volodja/workspace/qtcreator/utility/utility-test/source/utility/networking/server/socket/socket-test.cpp

CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/volodja/workspace/qtcreator/utility/utility-test/source/utility/networking/server/socket/socket-test.cpp > CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.i

CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/volodja/workspace/qtcreator/utility/utility-test/source/utility/networking/server/socket/socket-test.cpp -o CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.s

CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.requires:

.PHONY : CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.requires

CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.provides: CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.requires
	$(MAKE) -f CMakeFiles/utility-test.dir/build.make CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.provides.build
.PHONY : CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.provides

CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.provides.build: CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o


CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o: CMakeFiles/utility-test.dir/flags.make
CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o: utility/source/utility/networking/server/socket/implementation/socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/volodja/workspace/qtcreator/utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o -c /Users/volodja/workspace/qtcreator/utility/utility/source/utility/networking/server/socket/implementation/socket.cpp

CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/volodja/workspace/qtcreator/utility/utility/source/utility/networking/server/socket/implementation/socket.cpp > CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.i

CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/volodja/workspace/qtcreator/utility/utility/source/utility/networking/server/socket/implementation/socket.cpp -o CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.s

CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.requires:

.PHONY : CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.requires

CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.provides: CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/utility-test.dir/build.make CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.provides.build
.PHONY : CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.provides

CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.provides.build: CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o


# Object files for target utility-test
utility__test_OBJECTS = \
"CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o" \
"CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o" \
"CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o"

# External object files for target utility-test
utility__test_EXTERNAL_OBJECTS =

bin/utility-test: CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o
bin/utility-test: CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o
bin/utility-test: CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o
bin/utility-test: CMakeFiles/utility-test.dir/build.make
bin/utility-test: /usr/local/lib/libgtest.a
bin/utility-test: /usr/local/lib/libgtest_main.a
bin/utility-test: CMakeFiles/utility-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/volodja/workspace/qtcreator/utility/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable bin/utility-test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utility-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/utility-test.dir/build: bin/utility-test

.PHONY : CMakeFiles/utility-test.dir/build

CMakeFiles/utility-test.dir/requires: CMakeFiles/utility-test.dir/utility-test/source/main.cpp.o.requires
CMakeFiles/utility-test.dir/requires: CMakeFiles/utility-test.dir/utility-test/source/utility/networking/server/socket/socket-test.cpp.o.requires
CMakeFiles/utility-test.dir/requires: CMakeFiles/utility-test.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o.requires

.PHONY : CMakeFiles/utility-test.dir/requires

CMakeFiles/utility-test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/utility-test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/utility-test.dir/clean

CMakeFiles/utility-test.dir/depend:
	cd /Users/volodja/workspace/qtcreator/utility && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/volodja/workspace/qtcreator/utility /Users/volodja/workspace/qtcreator/utility /Users/volodja/workspace/qtcreator/utility /Users/volodja/workspace/qtcreator/utility /Users/volodja/workspace/qtcreator/utility/CMakeFiles/utility-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/utility-test.dir/depend

