# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/qmj/linux_net/webserver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/qmj/linux_net/webserver/build

# Include any dependencies generated for this target.
include http/CMakeFiles/http.dir/depend.make

# Include the progress variables for this target.
include http/CMakeFiles/http.dir/progress.make

# Include the compile flags for this target's objects.
include http/CMakeFiles/http.dir/flags.make

http/CMakeFiles/http.dir/http_conn.cpp.o: http/CMakeFiles/http.dir/flags.make
http/CMakeFiles/http.dir/http_conn.cpp.o: ../http/http_conn.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/qmj/linux_net/webserver/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object http/CMakeFiles/http.dir/http_conn.cpp.o"
	cd /home/qmj/linux_net/webserver/build/http && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/http.dir/http_conn.cpp.o -c /home/qmj/linux_net/webserver/http/http_conn.cpp

http/CMakeFiles/http.dir/http_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/http.dir/http_conn.cpp.i"
	cd /home/qmj/linux_net/webserver/build/http && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/qmj/linux_net/webserver/http/http_conn.cpp > CMakeFiles/http.dir/http_conn.cpp.i

http/CMakeFiles/http.dir/http_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/http.dir/http_conn.cpp.s"
	cd /home/qmj/linux_net/webserver/build/http && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/qmj/linux_net/webserver/http/http_conn.cpp -o CMakeFiles/http.dir/http_conn.cpp.s

http/CMakeFiles/http.dir/http_conn.cpp.o.requires:
.PHONY : http/CMakeFiles/http.dir/http_conn.cpp.o.requires

http/CMakeFiles/http.dir/http_conn.cpp.o.provides: http/CMakeFiles/http.dir/http_conn.cpp.o.requires
	$(MAKE) -f http/CMakeFiles/http.dir/build.make http/CMakeFiles/http.dir/http_conn.cpp.o.provides.build
.PHONY : http/CMakeFiles/http.dir/http_conn.cpp.o.provides

http/CMakeFiles/http.dir/http_conn.cpp.o.provides.build: http/CMakeFiles/http.dir/http_conn.cpp.o

# Object files for target http
http_OBJECTS = \
"CMakeFiles/http.dir/http_conn.cpp.o"

# External object files for target http
http_EXTERNAL_OBJECTS =

http/libhttp.a: http/CMakeFiles/http.dir/http_conn.cpp.o
http/libhttp.a: http/CMakeFiles/http.dir/build.make
http/libhttp.a: http/CMakeFiles/http.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libhttp.a"
	cd /home/qmj/linux_net/webserver/build/http && $(CMAKE_COMMAND) -P CMakeFiles/http.dir/cmake_clean_target.cmake
	cd /home/qmj/linux_net/webserver/build/http && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/http.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
http/CMakeFiles/http.dir/build: http/libhttp.a
.PHONY : http/CMakeFiles/http.dir/build

http/CMakeFiles/http.dir/requires: http/CMakeFiles/http.dir/http_conn.cpp.o.requires
.PHONY : http/CMakeFiles/http.dir/requires

http/CMakeFiles/http.dir/clean:
	cd /home/qmj/linux_net/webserver/build/http && $(CMAKE_COMMAND) -P CMakeFiles/http.dir/cmake_clean.cmake
.PHONY : http/CMakeFiles/http.dir/clean

http/CMakeFiles/http.dir/depend:
	cd /home/qmj/linux_net/webserver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/qmj/linux_net/webserver /home/qmj/linux_net/webserver/http /home/qmj/linux_net/webserver/build /home/qmj/linux_net/webserver/build/http /home/qmj/linux_net/webserver/build/http/CMakeFiles/http.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : http/CMakeFiles/http.dir/depend

