
include_directories(
  ${PROJECT_SOURCE_DIR}/lib
  )

add_executable(PolygonTriangulation main.cxx)
target_link_libraries(PolygonTriangulation ${CGAL_LIBRARIES})

## eof - CMakeLists.txt
