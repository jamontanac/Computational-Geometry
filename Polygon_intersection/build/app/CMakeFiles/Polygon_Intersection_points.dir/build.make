# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build"

# Include any dependencies generated for this target.
include app/CMakeFiles/Polygon_Intersection_points.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include app/CMakeFiles/Polygon_Intersection_points.dir/compiler_depend.make

# Include the progress variables for this target.
include app/CMakeFiles/Polygon_Intersection_points.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/Polygon_Intersection_points.dir/flags.make

app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o: app/CMakeFiles/Polygon_Intersection_points.dir/flags.make
app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o: /Users/jamontanac/Dropbox/Geometria\ computacional/Polygon_intersection/app/Polygon_Intersection_points.cxx
app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o: app/CMakeFiles/Polygon_Intersection_points.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o"
	cd "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o -MF CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o.d -o CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o -c "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/app/Polygon_Intersection_points.cxx"

app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.i"
	cd "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/app/Polygon_Intersection_points.cxx" > CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.i

app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.s"
	cd "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app" && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/app/Polygon_Intersection_points.cxx" -o CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.s

# Object files for target Polygon_Intersection_points
Polygon_Intersection_points_OBJECTS = \
"CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o"

# External object files for target Polygon_Intersection_points
Polygon_Intersection_points_EXTERNAL_OBJECTS =

Polygon_Intersection_points: app/CMakeFiles/Polygon_Intersection_points.dir/Polygon_Intersection_points.cxx.o
Polygon_Intersection_points: app/CMakeFiles/Polygon_Intersection_points.dir/build.make
Polygon_Intersection_points: app/CMakeFiles/Polygon_Intersection_points.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../Polygon_Intersection_points"
	cd "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Polygon_Intersection_points.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/CMakeFiles/Polygon_Intersection_points.dir/build: Polygon_Intersection_points
.PHONY : app/CMakeFiles/Polygon_Intersection_points.dir/build

app/CMakeFiles/Polygon_Intersection_points.dir/clean:
	cd "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app" && $(CMAKE_COMMAND) -P CMakeFiles/Polygon_Intersection_points.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/Polygon_Intersection_points.dir/clean

app/CMakeFiles/Polygon_Intersection_points.dir/depend:
	cd "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection" "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/app" "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build" "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app" "/Users/jamontanac/Dropbox/Geometria computacional/Polygon_intersection/build/app/CMakeFiles/Polygon_Intersection_points.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : app/CMakeFiles/Polygon_Intersection_points.dir/depend

