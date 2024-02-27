#include <CGAL/basic.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Partition_traits_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <cassert>
#include <list>
#include <stack>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Partition_traits_2<K>                         Traits;
typedef Traits::Point_2                                     Point_2;
typedef Traits::Polygon_2                                   Polygon_2;
typedef std::list<Polygon_2>                                Polygon_list;
typedef CGAL::Creator_uniform_2<int, Point_2>               Creator;
typedef CGAL::Random_points_in_square_2<Point_2, Creator>   Point_generator;

void make_polygon(Polygon_2& polygon)
{
   polygon.push_back(Point_2(391, 374));
   polygon.push_back(Point_2(240, 431));
   polygon.push_back(Point_2(252, 340));
   polygon.push_back(Point_2(374, 320));
   polygon.push_back(Point_2(289, 214));
   polygon.push_back(Point_2(134, 390));
   polygon.push_back(Point_2( 68, 186));
   polygon.push_back(Point_2(154, 259));
   polygon.push_back(Point_2(161, 107));
   polygon.push_back(Point_2(435, 108));
   polygon.push_back(Point_2(208, 148));
   polygon.push_back(Point_2(295, 160));
   polygon.push_back(Point_2(421, 212));
   polygon.push_back(Point_2(441, 303));
}
template <class _TInIt>
void triangularize(_TInIt b ,_TInIt e){
    unsigned long long n = std::distance(b, e);
    // std::cout << "Triangulating a polygon with " << n << " vertices." << std::endl;
    std::vector<bool> chain (n, false);
    auto M = *b;
    auto m = *e;
    unsigned long long iM = 0, im = 0;
    unsigned long long idx = 0;
    for (auto it = b; it != e; it++, idx++){
        if (M[1]>(*it)[1] ){
            M = *it;
            iM = idx;
        }
        if (m[1]<(*it)[1]){
            m = *it;
            im = idx;
        }
    }
    // std::cout << "M: " << M << " m: " << m << std::endl;
    // std::cout << "iM: " << iM << " im: " << im << std::endl;
    for(idx = im; idx != iM; idx = (idx + 1) % n){
        chain[idx] = true;
    }
    std::vector<std::pair< decltype(M), bool> > U;
    for(auto it = b; it != e; it++, idx++){
        U.push_back(std::make_pair(*it, chain[idx]));
    }
    std::sort(U.begin(), U.end(), [](const std::pair< decltype(M), bool> &a, const std::pair< decltype(M), bool> &b)->bool{
        if (a.first[1] == b.first[1]){
            return a.first[0] < b.first[0];
        }
        return a.first[1] < b.first[1];
    });
    std::stack<std::pair< decltype(M), bool>> S;
    S.push(U[0]);
    S.push(U[1]);
    for(idx = 2; idx < U.size(); idx++){
        auto u = U[idx];
        if (u.second != S.top().second){
            while(!S.empty()){
                S.pop();
            }
            S.push(U[idx-1]);
            S.push(U[idx]);
        }else{
            auto v = S.top();
            S.pop();
            while(!S.empty()){
                auto w = S.top();
                S.pop();
                std::cout << "Triangle: " << v.first << " " << u.first << " " << w.first << std::endl;
                v = w;
            }
            S.push(v);
            S.push(u);
        }
    }
}
int main( )
{
   Polygon_2    polygon;
   Polygon_list partition_polys;

/*
   CGAL::random_polygon_2(50, std::back_inserter(polygon), 
                          Point_generator(100));
*/
   make_polygon(polygon);
   CGAL::y_monotone_partition_2(polygon.vertices_begin(), 
                                polygon.vertices_end(),
                                std::back_inserter(partition_polys));

   std::list<Polygon_2>::const_iterator   poly_it;
   for (poly_it = partition_polys.begin(); poly_it != partition_polys.end();
        poly_it++)
   {
    triangularize((*poly_it).vertices_begin(),(*poly_it).vertices_end());
   }

    // std::cout << "The polygon has been decomposed into " << partition_polys.size() << " y-monotone polygons." << std::endl;
   return 0;
}