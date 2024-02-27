// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
// Floating numbers standard: IEEE_754
// =========================================================================

#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

/* TODO
   https://doc.cgal.org/latest/Kernel_23/classKernel.html

   CGAL::Cartesian<>
   CGAL::Homogeneous<>
   CGAL::Simple_cartesian<>
   CGAL::Simple_homogeneous<>
   CGAL::Filtered_kernel<>
   CGAL::Exact_predicates_exact_constructions_kernel
   CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt
   CGAL::Exact_predicates_inexact_constructions_kernel
*/

int main( int argc, char** argv )
{
  using _S = float;
  using _M = unsigned int;

  // Check input arguments
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[ 0 ] << " number" << std::endl;
    return( EXIT_FAILURE );
  } // end if

  // Input number
  _S n;
  std::istringstream arg( argv[ 1 ] );
  arg >> n;

  // Cast number bits
  _M h;
  std::memcpy( &h, &n, sizeof( _S ) );

  // Add one bit
  _M h1 = h + 1;

  // Number with one bit
  _S n1;
  std::memcpy( &n1, &h1, sizeof( _S ) );

  // Show some information
  std::cout << "Sizes       : "
            << sizeof( _S ) << " " << sizeof( _M ) << std::endl;
  std::cout << "Number      : " << std::dec << n << std::endl;
  std::cout << "Next number : " << std::dec << n1 << std::endl;
  std::cout << "Hexadecimal : 0x"
            << std::setfill( '0' ) << std::setw( sizeof( _M ) << 1 )
            << std::hex << h << std::dec << " (" << h << ")" << std::endl;
  std::cout << "Next hexa   : 0x"
            << std::setfill( '0' ) << std::setw( sizeof( _M ) << 1 )
            << std::hex << h1 << std::dec << " (" << h1 << ")" << std::endl;

  return( EXIT_SUCCESS );
}

// eof - type_information.cxx
