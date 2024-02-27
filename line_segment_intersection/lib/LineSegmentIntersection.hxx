// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __LineSegmentIntersection__hxx__
#define __LineSegmentIntersection__hxx__

#include <map>
#include <utility>
#include <vector>
#include <CGAL/intersections.h>

// -------------------------------------------------------------------------
template< class _TInIt, class _TOutIt >
void LineSegmentIntersection::
brute_force( _TInIt b, _TInIt e, _TOutIt o )
{
  using TSegment = typename std::iterator_traits< _TInIt >::value_type;
  using TKernel  = typename CGAL::Kernel_traits< TSegment >::Kernel;
  using TPoint   = typename TKernel::Point_2;

  for( _TInIt i = b; i != e; ++i )
  {
    _TInIt j = i;
    for( j++; j != e; ++j )
      if( const auto& x = CGAL::intersection( *i, *j ) )
        if( const TPoint* p = boost::get< TPoint >( &*x ) )
          *o = *p;
  } // end for
}

// -------------------------------------------------------------------------
template< class _TInIt, class _TOutIt >
void LineSegmentIntersection::
sweep( _TInIt b, _TInIt e, _TOutIt o )
{
  using TSegment = typename std::iterator_traits< _TInIt >::value_type;
  using TKernel = typename CGAL::Kernel_traits< TSegment >::Kernel;
  using TPoint = typename TKernel::Point_2;

  // Comparison "lambda" for event queue
  struct SQueueCmp
  {
    bool operator()( const TPoint& p, const TPoint& q ) const
      {
        if( p[ 1 ] == q[ 1 ] ) return( p[ 0 ] < q[ 0 ] );
        else                   return( p[ 1 ] > q[ 1 ] );
      }
  } Q_cmp;

  // Fill queue
  using TSegments = std::vector< TSegment >;
  std::map< TPoint, TSegments, SQueueCmp > Q;
  for( _TInIt i = b; i != e; ++i )
  {
    // Reorder segment if necessary
    TSegment s = ( Q_cmp( i->source( ), i->target( ) ) )? *i: i->opposite( );

    // Add upper event point
    Q.insert( std::make_pair( s.source( ), TSegments( ) ) )
      .first->second.push_back( s );

    // Add lower event point
    Q.insert( std::make_pair( s.target( ), TSegments( ) ) );
  } // end for

  // Main loop
  while( Q.size( ) > 0 )
  {
    // Get next event point
    auto pIt = Q.begin( );
    TPoint p = pIt->first;
    TSegments U = pIt->second;
    Q.erase( pIt );

    // Handle event point

  } // end while
}

#endif // LineSegmentIntersection__hxx__

// eof - LineSegmentIntersection.hxx
