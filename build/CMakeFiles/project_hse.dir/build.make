# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.26

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Ahmed\CLionProjects\project_hse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Ahmed\CLionProjects\project_hse\build

# Include any dependencies generated for this target.
include CMakeFiles/project_hse.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/project_hse.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/project_hse.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/project_hse.dir/flags.make

CMakeFiles/project_hse.dir/main.cpp.obj: CMakeFiles/project_hse.dir/flags.make
CMakeFiles/project_hse.dir/main.cpp.obj: C:/Users/Ahmed/CLionProjects/project_hse/main.cpp
CMakeFiles/project_hse.dir/main.cpp.obj: CMakeFiles/project_hse.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Ahmed\CLionProjects\project_hse\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/project_hse.dir/main.cpp.obj"
	C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/project_hse.dir/main.cpp.obj -MF CMakeFiles\project_hse.dir\main.cpp.obj.d -o CMakeFiles\project_hse.dir\main.cpp.obj -c C:\Users\Ahmed\CLionProjects\project_hse\main.cpp

CMakeFiles/project_hse.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/project_hse.dir/main.cpp.i"
	C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\Ahmed\CLionProjects\project_hse\main.cpp > CMakeFiles\project_hse.dir\main.cpp.i

CMakeFiles/project_hse.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/project_hse.dir/main.cpp.s"
	C:\mingw64\bin\c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\Ahmed\CLionProjects\project_hse\main.cpp -o CMakeFiles\project_hse.dir\main.cpp.s

# Object files for target project_hse
project_hse_OBJECTS = \
"CMakeFiles/project_hse.dir/main.cpp.obj"

# External object files for target project_hse
project_hse_EXTERNAL_OBJECTS =

project_hse.exe: CMakeFiles/project_hse.dir/main.cpp.obj
project_hse.exe: CMakeFiles/project_hse.dir/build.make
project_hse.exe: CMakeFiles/project_hse.dir/linkLibs.rsp
project_hse.exe: CMakeFiles/project_hse.dir/objects1.rsp
project_hse.exe: CMakeFiles/project_hse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Ahmed\CLionProjects\project_hse\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable project_hse.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\project_hse.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/project_hse.dir/build: project_hse.exe
.PHONY : CMakeFiles/project_hse.dir/build

CMakeFiles/project_hse.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\project_hse.dir\cmake_clean.cmake
.PHONY : CMakeFiles/project_hse.dir/clean

CMakeFiles/project_hse.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Ahmed\CLionProjects\project_hse C:\Users\Ahmed\CLionProjects\project_hse C:\Users\Ahmed\CLionProjects\project_hse\build C:\Users\Ahmed\CLionProjects\project_hse\build C:\Users\Ahmed\CLionProjects\project_hse\build\CMakeFiles\project_hse.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/project_hse.dir/depend

