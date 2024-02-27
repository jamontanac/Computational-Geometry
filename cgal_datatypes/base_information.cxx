// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <iostream>
#include <limits>

template< class _T >
void print( )
{
  using _L = std::numeric_limits< _T >;

  std::cout << "===================================" << std::endl;
  std::cout << "Mangled name      : " << typeid( _T ).name( ) << std::endl;
  std::cout << "Type size         : " << sizeof( _T ) << std::endl;
  std::cout << "Pointer size      : " << sizeof( _T* ) << std::endl;
  std::cout << "Lowest            : " << _L::lowest( ) << std::endl;
  std::cout << "Minimum           : " << _L::min( ) << std::endl;
  std::cout << "Maximum           : " << _L::max( ) << std::endl;
  std::cout << "Epsilon           : " << _L::epsilon( ) << std::endl;
  std::cout << "Error             : " << _L::round_error( ) << std::endl;
  std::cout << "Infinity          : " << _L::infinity( ) << std::endl;
  std::cout << "Quiet NaN         : " << _L::quiet_NaN( ) << std::endl;
  std::cout << "Signaling NaN     : " << _L::signaling_NaN( ) << std::endl;
  std::cout << "Denorm min        : " << _L::denorm_min( ) << std::endl;
  std::cout << "Is specialized    : " << _L::is_specialized << std::endl;
  std::cout << "Is signed         : " << _L::is_signed << std::endl;
  std::cout << "Is integer        : " << _L::is_integer << std::endl;
  std::cout << "Is exact          : " << _L::is_exact << std::endl;
  std::cout << "Has infinity      : " << _L::has_infinity << std::endl;
  std::cout << "Has quiet NaN     : " << _L::has_quiet_NaN << std::endl;
  std::cout << "Has signaling NaN : " << _L::has_signaling_NaN << std::endl;
  std::cout << "Has denorm        : " << _L::has_denorm << std::endl;
  std::cout << "Has denorm loss   : " << _L::has_denorm_loss << std::endl;
  std::cout << "Round style       : " << _L::round_style << std::endl;
  std::cout << "Is iec559         : " << _L::is_iec559 << std::endl;
  std::cout << "Is bounded        : " << _L::is_bounded << std::endl;
  std::cout << "Is modulo         : " << _L::is_modulo << std::endl;
  std::cout << "Digits            : " << _L::digits << std::endl;
  std::cout << "Digits 10         : " << _L::digits10 << std::endl;
  std::cout << "Max digits 10     : " << _L::max_digits10 << std::endl;
  std::cout << "Radix             : " << _L::radix << std::endl;
  std::cout << "Min exponent      : " << _L::min_exponent << std::endl;
  std::cout << "Min exponent10    : " << _L::min_exponent10 << std::endl;
  std::cout << "Max exponent      : " << _L::max_exponent << std::endl;
  std::cout << "Max exponent10    : " << _L::max_exponent10 << std::endl;
  std::cout << "Traps             : " << _L::traps << std::endl;
  std::cout << "Tinyness before   : " << _L::tinyness_before << std::endl;
  std::cout << "===================================" << std::endl;
}

int main( int argc, char** argv )
{
  print< bool >( );
  print< unsigned char >( );
  print< unsigned short >( );
  print< unsigned int >( );
  print< unsigned long >( );
  print< unsigned long long >( );
  print< signed char >( );
  print< char >( );
  print< short >( );
  print< int >( );
  print< long >( );
  print< long long >( );
  print< float >( );
  print< double >( );
  print< long double >( );

  return( EXIT_SUCCESS );
}

// eof - base_information.cxx
