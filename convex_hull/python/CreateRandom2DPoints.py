## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import random, sys

# Input arguments
if len( sys.argv ) < 2:
  print( 'Usage: ' + sys.argv[ 0 ] + ' number_of_points' )
  sys.exit( 1 )
# end if
N = int( sys.argv[ 1 ] )

# Generate points in CSV compatible shape
min_v = -100.0
max_v = 100.0
for n in range( N ):
  print(
    str( random.uniform( min_v, max_v ) )
    +
    ' '
    +
    str( random.uniform( min_v, max_v ) )
    )
# end for

## eof - CreateRandom2DPoints.oy
