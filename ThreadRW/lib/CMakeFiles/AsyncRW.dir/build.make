# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/edelwud/SPOVM/ThreadRW/lib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/edelwud/SPOVM/ThreadRW/lib

# Include any dependencies generated for this target.
include CMakeFiles/AsyncRW.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/AsyncRW.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/AsyncRW.dir/flags.make

CMakeFiles/AsyncRW.dir/src/main.cpp.o: CMakeFiles/AsyncRW.dir/flags.make
CMakeFiles/AsyncRW.dir/src/main.cpp.o: src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/edelwud/SPOVM/ThreadRW/lib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/AsyncRW.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/AsyncRW.dir/src/main.cpp.o -c /home/edelwud/SPOVM/ThreadRW/lib/src/main.cpp

CMakeFiles/AsyncRW.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AsyncRW.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/edelwud/SPOVM/ThreadRW/lib/src/main.cpp > CMakeFiles/AsyncRW.dir/src/main.cpp.i

CMakeFiles/AsyncRW.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AsyncRW.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/edelwud/SPOVM/ThreadRW/lib/src/main.cpp -o CMakeFiles/AsyncRW.dir/src/main.cpp.s

# Object files for target AsyncRW
AsyncRW_OBJECTS = \
"CMakeFiles/AsyncRW.dir/src/main.cpp.o"

# External object files for target AsyncRW
AsyncRW_EXTERNAL_OBJECTS =

libAsyncRW.so: CMakeFiles/AsyncRW.dir/src/main.cpp.o
libAsyncRW.so: CMakeFiles/AsyncRW.dir/build.make
libAsyncRW.so: CMakeFiles/AsyncRW.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/edelwud/SPOVM/ThreadRW/lib/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libAsyncRW.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AsyncRW.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/AsyncRW.dir/build: libAsyncRW.so

.PHONY : CMakeFiles/AsyncRW.dir/build

CMakeFiles/AsyncRW.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/AsyncRW.dir/cmake_clean.cmake
.PHONY : CMakeFiles/AsyncRW.dir/clean

CMakeFiles/AsyncRW.dir/depend:
	cd /home/edelwud/SPOVM/ThreadRW/lib && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/edelwud/SPOVM/ThreadRW/lib /home/edelwud/SPOVM/ThreadRW/lib /home/edelwud/SPOVM/ThreadRW/lib /home/edelwud/SPOVM/ThreadRW/lib /home/edelwud/SPOVM/ThreadRW/lib/CMakeFiles/AsyncRW.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/AsyncRW.dir/depend

