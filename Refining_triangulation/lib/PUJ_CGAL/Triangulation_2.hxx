#ifndef __PUJ_CGAL__Triangulation_2__hxx__
#define __PUJ_CGAL__Triangulation_2__hxx__

#include <list>
#include <stack>
#include <type_traits>

#include <PUJ_CGAL/Helpers.h>

#include <CGAL/partition_2.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/intersections.h>

// -------------------------------------------------------------------------
template< class _TKernel >
PUJ_CGAL::Triangulation_2< _TKernel >::
Triangulation_2( )
{
}

// -------------------------------------------------------------------------
template< class _TKernel >
PUJ_CGAL::Triangulation_2< _TKernel >::
Triangulation_2( const Self& other )
{
  this->m_Geometry.insert(
    this->m_Geometry.begin( ),
    other.m_Geometry.begin( ), other.m_Geometry.end( )
    );
  this->m_Topology.insert(
    this->m_Topology.begin( ),
    other.m_Topology.begin( ), other.m_Topology.end( )
    );
}

// -------------------------------------------------------------------------
template< class _TKernel >
typename PUJ_CGAL::Triangulation_2< _TKernel >::
Self& PUJ_CGAL::Triangulation_2< _TKernel >::
operator=( const Self& other )
{
  this->m_Geometry.clear( );
  this->m_Topology.clear( );

  this->m_Geometry.insert(
    this->m_Geometry.begin( ),
    other.m_Geometry.begin( ), other.m_Geometry.end( )
    );
  this->m_Topology.insert(
    this->m_Topology.begin( ),
    other.m_Topology.begin( ), other.m_Topology.end( )
    );

  return( *this );
}

// -------------------------------------------------------------------------
template< class _TKernel >
typename PUJ_CGAL::Triangulation_2< _TKernel >::
TGeometry& PUJ_CGAL::Triangulation_2< _TKernel >::
geometry( )
{
  return( this->m_Geometry );
}

// -------------------------------------------------------------------------
template< class _TKernel >
const typename PUJ_CGAL::Triangulation_2< _TKernel >::
TGeometry& PUJ_CGAL::Triangulation_2< _TKernel >::
geometry( ) const
{
  return( this->m_Geometry );
}

// -------------------------------------------------------------------------
template< class _TKernel >
template< class _TPoint >
unsigned long long PUJ_CGAL::Triangulation_2< _TKernel >::
add_point( const _TPoint& p )
{
  this->m_Geometry.push_back( TPoint( p[ 0 ], p[ 1 ] ) );

  unsigned long long n = this->m_Geometry.size( );
  TTopology old_topo = this->m_Topology;
  this->m_Topology.resize( n * n );
  std::fill( this->m_Topology.begin( ), this->m_Topology.end( ), false );

  for( unsigned long long i = 0; i < n - 1; ++i )
    for( unsigned long long j = 0; j < n - 1; ++j )
      this->m_Topology[ ( i * n ) + j ] = old_topo[ ( i * ( n - 1 ) ) + j ];

  return( n - 1 );
}

// -------------------------------------------------------------------------
template< class _TKernel >
template< class _TIt >
void PUJ_CGAL::Triangulation_2< _TKernel >::
add_points( _TIt b, _TIt e )
{
  for( _TIt i = b; i != e; ++i )
    this->add_point( *i );
}

// -------------------------------------------------------------------------
template< class _TKernel >
unsigned long long PUJ_CGAL::Triangulation_2< _TKernel >::
number_of_points( ) const
{
  return( this->m_Geometry.size( ) );
}

// -------------------------------------------------------------------------
template< class _TKernel >
void PUJ_CGAL::Triangulation_2< _TKernel >::
add_edge( const unsigned long long& a, const unsigned long long& b )
{
  unsigned long long n = this->m_Geometry.size( );
  if( a < n && b < n )
  {
    this->m_Topology[ ( a * n ) + b ] = true;
    this->m_Topology[ ( b * n ) + a ] = true;
  } // end if
}

// -------------------------------------------------------------------------
template< class _TKernel >
void PUJ_CGAL::Triangulation_2< _TKernel >::
remove_edge( const unsigned long long& a, const unsigned long long& b )
{
  unsigned long long n = this->m_Geometry.size( );
  if( a < n && b < n )
  {
    this->m_Topology[ ( a * n ) + b ] = false;
    this->m_Topology[ ( b * n ) + a ] = false;
  } // end if
}

// -------------------------------------------------------------------------
template< class _TKernel >
bool PUJ_CGAL::Triangulation_2< _TKernel >::
edge( const unsigned long long& a, const unsigned long long& b ) const
{
  unsigned long long n = this->m_Geometry.size( );
  if( a < n && b < n )
    return( this->m_Topology[ ( a * n ) + b ] );
  else
    return( false );
}

// -------------------------------------------------------------------------
template< class _TKernel >
template< class _TIt >
void PUJ_CGAL::Triangulation_2< _TKernel >::
build_from_polygon( _TIt b, _TIt e )
{
  // Get some types from inputs
  using TInputPoint  = typename std::iterator_traits< _TIt >::value_type;
  using TInputKernel = typename CGAL::Kernel_traits< TInputPoint >::Kernel;
  using TPropMap     = typename CGAL::Pointer_property_map< TPoint >::type;
  using TPartTraits  = CGAL::Partition_traits_2< TKernel, TPropMap >;
  using TPartPoint   = typename TPartTraits::Point_2;
  using TPartPolygon = typename TPartTraits::Polygon_2;

  // Prepare triangulation
  this->m_Geometry.clear( );
  this->m_Topology.clear( );

  this->add_points( b, e );
  long long n = this->number_of_points( );

  // Prepare an easiest polygon representation
  TPartTraits traits( CGAL::make_property_map( this->m_Geometry )  );
  TPartPolygon P;
  for( long long i = 0; i < n; ++i )
  {
    P.push_back( i );
    this->add_edge( i, ( i + 1 ) % n );
  } // end for

  // Y-monotone polygons
  std::list< TPartPolygon > monotones;
  CGAL::y_monotone_partition_2(
    P.vertices_begin( ), P.vertices_end( ),
    std::back_inserter( monotones ),
    traits
    );

  // Process each monotone polygon
  for( const auto& m: monotones )
  {
    for( auto i = m.container( ).begin( ); i != m.container( ).end( ); ++i )
    {
      auto j = i;
      j++;
      if( j == m.container( ).end( ) )
        j = m.container( ).begin( );
      this->add_edge( *i, *j );
    } // end for
    this->_build_from_polygon_monotone( m );
  } // end for
}

// -------------------------------------------------------------------------
template< class _TKernel >
template< class _TPolygon >
bool PUJ_CGAL::Triangulation_2< _TKernel >::
_build_from_polygon_self_intersection(
  const _TPolygon& P, const long long& a, const long long& b
  ) const
{
  using TSegment = typename TKernel::Segment_2;

  TSegment s( this->m_Geometry[ a ], this->m_Geometry[ b ] );
  if( s.is_degenerate( ) )
    return( false );

  unsigned long long count = 0;
  for( auto p = P.begin( ); p != P.end( ); ++p )
  {
    auto q = p;
    q++;
    if( q == P.end( ) )
      q = P.begin( );

    if( *p != a && *p != b && *q != a && *q != b )
    {
      TSegment t( this->m_Geometry[ *p ], this->m_Geometry[ *q ] );
      if( !( t.is_degenerate( ) ) )
        if( const auto& i = CGAL::intersection( s, t ) )
          if( const TPoint* r = boost::get< TPoint >( &*i ) )
            count++;
    } // end if
  } // end for
  return( count > 0 );
}

// -------------------------------------------------------------------------
template< class _TKernel >
template< class _TPolygon >
void PUJ_CGAL::Triangulation_2< _TKernel >::
_build_from_polygon_monotone( const _TPolygon& P )
{
  // Some values
  const auto& C = P.container( );
  long long n = this->m_Geometry.size( );
  long long np = C.size( );
  if( np <= 3 )
    return;

  // Build ordered container
  using TCont = decltype( C );
  using TCIt = typename std::remove_reference< TCont >::type::const_iterator;
  std::vector< TCIt > U;
  for( auto i = C.begin( ); i != C.end( ); ++i )
    U.push_back( i );
  U.shrink_to_fit( );

  std::sort(
    U.begin( ), U.end( ),
    [&]( const auto& a, const auto& b ) -> bool
    {
      if( this->m_Geometry[ *a ][ 1 ] == this->m_Geometry[ *b ][ 1 ] )
        return( this->m_Geometry[ *a ][ 0 ] < this->m_Geometry[ *b ][ 0 ] );
      else
        return( this->m_Geometry[ *b ][ 1 ] < this->m_Geometry[ *a ][ 1 ] );
    }
    );

  // Compute chains
  std::vector< bool > chain( this->m_Geometry.size( ), false );
  chain.shrink_to_fit( );

  auto M = *( U.begin( ) );
  auto m = *( U.rbegin( ) );
  auto i = M;
  while( i != m )
  {
    chain[ *i ] = true;
    i++;
    if( i == C.end( ) )
      i = C.begin( );
  } // end while
  chain[ *m ] = true;

  // Prepare stack
  std::stack< long long > S;
  S.push( 0 );
  S.push( 1 );

  // Main loop
  for( long long j = 2; j < np - 1; ++j )
  {
    if( chain[ *( U[ j ] ) ] == chain[ *( U[ S.top( ) ] ) ] )
    {
      long long last_popped = -1;
      if( S.size( ) > 0 )
      {
        last_popped = S.top( );
        S.pop( );
      } // end if

      bool stop = false;
      while( S.size( ) > 0 && !stop )
      {
        if(
          !(
            this->_build_from_polygon_self_intersection(
            P, *( U[ j ] ), *( U[ S.top( ) ] )
            )
            )
          )
        {
          this->add_edge( *( U[ j ] ), *( U[ S.top( ) ] ) );
          last_popped = S.top( );
          S.pop( );
        }
        else
          stop = true;
      } // end while

      if( last_popped >= 0 )
        S.push( last_popped );
      S.push( j );
    }
    else
    {
      while( S.size( ) > 0 )
      {
        if( S.size( ) > 1 )
          this->add_edge( *( U[ j ] ), *( U[ S.top( ) ] ) );
        S.pop( );
      } // end while
      S.push( j - 1 );
      S.push( j );
    } // end if
  } // end for

  // Last diagonals
  S.pop( );
  while( S.size( ) > 1 )
  {
    this->add_edge( *( U[ np - 1 ] ), *( U[ S.top( ) ] ) );
    S.pop( );
  } // end while
}

// -------------------------------------------------------------------------
template< class _TKernel >
void PUJ_CGAL::Triangulation_2< _TKernel >::
_to_stream( std::ostream& out ) const
{
  for( auto v: this->geometry( ) )
    out << "v " << v << " 0" << std::endl;
  out << std::endl;

  long long n = this->number_of_points( );
  for( long long i = 0; i < n; ++i )
    for( long long j = i + 1; j < n; ++j )
      if( this->edge( i, j ) )
        out << "l " << ( i + 1 ) << " " << ( j + 1 ) << std::endl;
}

#endif // __PUJ_CGAL__Triangulation_2__hxx__

