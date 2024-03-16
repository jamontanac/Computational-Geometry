#ifndef __PUJ_CGAL__Helpers__hxx__
#define __PUJ_CGAL__Helpers__hxx__

// -------------------------------------------------------------------------
template< class _TIt >
auto PUJ_CGAL::Helpers::
polygon_area( _TIt vb, _TIt ve )
{
  auto a = 0;

  for( auto i = vb; i != ve; ++i )
  {
    auto j = i;
    j++;
    if( j == ve )
      j = vb;

    a += ( ( *i )[ 0 ] * ( *j )[ 1 ] ) - ( ( *j )[ 0 ] * ( *i )[ 1 ] );
  } // end for

  return( a / decltype( a )( 2 ) );
}

#endif // __PUJ_CGAL__Helpers__hxx__

// eof - Helpers.hxx
