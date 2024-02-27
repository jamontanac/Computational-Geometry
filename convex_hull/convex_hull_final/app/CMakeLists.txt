## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

include_directories(
  ${PROJECT_SOURCE_DIR}/lib
  )

add_executable(PUJ_ConvexHull_create_pointcloud create_pointcloud.cxx)
add_executable(PUJ_ConvexHull_brute_force brute_force.cxx)
add_executable(PUJ_ConvexHull_incremental incremental.cxx)

## eof - CMakeLists.txt
