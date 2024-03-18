
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

std::pair<TPoint, double> make_barycenter_partition(std::tuple<int, int, int> t, const TTriangulation &T){
    auto p0 = T.geometry()[std::get<0>(t)];
    auto p1 = T.geometry()[std::get<1>(t)];
    auto p2 = T.geometry()[std::get<2>(t)];
    double triangle_area = std::abs(CGAL::area(p0, p1, p2));
    
    // Compute the barycenter
    auto barycenter = CGAL::barycenter(p0, 1, p1, 1, p2, 1);
    return std::make_pair(barycenter, triangle_area);
}

void refine_triangulation_times(TTriangulation &T, int partitions){
  auto triangles = get_triangles(T);
  int refinement_count = 1;

  while( refinement_count <= partitions) {
      triangles = get_triangles(T);
      for (const auto& t : triangles) {
        auto barycenter = make_barycenter_partition(t,T);
        unsigned long long barycenter_index = T.add_point(barycenter.first);
        T.add_edge(std::get<0>(t), barycenter_index);
        T.add_edge(std::get<1>(t), barycenter_index);
        T.add_edge(std::get<2>(t), barycenter_index);
      }
      refinement_count++;
      std::cout<<"Refinement count: "<<refinement_count<<std::endl;
  }
}

void refine_triangulation_area(TTriangulation &T, double area_threshold){
  auto triangles = get_triangles(T);
  unsigned long long number_triangles = T.number_of_points()-2;
  unsigned long long n = 0;
  unsigned long long area_count = 0;
  while( area_count <= number_triangles) {
      triangles = get_triangles(T);
      for (const auto& t : triangles) {
        auto barycenter = make_barycenter_partition(t,T);

        if (barycenter.second <= area_threshold){
          area_count++;
          n++;
          continue;
        }
        
        // std::cout<<area_count<<std::endl;
        unsigned long long barycenter_index = T.add_point(barycenter.first);
        T.add_edge(std::get<0>(t), barycenter_index);
        T.add_edge(std::get<1>(t), barycenter_index);
        T.add_edge(std::get<2>(t), barycenter_index);
        n+=3;
      }
      if (number_triangles == area_count)
        break;
      number_triangles = n;
      // std::cout<<"supposed "<<3*number_triangles-2*area_count<<std::endl;
      // std::cout<<"Number of triangles: "<<number_triangles<<" triangles over area "<<area_count<<std::endl;
      area_count = 0;
      n=0;
  }
}

void refine_triangulation_both(TTriangulation &T, int partitions, double area_threshold){
  auto triangles = get_triangles(T);
  unsigned long long number_triangles = T.number_of_points()-2;
  unsigned long long n = 0;
  unsigned long long area_count = 0;
  int refinement_count = 0;

  while( refinement_count <= partitions || area_count <= number_triangles) {
      triangles = get_triangles(T);
      for (const auto& t : triangles) {
        auto barycenter = make_barycenter_partition(t,T);

        if (barycenter.second <= area_threshold){
          area_count++;
          n++;
          continue;
        }
        
        unsigned long long barycenter_index = T.add_point(barycenter.first);
        T.add_edge(std::get<0>(t), barycenter_index);
        T.add_edge(std::get<1>(t), barycenter_index);
        T.add_edge(std::get<2>(t), barycenter_index);
        n+=3;
      }
      if (number_triangles == area_count)
        break;
      number_triangles = n;
      refinement_count++;
      std::cout<<"Number of triangles: "<<number_triangles<<
      " triangles over area "<<area_count<<" iteration number "<<refinement_count<<std::endl;
  }
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

  double area_threshold = std::numeric_limits<double>::max();
  std::vector<std::string> args(argv, argv + argc);

  auto it_area_flag = std::find(args.begin(), args.end(), "--area");
  auto it_partitions_flag = std::find(args.begin(), args.end(), "--partitions");

// Parse command-line arguments
  if (it_area_flag != args.end() && std::next(it_area_flag) != args.end())
      area_threshold = std::atof(std::next(it_area_flag)->c_str());

  if (it_partitions_flag != args.end() && std::next(it_partitions_flag) != args.end())
      times_to_repeat = std::atoi(std::next(it_partitions_flag)->c_str());

  
    // if the user wants to refine the triangulation by partitions
    if(area_threshold == std::numeric_limits<double>::max())
      refine_triangulation_times(T, times_to_repeat);

    // if the user wants to refine the triangulation by area
    else if (times_to_repeat == 1)
      refine_triangulation_area(T, area_threshold);

    // if the user wants to refine the triangulation by area and partitions
    else
      refine_triangulation_both(T,times_to_repeat, area_threshold);
    

  std::cerr<<T<<std::endl;
  return( EXIT_SUCCESS );
}

// eof - main.cxx
