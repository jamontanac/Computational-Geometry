## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(
  ${PROJECT_SOURCE_DIR}/lib
  )

add_executable(
  PUJ_LineSegmentsIntersections_innocent_brute_force innocent_brute_force.cxx
  )
add_executable(
  PUJ_LineSegmentsIntersections_brute_force brute_force.cxx
  )
add_executable(
  PUJ_LineSegmentsIntersections_sweep sweep.cxx
  )


## eof - CMakeLists.txt
