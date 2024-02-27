// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __ConvexHull__hxx__
#define __ConvexHull__hxx__

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <CGAL/Kernel_traits.h>

// -------------------------------------------------------------------------
template< class _TInIt, class _TOutIt >
void ConvexHull::
brute_force( _TInIt b, _TInIt e, _TOutIt o )
{
  // Get some types from inputs
  using TInPoint = typename std::iterator_traits< _TInIt >::value_type;
  using TInKernel = typename CGAL::Kernel_traits< TInPoint >::Kernel;
  using TInLine = typename TInKernel::Line_2;
  using TOutContainer = typename _TOutIt::container_type;
  using TOutPoint = typename TOutContainer::value_type;
  using TOutKernel = typename CGAL::Kernel_traits< TOutPoint >::Kernel;
  using TOutReal = typename TOutKernel::FT;

  // For each possible edge...
  std::map< _TInIt, _TInIt > indices;
  _TInIt tip = e;
  for( _TInIt i = b; i != e; ++i )
  {
    for( _TInIt j = b; j != e; ++j )
    {
      if( i != j )
      {
        bool border_edge = true;

        // ... check all point (not on its limits)
        TInLine l( *i, *j );
        for( _TInIt k = b; k != e; ++k )
          if( i != k && j != k )
            if( l.oriented_side( *k ) != CGAL::ON_POSITIVE_SIDE )
              border_edge = false;

        // Ok, is a border edge
        if( border_edge )
        {
          tip = i;
          indices[ j ] = i;
        } // end if

      } // end if
    } // end for
  } // end for

  // Backtrack points from indices
  _TInIt i = tip;
  while( indices[ i ] != tip )
  {
    *o = TOutPoint( TOutReal( i->x( ) ), TOutReal( i->y( ) ) );
    i = indices[ i ];
  } // end while
}

// -------------------------------------------------------------------------
template< class _TInIt, class _TOutIt >
void ConvexHull::
incremental( _TInIt b, _TInIt e, _TOutIt o )
{
  // Get some types from inputs
  using TInPoint = typename std::iterator_traits< _TInIt >::value_type;
  using TInKernel = typename CGAL::Kernel_traits< TInPoint >::Kernel;
  using TInLine = typename TInKernel::Line_2;
  using TOutContainer = typename _TOutIt::container_type;
  using TOutPoint = typename TOutContainer::value_type;
  using TOutKernel = typename CGAL::Kernel_traits< TOutPoint >::Kernel;
  using TOutReal = typename TOutKernel::FT;

  // Sort input points
  struct SInItCmp
  {
    bool operator()( const _TInIt& p, const _TInIt& q ) const
      {
        if( p->x( ) == q->x( ) ) return( p->y( ) < q->y( ) );
        else                     return( p->x( ) < q->x( ) );
      }
  };
  std::set< _TInIt, SInItCmp > P;
  for( _TInIt i = b; i != e; ++i )
    P.insert( i );

  // Half hulls algorithm
  auto half_hull = [ &o ]( auto f, auto l, auto& H ) -> void
    {
      auto c = f;
      H.push_back( *c ); c++;
      H.push_back( *c ); c++;
      for( ; c != l; ++c )
      {
        H.push_back( *c );

        auto r = H.rbegin( );
        auto q = r; q++;
        auto p = q; p++;
        while(
          H.size( ) > 2
          &&
          TInLine( **p, **q ).oriented_side( **r ) != CGAL::ON_NEGATIVE_SIDE
          )
        {
          H.erase( p.base( ) );

          q = r; q++;
          p = q; p++;
        } // end while
      } // end for

      // Copy to output
      H.erase( H.begin( ) );
      for( auto i: H )
        *o = TOutPoint( TOutReal( i->x( ) ), TOutReal( i->y( ) ) );
    };

  // Compute half hulls
  std::list< _TInIt > U, L;
  half_hull( P.begin( ), P.end( ), U );
  half_hull( P.rbegin( ), P.rend( ), L );
}

#endif // __ConvexHull__hxx__

// eof - ConvexHull.hxx
