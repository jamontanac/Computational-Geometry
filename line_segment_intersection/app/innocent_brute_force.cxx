// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <vector>
#include <CGAL/Cartesian.h>
#include "WavefrontOBJ.h"

// -------------------------------------------------------------------------
using TScalar   = float;
using TKernel   = CGAL::Cartesian< TScalar >;
using TPoint    = TKernel::Point_2;
using TVector   = TKernel::Vector_2;
using TSegment  = TKernel::Segment_2;
using TPoints   = std::vector< TPoint >;
using TSegments = std::vector< TSegment >;

// -------------------------------------------------------------------------
TScalar cross( const TVector& a, const TVector& b )
{
  return( ( a[ 0 ] * b[ 1 ] ) - ( a[ 1 ] * b[ 0 ] ) );
}

// -------------------------------------------------------------------------
bool intersection(
  TScalar& t,
  const TPoint& pi, const TPoint& qi, const TPoint& pj, const TPoint& qj
  )
{
  TScalar d = cross( qi - pi, qj - pj );
  if( std::fabs( d ) > TScalar( 0 ) )
  {
    t = cross( pj - pi, qj - pj ) / d;
    return( true );
  }
  else
    return( false );
}

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  TSegments segments;
  WavefrontOBJ::read( argv[ 1 ], std::back_inserter( segments ) );

  TPoints intersections;
  for( auto i = segments.begin( ); i != segments.end( ); ++i )
  {
    auto j = i;
    for( j++; j != segments.end( ); ++j )
    {
      auto pi = i->source( );
      auto qi = i->target( );
      auto pj = j->source( );
      auto qj = j->target( );

      TScalar ti, tj;
      if(
        intersection( ti, pi, qi, pj, qj )
        &&
        intersection( tj, pj, qj, pi, qi )
        )
      {
        if(
          TScalar( 0 ) <= ti && ti <= TScalar( 1 )
          &&
          TScalar( 0 ) <= tj && tj <= TScalar( 1 )
          )
          intersections.push_back( pi + ( ( qi - pi ) * ti ) );
      } // end if

    } // end for
  } // end for

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

// eof - brute_force.cxx
