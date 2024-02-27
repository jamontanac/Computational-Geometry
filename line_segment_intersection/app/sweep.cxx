// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <vector>
#include <CGAL/Cartesian.h>
#include "WavefrontOBJ.h"
#include "LineSegmentIntersection.h"

using TScalar = float;
using TKernel = CGAL::Cartesian< TScalar >;
using TPoint = TKernel::Point_2;
using TSegment = TKernel::Segment_2;
using TPoints = std::vector< TPoint >;
using TSegments = std::vector< TSegment >;

int main( int argc, char** argv )
{
  TSegments segments;
  WavefrontOBJ::read( argv[ 1 ], std::back_inserter( segments ) );

  TPoints intersections;
  LineSegmentIntersection::
    sweep(
      segments.begin( ), segments.end( ), std::back_inserter( intersections )
      );

  std::cout
    << "Number of segments = " << segments.size( )
    << std::endl;
  std::cout
    << "Number of intersections = " << intersections.size( )
    << std::endl;
  unsigned long long c = 0;
  for( const auto& i: intersections )
    std::cout << "Intersection " << ++c << " = " << i << std::endl;

  return( EXIT_SUCCESS );
}

// eof - sweep.cxx
