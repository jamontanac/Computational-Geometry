// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <CGAL/Cartesian.h>

// Some types
using TInt = long long;
using TReal = float;
// using TPoint = std::array< TReal, 2 >;
using TKernel = CGAL::Cartesian< TReal >;
using TPoint = TKernel::Point_2;
using TPoints = std::vector< TPoint >;

// -------------------------------------------------------------------------
// Function to read CSV file
// -------------------------------------------------------------------------
TPoints read_csv( std::string fname )
{
  // Open input stream
  std::ifstream input( fname.c_str( ) );
  /* TODO: SURPRISE!!!
     std::ifstream ifs( fname.c_str( ) );
     ifs.seekg( 0, std::ios::end );
     std::size_t size = ifs.tellg( );
     ifs.seekg( 0, std::ios::beg );
     std::string buffer( size, 0 );
     ifs.read( &buffer[ 0 ], size );
     ifs.close( );
     std::istringstream input( buffer );
  */

  // Read data
  TPoints points;
  std::string line;
  TInt n = 0;
  while( std::getline( input, line ) )
  {
    std::size_t s = line.find( ' ' );
    TPoint p;
    TReal x, y;
    std::istringstream( line.substr( 0, s ) ) >> x;
    std::istringstream( line.substr( s + 1 ) ) >> y;
    // std::istringstream( line.substr( s + 1 ) ) >> p[ 1 ];
    p = TPoint( x, y );
    points.push_back( p );
  } // ed while

  // Close input stream
  input.close( );

  // Return points
  return( points );
}

// -------------------------------------------------------------------------
// Function to check segment-point side
// -------------------------------------------------------------------------
#define SIDE_LEFT  0
#define SIDE_RIGHT 1
#define SIDE_OVER  2

int side( TPoint p, TPoint q, TPoint r )
{

  TKernel::Line_2 l( p, q );

  auto z = l.oriented_side( r );
  if (z==CGAL::ON_POSITIVE_SIDE) return SIDE_LEFT;
  if (z==CGAL::ON_NEGATIVE_SIDE) return SIDE_RIGHT;
  return SIDE_OVER;

}

// -------------------------------------------------------------------------
// Function to read CSV file
// -------------------------------------------------------------------------
Tpoints convex_hull( TPoints points )
{
  // Sort points
  auto cmp = []( TPoint a, TPoint b ) -> bool
  {
    if( a[ 0 ] == b[ 0 ] )
      return( a[ 1 ] < b[ 1 ] );
    else
      return( a[ 0 ] < b[ 0 ] );
  };
  std::sort( points.begin( ), points.end( ), cmp);

  std::list<TPoint> ch;
  ch.push_back(points[0]);
  ch.push_back(points[1]);

  for (TInt i = 2; i < points.size(); i++)
  {
    ch.push_back(points[i]);
    auto r = ch.rbegin();
    auto q=r;q++;
    auto p=q;p++;
    while (ch.size()>=3 && side(*p,*q,*r)==SIDE_RIGHT)
    {
      ch.erase(q);
      r = ch.rbegin();
      q=r;q++;
      p=q;p++;
    }
  }
  return( ch );
}
// TPoints convex_hull( TPoints points )
// {
//   TInt n = points.size( );

//   // For each possible edge...
//   std::vector< TInt > indices( n, -1 );
//   TInt tip = -1;
//   for( TInt i = 0; i < n; ++i )
//   {
//     TPoint p = points[ i ];
//     for( TInt j = 0; j < n; ++j )
//     {
//       if( i != j )
//       {
//         TPoint q = points[ j ];
//         bool border_edge = true;

//         // ... check all point (not on its limits)
//         for( TInt k = 0; k < n; ++k )
//         {
//           if( i != k && j != k )
//           {
//             TPoint r = points[ k ];
//             if( side( p, q, r ) != SIDE_LEFT )
//               border_edge = false;
//           } // end if
//         } // end for

//         // Ok, is a border edge
//         if( border_edge )
//         {
//           tip = i;
//           indices[ j ] = i;
//         } // end if
//       } // end if
//     } // end for
//   } // end for

//   // Backtrack points from indices
//   TPoints ch;
//   TInt i = tip;
//   while( indices[ i ] != tip )
//   {
//     ch.push_back( points[ i ] );
//     i = indices[ i ];
//   } // end while
//   ch.push_back( points[ i ] );

//   return( ch );
// }

// -------------------------------------------------------------------------
// Main function
// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  // Input arguments
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " points_file" << std::endl;
    return( EXIT_FAILURE );
  } // end if

  // Read CSV
  TPoints points = read_csv( argv[ 1 ] );

  // Compute convex hull
  TPoints ch = convex_hull( points );

  // Print polygon points
  for( TInt i = 0; i < ch.size( ); ++i )
    std::cout << ch[ i ][ 0 ] << " " << ch[ i ][ 1 ] << std::endl;
  std::cout << ch[ 0 ][ 0 ] << " " << ch[ 0 ][ 1 ] << std::endl;

  return( EXIT_SUCCESS );
}

// eof - ConvexHullBruteForce.cxx
