// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <random>
#include <sstream>

// Some types
using TScalar = float;

int main( int argc, char** argv )
{
  // Input arguments
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " number_of_points" << std::endl;
    return( EXIT_FAILURE );
  } // end if
  unsigned long long N;
  std::istringstream( argv[ 1 ] ) >> N;

  // Random numbers generator
  std::random_device dev;
  std::mt19937 gen( dev( ) );
  std::uniform_real_distribution< TScalar > dis( -100, 100 );

  // Generate points in CSV compatible shape
  for( unsigned long long n = 0; n < N; ++n )
    std::cout << dis( gen ) << " " << dis( gen ) << std::endl;

  return( EXIT_SUCCESS );
}

// eof - CreateRandom2DPoints.cxx
