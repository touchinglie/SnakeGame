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
CMAKE_COMMAND = J:\cmake3.29.1\bin\cmake.exe

# The command to remove a file.
RM = J:\cmake3.29.1\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = G:\programing\c\course\special\5.Snake\packup

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = G:\programing\c\course\special\5.Snake\packup\build

# Include any dependencies generated for this target.
include CMakeFiles/SnakeGame.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/SnakeGame.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/SnakeGame.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SnakeGame.dir/flags.make

resources.c: G:/programing/c/course/special/5.Snake/packup/snakeGame.gresource.xml
resources.c: res/micon.ico
resources.c: res/skaterswaltz_8bit.mp3
resources.c: res/Rockman3Shortend.mp3
resources.c: res/ate_8bit.mp3
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=G:\programing\c\course\special\5.Snake\packup\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating resources.c"
	glib-compile-resources G:/programing/c/course/special/5.Snake/packup/snakeGame.gresource.xml --target=resources.c --generate-source --sourcedir=G:/programing/c/course/special/5.Snake/packup

CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj: CMakeFiles/SnakeGame.dir/flags.make
CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj: CMakeFiles/SnakeGame.dir/includes_C.rsp
CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj: G:/programing/c/course/special/5.Snake/packup/snakeGameWin.c
CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj: CMakeFiles/SnakeGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=G:\programing\c\course\special\5.Snake\packup\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj"
	E:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj -MF CMakeFiles\SnakeGame.dir\snakeGameWin.c.obj.d -o CMakeFiles\SnakeGame.dir\snakeGameWin.c.obj -c G:\programing\c\course\special\5.Snake\packup\snakeGameWin.c

CMakeFiles/SnakeGame.dir/snakeGameWin.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/SnakeGame.dir/snakeGameWin.c.i"
	E:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E G:\programing\c\course\special\5.Snake\packup\snakeGameWin.c > CMakeFiles\SnakeGame.dir\snakeGameWin.c.i

CMakeFiles/SnakeGame.dir/snakeGameWin.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/SnakeGame.dir/snakeGameWin.c.s"
	E:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S G:\programing\c\course\special\5.Snake\packup\snakeGameWin.c -o CMakeFiles\SnakeGame.dir\snakeGameWin.c.s

CMakeFiles/SnakeGame.dir/resources.c.obj: CMakeFiles/SnakeGame.dir/flags.make
CMakeFiles/SnakeGame.dir/resources.c.obj: CMakeFiles/SnakeGame.dir/includes_C.rsp
CMakeFiles/SnakeGame.dir/resources.c.obj: resources.c
CMakeFiles/SnakeGame.dir/resources.c.obj: CMakeFiles/SnakeGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=G:\programing\c\course\special\5.Snake\packup\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/SnakeGame.dir/resources.c.obj"
	E:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/SnakeGame.dir/resources.c.obj -MF CMakeFiles\SnakeGame.dir\resources.c.obj.d -o CMakeFiles\SnakeGame.dir\resources.c.obj -c G:\programing\c\course\special\5.Snake\packup\build\resources.c

CMakeFiles/SnakeGame.dir/resources.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/SnakeGame.dir/resources.c.i"
	E:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E G:\programing\c\course\special\5.Snake\packup\build\resources.c > CMakeFiles\SnakeGame.dir\resources.c.i

CMakeFiles/SnakeGame.dir/resources.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/SnakeGame.dir/resources.c.s"
	E:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S G:\programing\c\course\special\5.Snake\packup\build\resources.c -o CMakeFiles\SnakeGame.dir\resources.c.s

# Object files for target SnakeGame
SnakeGame_OBJECTS = \
"CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj" \
"CMakeFiles/SnakeGame.dir/resources.c.obj"

# External object files for target SnakeGame
SnakeGame_EXTERNAL_OBJECTS =

SnakeGame.exe: CMakeFiles/SnakeGame.dir/snakeGameWin.c.obj
SnakeGame.exe: CMakeFiles/SnakeGame.dir/resources.c.obj
SnakeGame.exe: CMakeFiles/SnakeGame.dir/build.make
SnakeGame.exe: CMakeFiles/SnakeGame.dir/linkLibs.rsp
SnakeGame.exe: CMakeFiles/SnakeGame.dir/objects1.rsp
SnakeGame.exe: CMakeFiles/SnakeGame.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=G:\programing\c\course\special\5.Snake\packup\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable SnakeGame.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SnakeGame.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SnakeGame.dir/build: SnakeGame.exe
.PHONY : CMakeFiles/SnakeGame.dir/build

CMakeFiles/SnakeGame.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SnakeGame.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SnakeGame.dir/clean

CMakeFiles/SnakeGame.dir/depend: resources.c
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" G:\programing\c\course\special\5.Snake\packup G:\programing\c\course\special\5.Snake\packup G:\programing\c\course\special\5.Snake\packup\build G:\programing\c\course\special\5.Snake\packup\build G:\programing\c\course\special\5.Snake\packup\build\CMakeFiles\SnakeGame.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/SnakeGame.dir/depend

