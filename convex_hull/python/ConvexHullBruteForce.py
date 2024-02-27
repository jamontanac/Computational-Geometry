## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import csv, sys

'''
Function to read CSV file
'''
def read_csv( fname ):
  csvfile = open( fname )
  csvrows = csv.reader( csvfile, delimiter = ' ' )

  points = []
  for r in csvrows:
    points += [ tuple( [ float( x ) for x in r ] ) ]
  # end for
  csvfile.close( )

  return points
# end def

'''
Function to check segment-point side
'''
def side( p, q, r ):
  a = [ r[ 0 ] - p[ 0 ], r[ 1 ] - p[ 1 ] ]
  b = [ q[ 0 ] - p[ 0 ], q[ 1 ] - p[ 1 ] ]

  z = ( a[ 0 ] * b[ 1 ] ) - ( a[ 1 ] * b[ 0 ] )

  if   z < 0 : return 'left'
  elif z > 0 : return 'right'
  else       : return 'over'
# end def

'''
Convex hull algorithm: brute force
'''
def convex_hull( points ):
  n = len( points )

  # For each possible edge...
  indices = [ -1 for i in range( n ) ]
  tip = -1
  for i in range( n ):
    p = points[ i ]
    for j in range( n ):
      if i != j:
        q = points[ j ]
        border_edge = True

        # ... check all point (not on its limits)
        for k in range( n ):
          if i != k and j != k:
            r = points[ k ]
            if side( p, q, r ) != 'left':
              border_edge = False
            # end if
          # end if
        # end for

        # Ok, is a border edge
        if border_edge == True:
          tip = i
          indices[ j ] = i
        # end if

      # end if
    # end for
  # end for

  # Backtrack points from indices
  ch = []
  i = tip
  while indices[ i ] != tip:
    ch += [ points[ i ] ]
    i = indices[ i ]
  # end while
  ch += [ points[ i ] ]

  return ch
# end def

'''
Main function
'''
# Input arguments
if len( sys.argv ) < 2:
  print( 'Usage: ' + sys.argv[ 0 ] << ' points_file' )
  sys.exit( 1 )
# end if

# Read CSV
points = read_csv( sys.argv[ 1 ] );

# Compute convex hull
ch = convex_hull( points )

# Print polygon points
for p in ch:
  print( str( p[ 0 ] ) + ' ' + str( p[ 1 ] ) )
# end for
print( str( ch[ 0 ][ 0 ] ) + ' ' + str( ch[ 0 ][ 1 ] ) )

## eof - ConvexHullBruteForce.py
