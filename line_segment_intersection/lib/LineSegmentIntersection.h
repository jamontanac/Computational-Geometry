// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __LineSegmentIntersection__h__
#define __LineSegmentIntersection__h__

namespace LineSegmentIntersection
{
  /**
   */
  template< class _TInIt, class _TOutIt >
  void brute_force( _TInIt b, _TInIt e, _TOutIt o );

  /**
   */
  template< class _TInIt, class _TOutIt >
  void sweep( _TInIt b, _TInIt e, _TOutIt o );

} // end namespace

#include "LineSegmentIntersection.hxx"

#endif // __LineSegmentIntersection__h__

// eof - LineSegmentIntersection.h
