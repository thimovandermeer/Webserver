# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jbennink/Documents/Projects/webserv

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jbennink/Documents/Projects/webserv/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/webserv.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/webserv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/webserv.dir/flags.make

CMakeFiles/webserv.dir/srcs/Response/Response.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Response/Response.cpp.o: ../srcs/Response/Response.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/webserv.dir/srcs/Response/Response.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Response/Response.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Response/Response.cpp

CMakeFiles/webserv.dir/srcs/Response/Response.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Response/Response.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Response/Response.cpp > CMakeFiles/webserv.dir/srcs/Response/Response.cpp.i

CMakeFiles/webserv.dir/srcs/Response/Response.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Response/Response.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Response/Response.cpp -o CMakeFiles/webserv.dir/srcs/Response/Response.cpp.s

CMakeFiles/webserv.dir/srcs/Request/request.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Request/request.cpp.o: ../srcs/Request/request.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/webserv.dir/srcs/Request/request.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Request/request.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Request/request.cpp

CMakeFiles/webserv.dir/srcs/Request/request.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Request/request.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Request/request.cpp > CMakeFiles/webserv.dir/srcs/Request/request.cpp.i

CMakeFiles/webserv.dir/srcs/Request/request.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Request/request.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Request/request.cpp -o CMakeFiles/webserv.dir/srcs/Request/request.cpp.s

CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.o: ../srcs/Response/ResponseHeader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Response/ResponseHeader.cpp

CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Response/ResponseHeader.cpp > CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.i

CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Response/ResponseHeader.cpp -o CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.s

CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.o: ../srcs/Utils/utils.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Utils/utils.cpp

CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Utils/utils.cpp > CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.i

CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Utils/utils.cpp -o CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.s

CMakeFiles/webserv.dir/srcs/Server/location.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Server/location.cpp.o: ../srcs/Server/location.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/webserv.dir/srcs/Server/location.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Server/location.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Server/location.cpp

CMakeFiles/webserv.dir/srcs/Server/location.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Server/location.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Server/location.cpp > CMakeFiles/webserv.dir/srcs/Server/location.cpp.i

CMakeFiles/webserv.dir/srcs/Server/location.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Server/location.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Server/location.cpp -o CMakeFiles/webserv.dir/srcs/Server/location.cpp.s

CMakeFiles/webserv.dir/srcs/Server/server.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Server/server.cpp.o: ../srcs/Server/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/webserv.dir/srcs/Server/server.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Server/server.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Server/server.cpp

CMakeFiles/webserv.dir/srcs/Server/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Server/server.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Server/server.cpp > CMakeFiles/webserv.dir/srcs/Server/server.cpp.i

CMakeFiles/webserv.dir/srcs/Server/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Server/server.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Server/server.cpp -o CMakeFiles/webserv.dir/srcs/Server/server.cpp.s

CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.o: ../srcs/Cluster/serverCluster.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Cluster/serverCluster.cpp

CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Cluster/serverCluster.cpp > CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.i

CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Cluster/serverCluster.cpp -o CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.s

CMakeFiles/webserv.dir/srcs/Server/parser.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/Server/parser.cpp.o: ../srcs/Server/parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/webserv.dir/srcs/Server/parser.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/Server/parser.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/Server/parser.cpp

CMakeFiles/webserv.dir/srcs/Server/parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/Server/parser.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/Server/parser.cpp > CMakeFiles/webserv.dir/srcs/Server/parser.cpp.i

CMakeFiles/webserv.dir/srcs/Server/parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/Server/parser.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/Server/parser.cpp -o CMakeFiles/webserv.dir/srcs/Server/parser.cpp.s

CMakeFiles/webserv.dir/srcs/webserv.cpp.o: CMakeFiles/webserv.dir/flags.make
CMakeFiles/webserv.dir/srcs/webserv.cpp.o: ../srcs/webserv.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/webserv.dir/srcs/webserv.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/webserv.dir/srcs/webserv.cpp.o -c /Users/jbennink/Documents/Projects/webserv/srcs/webserv.cpp

CMakeFiles/webserv.dir/srcs/webserv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/webserv.dir/srcs/webserv.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jbennink/Documents/Projects/webserv/srcs/webserv.cpp > CMakeFiles/webserv.dir/srcs/webserv.cpp.i

CMakeFiles/webserv.dir/srcs/webserv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/webserv.dir/srcs/webserv.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jbennink/Documents/Projects/webserv/srcs/webserv.cpp -o CMakeFiles/webserv.dir/srcs/webserv.cpp.s

# Object files for target webserv
webserv_OBJECTS = \
"CMakeFiles/webserv.dir/srcs/Response/Response.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Request/request.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Server/location.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Server/server.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.o" \
"CMakeFiles/webserv.dir/srcs/Server/parser.cpp.o" \
"CMakeFiles/webserv.dir/srcs/webserv.cpp.o"

# External object files for target webserv
webserv_EXTERNAL_OBJECTS =

webserv: CMakeFiles/webserv.dir/srcs/Response/Response.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Request/request.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Response/ResponseHeader.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Utils/utils.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Server/location.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Server/server.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Cluster/serverCluster.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/Server/parser.cpp.o
webserv: CMakeFiles/webserv.dir/srcs/webserv.cpp.o
webserv: CMakeFiles/webserv.dir/build.make
webserv: CMakeFiles/webserv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable webserv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/webserv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/webserv.dir/build: webserv

.PHONY : CMakeFiles/webserv.dir/build

CMakeFiles/webserv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/webserv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/webserv.dir/clean

CMakeFiles/webserv.dir/depend:
	cd /Users/jbennink/Documents/Projects/webserv/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jbennink/Documents/Projects/webserv /Users/jbennink/Documents/Projects/webserv /Users/jbennink/Documents/Projects/webserv/cmake-build-debug /Users/jbennink/Documents/Projects/webserv/cmake-build-debug /Users/jbennink/Documents/Projects/webserv/cmake-build-debug/CMakeFiles/webserv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/webserv.dir/depend

