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
CMAKE_SOURCE_DIR = /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build

# Include any dependencies generated for this target.
include CMakeFiles/03_manage_recurring_transactions.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/03_manage_recurring_transactions.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/03_manage_recurring_transactions.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/03_manage_recurring_transactions.dir/flags.make

CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o: CMakeFiles/03_manage_recurring_transactions.dir/flags.make
CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o: ../src/manage_recurring_transactions.cpp
CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o: CMakeFiles/03_manage_recurring_transactions.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o -MF CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o.d -o CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o -c /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/src/manage_recurring_transactions.cpp

CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/src/manage_recurring_transactions.cpp > CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.i

CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/src/manage_recurring_transactions.cpp -o CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.s

# Object files for target 03_manage_recurring_transactions
03_manage_recurring_transactions_OBJECTS = \
"CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o"

# External object files for target 03_manage_recurring_transactions
03_manage_recurring_transactions_EXTERNAL_OBJECTS =

manage_recurring_transactions: CMakeFiles/03_manage_recurring_transactions.dir/src/manage_recurring_transactions.cpp.o
manage_recurring_transactions: CMakeFiles/03_manage_recurring_transactions.dir/build.make
manage_recurring_transactions: CMakeFiles/03_manage_recurring_transactions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable manage_recurring_transactions"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/03_manage_recurring_transactions.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -E copy /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build/manage_recurring_transactions /home/fabian51/Project/Vinager/exe

# Rule to build all files generated by this target.
CMakeFiles/03_manage_recurring_transactions.dir/build: manage_recurring_transactions
.PHONY : CMakeFiles/03_manage_recurring_transactions.dir/build

CMakeFiles/03_manage_recurring_transactions.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/03_manage_recurring_transactions.dir/cmake_clean.cmake
.PHONY : CMakeFiles/03_manage_recurring_transactions.dir/clean

CMakeFiles/03_manage_recurring_transactions.dir/depend:
	cd /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build /home/fabian51/Project/Vinager/module/00_initialize/03_manage_recurring_transactions/build/CMakeFiles/03_manage_recurring_transactions.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/03_manage_recurring_transactions.dir/depend
