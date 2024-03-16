#ifndef __WavefrontOBJ__hxx__
#define __WavefrontOBJ__hxx__

#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <CGAL/Kernel_traits.h>

// -------------------------------------------------------------------------
template< class _TOutIt >
bool WavefrontOBJ::
read( const std::string& fname, _TOutIt data )
{
  // Get some types from inputs
  using _C = typename _TOutIt::container_type;
  using _L = typename _C::value_type;
  using _K = typename CGAL::Kernel_traits< _L >::Kernel;
  using _P = typename _K::Point_2;
  using _I = unsigned long long;
  using _R = typename _K::FT;

  // Read input stream
  std::ifstream ifs( fname.c_str( ) );
  ifs.seekg( 0, std::ios::end );
  std::size_t size = ifs.tellg( );
  ifs.seekg( 0, std::ios::beg );
  std::string buffer( size, 0 );
  ifs.read( &buffer[ 0 ], size );
  ifs.close( );
  std::istringstream input( buffer );

  // Read data
  std::vector< _P > points;
  std::vector< std::pair< _I, _I > > lines;
  std::string line;
  while( std::getline( input, line ) )
  {
    std::istringstream line_str( line );
    std::string tok;
    line_str >> tok;

    if( tok == "v" )
    {
      _R x, y, z;
      line_str >> x >> y >> z;
      points.push_back( _P( x, y ) );
    }
    else if( tok == "l" )
    {
      _I s, e;
      line_str >> s >> e;
      lines.push_back( std::make_pair( s - 1, e - 1 ) );
    } // end if
  } // end while

  // Fill result
  for( const auto& l: lines )
    *data = _L( points[ l.first ], points[ l.second ] );

  return( true );
}

#endif // __WavefrontOBJ__hxx__

