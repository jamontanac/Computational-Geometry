// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CSV__hxx__
#define __CSV__hxx__

#include <fstream>
#include <sstream>

// -------------------------------------------------------------------------
template< class _TOutIt >
bool ConvexHull::
read( const std::string& fname, _TOutIt o )
{
  // Get some types from inputs
  using TContainer = typename _TOutIt::container_type;
  using TPoint     = typename TContainer::value_type;
  using TKernel    = typename CGAL::Kernel_traits< TPoint >::Kernel;
  using TReal      = typename TKernel::FT;

  // Open input stream: load all file into a memory buffer
  std::ifstream ifs( fname.c_str( ) );
  ifs.seekg( 0, std::ios::end );
  std::size_t size = ifs.tellg( );
  ifs.seekg( 0, std::ios::beg );
  std::string buffer( size, 0 );
  ifs.read( &buffer[ 0 ], size );
  ifs.close( );
  std::istringstream input( buffer );

  // Read data
  std::string line;
  while( std::getline( input, line ) )
  {
    std::size_t s = line.find( ' ' );

    TReal x, y;
    std::istringstream( line.substr( 0, s ) ) >> x;
    std::istringstream( line.substr( s + 1 ) ) >> y;

    *o = TPoint( x, y );
  } // end while

  return( true );
}

// -------------------------------------------------------------------------
template< class _TInIt >
bool ConvexHull::
write( const std::string& fname, _TInIt b, _TInIt e )
{
  std::stringstream out;
  for( _TInIt i = b; i != e; ++i )
    out << *i << std::endl;

  std::ofstream ofs( fname.c_str( ) );
  ofs.write( out.str( ).c_str( ), out.str( ).size( ) );
  ofs.close( );

  return( true );
}

#endif // __CSV__hxx__

// eof - CSV.hxx
