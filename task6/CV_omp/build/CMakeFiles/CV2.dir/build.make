# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

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
CMAKE_COMMAND = "D:\APP program\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "D:\APP program\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\biancheng\code\ASC_EX\task6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\biancheng\code\ASC_EX\task6\build

# Include any dependencies generated for this target.
include CMakeFiles/CV2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/CV2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/CV2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CV2.dir/flags.make

CMakeFiles/CV2.dir/CV2.cpp.obj: CMakeFiles/CV2.dir/flags.make
CMakeFiles/CV2.dir/CV2.cpp.obj: D:/biancheng/code/ASC_EX/task6/CV2.cpp
CMakeFiles/CV2.dir/CV2.cpp.obj: CMakeFiles/CV2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\biancheng\code\ASC_EX\task6\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CV2.dir/CV2.cpp.obj"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/CV2.dir/CV2.cpp.obj -MF CMakeFiles\CV2.dir\CV2.cpp.obj.d -o CMakeFiles\CV2.dir\CV2.cpp.obj -c D:\biancheng\code\ASC_EX\task6\CV2.cpp

CMakeFiles/CV2.dir/CV2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/CV2.dir/CV2.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\biancheng\code\ASC_EX\task6\CV2.cpp > CMakeFiles\CV2.dir\CV2.cpp.i

CMakeFiles/CV2.dir/CV2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/CV2.dir/CV2.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\biancheng\code\ASC_EX\task6\CV2.cpp -o CMakeFiles\CV2.dir\CV2.cpp.s

# Object files for target CV2
CV2_OBJECTS = \
"CMakeFiles/CV2.dir/CV2.cpp.obj"

# External object files for target CV2
CV2_EXTERNAL_OBJECTS =

CV2.exe: CMakeFiles/CV2.dir/CV2.cpp.obj
CV2.exe: CMakeFiles/CV2.dir/build.make
CV2.exe: CMakeFiles/CV2.dir/linkLibs.rsp
CV2.exe: CMakeFiles/CV2.dir/objects1.rsp
CV2.exe: CMakeFiles/CV2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\biancheng\code\ASC_EX\task6\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CV2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\CV2.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CV2.dir/build: CV2.exe
.PHONY : CMakeFiles/CV2.dir/build

CMakeFiles/CV2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\CV2.dir\cmake_clean.cmake
.PHONY : CMakeFiles/CV2.dir/clean

CMakeFiles/CV2.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\biancheng\code\ASC_EX\task6 D:\biancheng\code\ASC_EX\task6 D:\biancheng\code\ASC_EX\task6\build D:\biancheng\code\ASC_EX\task6\build D:\biancheng\code\ASC_EX\task6\build\CMakeFiles\CV2.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/CV2.dir/depend

