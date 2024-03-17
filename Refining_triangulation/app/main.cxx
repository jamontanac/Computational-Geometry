
#include <iostream>
#include <cstdlib>
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

  // Command-line argument parsing
  int times_to_repeat = 1; // Default value
  double area_threshold = 0.0; // Default value
  std::vector<std::string> args(argv, argv + argc);

  auto it_area_flag = std::find(args.begin(), args.end(), "--area");
  auto it_partitions_flag = std::find(args.begin(), args.end(), "--partitions");
// Parse command-line arguments
  if (it_area_flag != args.end() && std::next(it_area_flag) != args.end())
      area_threshold = std::atof(std::next(it_area_flag)->c_str());

  if (it_partitions_flag != args.end() && std::next(it_partitions_flag) != args.end())
      times_to_repeat = std::atoi(std::next(it_partitions_flag)->c_str());
  
  // std::cout << "Area threshold: " << area_threshold << std::endl;
  // std::cout << "Partitions: " << times_to_repeat << std::endl;

 // Refine the triangulation
    double area_count = 0; // if all triangles are below the threshold, the loop will not run
    int refinement_count = 0;
    auto triangles = get_triangles(T);
    auto number_triangles = triangles.size();
    auto n = T.number_of_points()-2;
    while (refinement_count < times_to_repeat || area_count < number_triangles) {
        // Add the barycenter of each triangle to the triangulation (and connect it to the vertices of the triangle)
        triangles = get_triangles(T);
        // number_triangles =std::pow(3, refinement_count) * number_triangles-3*area_count;
        number_triangles = triangles.size();
        
        area_count = 0;
        for (const auto& t : triangles) {
            auto p0 = T.geometry()[std::get<0>(t)];
            auto p1 = T.geometry()[std::get<1>(t)];
            auto p2 = T.geometry()[std::get<2>(t)];

            // Compute the area of the triangle
            double triangle_area = std::abs(CGAL::area(p0, p1, p2));
            if (triangle_area <= area_threshold){
                area_count++;
                continue;
            }

            // Compute the barycenter
            auto barycenter = CGAL::barycenter(p0, 1, p1, 1, p2, 1);
            unsigned long long barycenter_index = T.add_point(barycenter);

            // Connect the barycenter to the vertices of the triangle
            T.add_edge(std::get<0>(t), barycenter_index);
            T.add_edge(std::get<1>(t), barycenter_index);
            T.add_edge(std::get<2>(t), barycenter_index);

        }
        if(area_count >= n){
            std::cout << "All triangles are below the threshold" << std::endl;
            break;
        }

        std::cout << "Refinement " << refinement_count << " with " <<
        area_count << " triangles above the threshold out of " <<
        number_triangles<< " expected " <<
        n<< std::endl;
        n = 3*(n-area_count);
        refinement_count++;
    } 
  std::cerr<<T<<std::endl;
  return( EXIT_SUCCESS );
}

// eof - main.cxx
