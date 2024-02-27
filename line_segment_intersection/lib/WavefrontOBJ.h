// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __WavefrontOBJ__h__
#define __WavefrontOBJ__h__

#include <string>

namespace WavefrontOBJ
{
  /**
   */
  template< class _TOutIt >
  bool read( const std::string& fname, _TOutIt data );

} // end namespace

#include "WavefrontOBJ.hxx"

#endif // __WavefrontOBJ__h__

// eof - WavefrontOBJ.h
