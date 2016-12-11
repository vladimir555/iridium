# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.7.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.7.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aa/workspace/qtcreator/utility

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aa/workspace/qtcreator/utility

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/Cellar/cmake/3.7.0/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/Cellar/cmake/3.7.0/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target test
test:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running tests..."
	/usr/local/Cellar/cmake/3.7.0/bin/ctest --force-new-ctest-process $(ARGS)
.PHONY : test

# Special rule for the target test
test/fast: test

.PHONY : test/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/aa/workspace/qtcreator/utility/CMakeFiles /Users/aa/workspace/qtcreator/utility/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/aa/workspace/qtcreator/utility/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named utility_test

# Build rule for target.
utility_test: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 utility_test
.PHONY : utility_test

# fast build rule for target.
utility_test/fast:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/build
.PHONY : utility_test/fast

#=============================================================================
# Target rules for targets named utility

# Build rule for target.
utility: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 utility
.PHONY : utility

# fast build rule for target.
utility/fast:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/build
.PHONY : utility/fast

utility/source/utility/convertion/implementation/convert.o: utility/source/utility/convertion/implementation/convert.cpp.o

.PHONY : utility/source/utility/convertion/implementation/convert.o

# target to build an object file
utility/source/utility/convertion/implementation/convert.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/convertion/implementation/convert.cpp.o
.PHONY : utility/source/utility/convertion/implementation/convert.cpp.o

utility/source/utility/convertion/implementation/convert.i: utility/source/utility/convertion/implementation/convert.cpp.i

.PHONY : utility/source/utility/convertion/implementation/convert.i

# target to preprocess a source file
utility/source/utility/convertion/implementation/convert.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/convertion/implementation/convert.cpp.i
.PHONY : utility/source/utility/convertion/implementation/convert.cpp.i

utility/source/utility/convertion/implementation/convert.s: utility/source/utility/convertion/implementation/convert.cpp.s

.PHONY : utility/source/utility/convertion/implementation/convert.s

# target to generate assembly for a file
utility/source/utility/convertion/implementation/convert.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/convertion/implementation/convert.cpp.s
.PHONY : utility/source/utility/convertion/implementation/convert.cpp.s

utility/source/utility/networking/address.o: utility/source/utility/networking/address.cpp.o

.PHONY : utility/source/utility/networking/address.o

# target to build an object file
utility/source/utility/networking/address.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/networking/address.cpp.o
.PHONY : utility/source/utility/networking/address.cpp.o

utility/source/utility/networking/address.i: utility/source/utility/networking/address.cpp.i

.PHONY : utility/source/utility/networking/address.i

# target to preprocess a source file
utility/source/utility/networking/address.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/networking/address.cpp.i
.PHONY : utility/source/utility/networking/address.cpp.i

utility/source/utility/networking/address.s: utility/source/utility/networking/address.cpp.s

.PHONY : utility/source/utility/networking/address.s

# target to generate assembly for a file
utility/source/utility/networking/address.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/networking/address.cpp.s
.PHONY : utility/source/utility/networking/address.cpp.s

utility/source/utility/networking/server/socket/implementation/socket.o: utility/source/utility/networking/server/socket/implementation/socket.cpp.o

.PHONY : utility/source/utility/networking/server/socket/implementation/socket.o

# target to build an object file
utility/source/utility/networking/server/socket/implementation/socket.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.o
.PHONY : utility/source/utility/networking/server/socket/implementation/socket.cpp.o

utility/source/utility/networking/server/socket/implementation/socket.i: utility/source/utility/networking/server/socket/implementation/socket.cpp.i

.PHONY : utility/source/utility/networking/server/socket/implementation/socket.i

# target to preprocess a source file
utility/source/utility/networking/server/socket/implementation/socket.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.i
.PHONY : utility/source/utility/networking/server/socket/implementation/socket.cpp.i

utility/source/utility/networking/server/socket/implementation/socket.s: utility/source/utility/networking/server/socket/implementation/socket.cpp.s

.PHONY : utility/source/utility/networking/server/socket/implementation/socket.s

# target to generate assembly for a file
utility/source/utility/networking/server/socket/implementation/socket.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/networking/server/socket/implementation/socket.cpp.s
.PHONY : utility/source/utility/networking/server/socket/implementation/socket.cpp.s

utility/source/utility/pasring/implementation/node_.o: utility/source/utility/pasring/implementation/node_.cpp.o

.PHONY : utility/source/utility/pasring/implementation/node_.o

# target to build an object file
utility/source/utility/pasring/implementation/node_.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/pasring/implementation/node_.cpp.o
.PHONY : utility/source/utility/pasring/implementation/node_.cpp.o

utility/source/utility/pasring/implementation/node_.i: utility/source/utility/pasring/implementation/node_.cpp.i

.PHONY : utility/source/utility/pasring/implementation/node_.i

# target to preprocess a source file
utility/source/utility/pasring/implementation/node_.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/pasring/implementation/node_.cpp.i
.PHONY : utility/source/utility/pasring/implementation/node_.cpp.i

utility/source/utility/pasring/implementation/node_.s: utility/source/utility/pasring/implementation/node_.cpp.s

.PHONY : utility/source/utility/pasring/implementation/node_.s

# target to generate assembly for a file
utility/source/utility/pasring/implementation/node_.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/pasring/implementation/node_.cpp.s
.PHONY : utility/source/utility/pasring/implementation/node_.cpp.s

utility/source/utility/pasring/node.o: utility/source/utility/pasring/node.cpp.o

.PHONY : utility/source/utility/pasring/node.o

# target to build an object file
utility/source/utility/pasring/node.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/pasring/node.cpp.o
.PHONY : utility/source/utility/pasring/node.cpp.o

utility/source/utility/pasring/node.i: utility/source/utility/pasring/node.cpp.i

.PHONY : utility/source/utility/pasring/node.i

# target to preprocess a source file
utility/source/utility/pasring/node.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/pasring/node.cpp.i
.PHONY : utility/source/utility/pasring/node.cpp.i

utility/source/utility/pasring/node.s: utility/source/utility/pasring/node.cpp.s

.PHONY : utility/source/utility/pasring/node.s

# target to generate assembly for a file
utility/source/utility/pasring/node.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/pasring/node.cpp.s
.PHONY : utility/source/utility/pasring/node.cpp.s

utility/source/utility/system/terminator.o: utility/source/utility/system/terminator.cpp.o

.PHONY : utility/source/utility/system/terminator.o

# target to build an object file
utility/source/utility/system/terminator.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/system/terminator.cpp.o
.PHONY : utility/source/utility/system/terminator.cpp.o

utility/source/utility/system/terminator.i: utility/source/utility/system/terminator.cpp.i

.PHONY : utility/source/utility/system/terminator.i

# target to preprocess a source file
utility/source/utility/system/terminator.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/system/terminator.cpp.i
.PHONY : utility/source/utility/system/terminator.cpp.i

utility/source/utility/system/terminator.s: utility/source/utility/system/terminator.cpp.s

.PHONY : utility/source/utility/system/terminator.s

# target to generate assembly for a file
utility/source/utility/system/terminator.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/system/terminator.cpp.s
.PHONY : utility/source/utility/system/terminator.cpp.s

utility/source/utility/threading/implementation/condition.o: utility/source/utility/threading/implementation/condition.cpp.o

.PHONY : utility/source/utility/threading/implementation/condition.o

# target to build an object file
utility/source/utility/threading/implementation/condition.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/condition.cpp.o
.PHONY : utility/source/utility/threading/implementation/condition.cpp.o

utility/source/utility/threading/implementation/condition.i: utility/source/utility/threading/implementation/condition.cpp.i

.PHONY : utility/source/utility/threading/implementation/condition.i

# target to preprocess a source file
utility/source/utility/threading/implementation/condition.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/condition.cpp.i
.PHONY : utility/source/utility/threading/implementation/condition.cpp.i

utility/source/utility/threading/implementation/condition.s: utility/source/utility/threading/implementation/condition.cpp.s

.PHONY : utility/source/utility/threading/implementation/condition.s

# target to generate assembly for a file
utility/source/utility/threading/implementation/condition.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/condition.cpp.s
.PHONY : utility/source/utility/threading/implementation/condition.cpp.s

utility/source/utility/threading/implementation/mutex.o: utility/source/utility/threading/implementation/mutex.cpp.o

.PHONY : utility/source/utility/threading/implementation/mutex.o

# target to build an object file
utility/source/utility/threading/implementation/mutex.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/mutex.cpp.o
.PHONY : utility/source/utility/threading/implementation/mutex.cpp.o

utility/source/utility/threading/implementation/mutex.i: utility/source/utility/threading/implementation/mutex.cpp.i

.PHONY : utility/source/utility/threading/implementation/mutex.i

# target to preprocess a source file
utility/source/utility/threading/implementation/mutex.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/mutex.cpp.i
.PHONY : utility/source/utility/threading/implementation/mutex.cpp.i

utility/source/utility/threading/implementation/mutex.s: utility/source/utility/threading/implementation/mutex.cpp.s

.PHONY : utility/source/utility/threading/implementation/mutex.s

# target to generate assembly for a file
utility/source/utility/threading/implementation/mutex.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/mutex.cpp.s
.PHONY : utility/source/utility/threading/implementation/mutex.cpp.s

utility/source/utility/threading/implementation/timed_mutex.o: utility/source/utility/threading/implementation/timed_mutex.cpp.o

.PHONY : utility/source/utility/threading/implementation/timed_mutex.o

# target to build an object file
utility/source/utility/threading/implementation/timed_mutex.cpp.o:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/timed_mutex.cpp.o
.PHONY : utility/source/utility/threading/implementation/timed_mutex.cpp.o

utility/source/utility/threading/implementation/timed_mutex.i: utility/source/utility/threading/implementation/timed_mutex.cpp.i

.PHONY : utility/source/utility/threading/implementation/timed_mutex.i

# target to preprocess a source file
utility/source/utility/threading/implementation/timed_mutex.cpp.i:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/timed_mutex.cpp.i
.PHONY : utility/source/utility/threading/implementation/timed_mutex.cpp.i

utility/source/utility/threading/implementation/timed_mutex.s: utility/source/utility/threading/implementation/timed_mutex.cpp.s

.PHONY : utility/source/utility/threading/implementation/timed_mutex.s

# target to generate assembly for a file
utility/source/utility/threading/implementation/timed_mutex.cpp.s:
	$(MAKE) -f CMakeFiles/utility.dir/build.make CMakeFiles/utility.dir/utility/source/utility/threading/implementation/timed_mutex.cpp.s
.PHONY : utility/source/utility/threading/implementation/timed_mutex.cpp.s

utility_test/source/utility/convertion/convert_test.o: utility_test/source/utility/convertion/convert_test.cpp.o

.PHONY : utility_test/source/utility/convertion/convert_test.o

# target to build an object file
utility_test/source/utility/convertion/convert_test.cpp.o:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/utility_test/source/utility/convertion/convert_test.cpp.o
.PHONY : utility_test/source/utility/convertion/convert_test.cpp.o

utility_test/source/utility/convertion/convert_test.i: utility_test/source/utility/convertion/convert_test.cpp.i

.PHONY : utility_test/source/utility/convertion/convert_test.i

# target to preprocess a source file
utility_test/source/utility/convertion/convert_test.cpp.i:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/utility_test/source/utility/convertion/convert_test.cpp.i
.PHONY : utility_test/source/utility/convertion/convert_test.cpp.i

utility_test/source/utility/convertion/convert_test.s: utility_test/source/utility/convertion/convert_test.cpp.s

.PHONY : utility_test/source/utility/convertion/convert_test.s

# target to generate assembly for a file
utility_test/source/utility/convertion/convert_test.cpp.s:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/utility_test/source/utility/convertion/convert_test.cpp.s
.PHONY : utility_test/source/utility/convertion/convert_test.cpp.s

utility_test/source/utility/networking/server/socket/socket-test.o: utility_test/source/utility/networking/server/socket/socket-test.cpp.o

.PHONY : utility_test/source/utility/networking/server/socket/socket-test.o

# target to build an object file
utility_test/source/utility/networking/server/socket/socket-test.cpp.o:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/utility_test/source/utility/networking/server/socket/socket-test.cpp.o
.PHONY : utility_test/source/utility/networking/server/socket/socket-test.cpp.o

utility_test/source/utility/networking/server/socket/socket-test.i: utility_test/source/utility/networking/server/socket/socket-test.cpp.i

.PHONY : utility_test/source/utility/networking/server/socket/socket-test.i

# target to preprocess a source file
utility_test/source/utility/networking/server/socket/socket-test.cpp.i:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/utility_test/source/utility/networking/server/socket/socket-test.cpp.i
.PHONY : utility_test/source/utility/networking/server/socket/socket-test.cpp.i

utility_test/source/utility/networking/server/socket/socket-test.s: utility_test/source/utility/networking/server/socket/socket-test.cpp.s

.PHONY : utility_test/source/utility/networking/server/socket/socket-test.s

# target to generate assembly for a file
utility_test/source/utility/networking/server/socket/socket-test.cpp.s:
	$(MAKE) -f CMakeFiles/utility_test.dir/build.make CMakeFiles/utility_test.dir/utility_test/source/utility/networking/server/socket/socket-test.cpp.s
.PHONY : utility_test/source/utility/networking/server/socket/socket-test.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... test"
	@echo "... utility_test"
	@echo "... utility"
	@echo "... utility/source/utility/convertion/implementation/convert.o"
	@echo "... utility/source/utility/convertion/implementation/convert.i"
	@echo "... utility/source/utility/convertion/implementation/convert.s"
	@echo "... utility/source/utility/networking/address.o"
	@echo "... utility/source/utility/networking/address.i"
	@echo "... utility/source/utility/networking/address.s"
	@echo "... utility/source/utility/networking/server/socket/implementation/socket.o"
	@echo "... utility/source/utility/networking/server/socket/implementation/socket.i"
	@echo "... utility/source/utility/networking/server/socket/implementation/socket.s"
	@echo "... utility/source/utility/pasring/implementation/node_.o"
	@echo "... utility/source/utility/pasring/implementation/node_.i"
	@echo "... utility/source/utility/pasring/implementation/node_.s"
	@echo "... utility/source/utility/pasring/node.o"
	@echo "... utility/source/utility/pasring/node.i"
	@echo "... utility/source/utility/pasring/node.s"
	@echo "... utility/source/utility/system/terminator.o"
	@echo "... utility/source/utility/system/terminator.i"
	@echo "... utility/source/utility/system/terminator.s"
	@echo "... utility/source/utility/threading/implementation/condition.o"
	@echo "... utility/source/utility/threading/implementation/condition.i"
	@echo "... utility/source/utility/threading/implementation/condition.s"
	@echo "... utility/source/utility/threading/implementation/mutex.o"
	@echo "... utility/source/utility/threading/implementation/mutex.i"
	@echo "... utility/source/utility/threading/implementation/mutex.s"
	@echo "... utility/source/utility/threading/implementation/timed_mutex.o"
	@echo "... utility/source/utility/threading/implementation/timed_mutex.i"
	@echo "... utility/source/utility/threading/implementation/timed_mutex.s"
	@echo "... utility_test/source/utility/convertion/convert_test.o"
	@echo "... utility_test/source/utility/convertion/convert_test.i"
	@echo "... utility_test/source/utility/convertion/convert_test.s"
	@echo "... utility_test/source/utility/networking/server/socket/socket-test.o"
	@echo "... utility_test/source/utility/networking/server/socket/socket-test.i"
	@echo "... utility_test/source/utility/networking/server/socket/socket-test.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

