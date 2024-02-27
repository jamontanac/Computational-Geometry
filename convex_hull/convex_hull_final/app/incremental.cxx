// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <vector>
#include <CGAL/Cartesian.h>

#include "CSV.h"
#include "ConvexHull.h"

// -- Some types
using TReal = float;
using TKernel = CGAL::Cartesian< TReal >;
using TPoint = TKernel::Point_2;
using TPoints = std::vector< TPoint >;
using TConvexHull = std::vector< TPoint >;

// -- Main function
int main( int argc, char** argv )
{
  // Input arguments
  if( argc < 3 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " input output" << std::endl;
    return( EXIT_FAILURE );
  } // end if

  // Read CSV
  TPoints P;
  ConvexHull::read( argv[ 1 ], std::back_inserter( P ) );

  // Compute convex hull
  TConvexHull ch;
  ConvexHull::incremental( P.begin( ), P.end( ), std::back_inserter( ch ) );

  // Save results
  ConvexHull::write( argv[ 2 ], ch.begin( ), ch.end( ) );

  return( EXIT_SUCCESS );
}

// eof - incremental.cxx
