// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __ConvexHull__h__
#define __ConvexHull__h__

namespace ConvexHull
{
  /**
   */
  template< class _TInIt, class _TOutIt >
  void brute_force( _TInIt b, _TInIt e, _TOutIt o );

  /**
   */
  template< class _TInIt, class _TOutIt >
  void incremental( _TInIt b, _TInIt e, _TOutIt o );

} // end namespace

#include "ConvexHull.hxx"

#endif // __ConvexHull__h__

// eof - ConvexHull.h
