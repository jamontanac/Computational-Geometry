
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <PUJ_CGAL/Triangulation_2.h>

using TKernel  = CGAL::Exact_predicates_inexact_constructions_kernel;
using TPoint   = TKernel::Point_2;
using TPolygon = std::vector< TPoint >;
using TTriangulation = PUJ_CGAL::Triangulation_2< TKernel >;
// -------------------------------------------------------------------------
template< class _TIt >
void read_polygon( _TIt o )
{
  using TContainer = typename _TIt::container_type;
  using TPoint     = typename TContainer::value_type;
  using TKernel    = typename CGAL::Kernel_traits< TPoint >::Kernel;

  *o = TPoint( 391, 374 ); //  0
  *o = TPoint( 240, 431 ); //  1
  *o = TPoint( 252, 340 ); //  2
  *o = TPoint( 374, 320 ); //  3
  *o = TPoint( 289, 214 ); //  4
  *o = TPoint( 134, 390 ); //  5
  *o = TPoint(  68, 186 ); //  6
  *o = TPoint( 154, 259 ); //  7
  *o = TPoint( 161, 107 ); //  8
  *o = TPoint( 435, 108 ); //  9
  *o = TPoint( 208, 148 ); // 10
  *o = TPoint( 295, 160 ); // 11
  *o = TPoint( 421, 212 ); // 12
  *o = TPoint( 441, 303 ); // 13
}
auto get_triangles(TTriangulation T)
{

  //New structure to keep the geometry, the edges and the triangles
  std::vector< std::tuple<int, int, int> > triangles;

  //iterate over the edges and over the vertex and get the triangles
  for( auto i = 0; i < T.number_of_points( ); ++i )
  {
    for( auto j = i + 1; j < T.number_of_points( ); ++j )
    {
      if( T.edge( i, j ) )
      {
        for( auto k = j + 1; k < T.number_of_points( ); ++k )
        {
          if( T.edge( j, k ) && T.edge( k, i ) )
            triangles.push_back(std::make_tuple(i, j, k));
        } // end for
      } // end if
    } // end for
  } // end for
  return triangles;
}
// -------------------------------------------------------------------------
int main( int argc, char** argv )
{

  // Read polygon
  TPolygon P;
  read_polygon( std::back_inserter( P ) );

  // Triangulate it
  TTriangulation T;
  if( PUJ_CGAL::Helpers::polygon_area( P.begin( ), P.end( ) ) >= 0 )
    T.build_from_polygon( P.begin( ), P.end( ) );
  else
    T.build_from_polygon( P.rbegin( ), P.rend( ) );
  auto triangles = get_triangles(T);
  for(const auto &t : triangles)
  {
    auto p0 = T.geometry()[std::get<0>(t)];
    auto p1 = T.geometry()[std::get<1>(t)];
    auto p2 = T.geometry()[std::get<2>(t)];
    // std::cout << p0.x() << " " << p1.x() << " " << p2.x() << std::endl;
    // auto [p0, p1, p2] = std::tie(T.geometry()[std::get<0>(t)], T.geometry()[std::get<1>(t)], T.geometry()[std::get<2>(t)]);
    // auto barycenter = (p0 + p1 + p2) / 3;
    auto barycenter = CGAL::barycenter(p0,1,p1,1,p2,1);
    // std::cout << barycenter.x() << " " << barycenter.y() << std::endl;
    unsigned long long barycenter_index = T.add_point(barycenter);

    T.add_edge(std::get<0>(t), barycenter_index);
    T.add_edge(std::get<1>(t), barycenter_index);
    T.add_edge(std::get<2>(t), barycenter_index);
    // std::cout << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
  }

  std::cout<<T<<std::endl;
  return( EXIT_SUCCESS );
}

// eof - main.cxx
