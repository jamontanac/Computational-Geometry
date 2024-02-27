// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __CSV__h__
#define __CSV__h__

#include <string>

namespace ConvexHull
{
  /**
   */
  template< class _TOutIt >
  bool read( const std::string& fname, _TOutIt o );

  /**
   */
  template< class _TInIt >
  bool write( const std::string& fname, _TInIt b, _TInIt e );

} // end namespace

#include "CSV.hxx"

#endif // __CSV__h__

// eof - CSV.h
