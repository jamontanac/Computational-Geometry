#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <vector>
#include <list>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
bool isInside(const Point_2& p, const Segment_2& edge) {
    auto orient = CGAL::orientation(edge.source(), edge.target(), p);
    return orient != CGAL::RIGHT_TURN; // Inside or on the edge
}

boost::optional<Point_2> lineIntersection(const Segment_2& s1, const Segment_2& s2) {
    auto result = CGAL::intersection(s1, s2);
    if (const Point_2* p = boost::get<Point_2>(&*result)) {
        return *p; // Return the intersection point
    }
    return {}; // No intersection
}
Polygon_2 polygonIntersection(const Polygon_2& clipPolygon, const Polygon_2& subjectPolygon) {
    Polygon_2 result = subjectPolygon;

    for (auto clipEdgeIt = clipPolygon.edges_begin(); clipEdgeIt != clipPolygon.edges_end(); ++clipEdgeIt) {
        Polygon_2 inputPolygon = result;
        result.clear();
        Point_2 S = *(inputPolygon.vertices_begin() + (inputPolygon.size() - 1));

        for (auto edgeIt = inputPolygon.edges_begin(); edgeIt != inputPolygon.edges_end(); ++edgeIt) {
            Point_2 E = edgeIt->target();
            if (isInside(E, *clipEdgeIt)) {
                if (!isInside(S, *clipEdgeIt)) {
                    auto intPoint = lineIntersection(Segment_2(S, E), *clipEdgeIt);
                    if (intPoint) result.push_back(*intPoint);
                }
                result.push_back(E);
            } else if (isInside(S, *clipEdgeIt)) {
                auto intPoint = lineIntersection(Segment_2(S, E), *clipEdgeIt);
                if (intPoint) result.push_back(*intPoint);
            }
            S = E;
        }
    }

    return result;
}
int main() {
    // Example polygons
    Polygon_2 clipPolygon, subjectPolygon;
    clipPolygon.push_back(Point_2(1, 1));
    clipPolygon.push_back(Point_2(5, 1));
    clipPolygon.push_back(Point_2(5, 5));
    clipPolygon.push_back(Point_2(1, 5));

    subjectPolygon.push_back(Point_2(2, 2));
    subjectPolygon.push_back(Point_2(6, 2));
    subjectPolygon.push_back(Point_2(6, 6));
    subjectPolygon.push_back(Point_2(2, 6));

    // Compute intersection
    Polygon_2 intersectionPolygon = polygonIntersection(clipPolygon, subjectPolygon);

    // Output the result
    for (auto vit = intersectionPolygon.vertices_begin(); vit != intersectionPolygon.vertices_end(); ++vit) {
        std::cout << "(" << CGAL::to_double(vit->x()) << ", " << CGAL::to_double(vit->y()) << ")" << std::endl;
    }

    return 0;
}

// #define CGAL_USE_BASIC_VIEWER
// #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Polygon_2.h>
// #include <CGAL/draw_polygon_2.h>
// typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// typedef CGAL::Polygon_2<K>                                  Polygon_2;
// typedef CGAL::Point_2<K>                                    Point;
// int main()
// {
//   // create a polygon and put some points in it
//   Polygon_2 p;
//   p.push_back(Point(0,0));
//   p.push_back(Point(4,0));
//   p.push_back(Point(4,4));
//   p.push_back(Point(2,2));
//   p.push_back(Point(0,4));
//   CGAL::draw(p);
//   return EXIT_SUCCESS;
// }
// #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
// #include <CGAL/Polygon_2_algorithms.h>
// #include <iostream>
// typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// typedef K::Point_2 Point;
// using std::cout; using std::endl;
// void check_inside(Point pt, Point *pgn_begin, Point *pgn_end, K traits)
// {
//   cout << "The point " << pt;
//   switch(CGAL::bounded_side_2(pgn_begin, pgn_end,pt, traits)) {
//     case CGAL::ON_BOUNDED_SIDE :
//       cout << " is inside the polygon.\n";
//       break;
//     case CGAL::ON_BOUNDARY:
//       cout << " is on the polygon boundary.\n";
//       break;
//     case CGAL::ON_UNBOUNDED_SIDE:
//       cout << " is outside the polygon.\n";
//       break;
//   }
// }
// int main()
// {
//   Point points[] = { Point(0,0), Point(5.1,0), Point(1,1), Point(0.5,6)};
//   // check if the polygon is simple.
//   cout << "The polygon is "
//     << (CGAL::is_simple_2(points, points+4, K()) ? "" : "not ")
//     << "simple." << endl;
//   check_inside(Point(0.5, 0.5), points, points+4, K());
//   check_inside(Point(1.5, 2.5), points, points+4, K());
//   check_inside(Point(2.5, 0), points, points+4, K());
//   return 0;
// }
// // #include <iostream>
// // #include <vector>

// // int main() {
// //     std::vector<int> vec = {1, 2, 3, 4, 5};

// //     // Define the position from which you want to start iterating backward
// //     int start_position = 2;
// //     int position = start_position;
// //     int iterator = 0;
// //     // Iterate backward from the start_position to the beginning of the vector
// //     while(iterator < vec.size()) {
// //         if (position == -1)
// //         {
// //             position = vec.size() - 1;
// //             /* code */
// //         }
// //         std::cout << vec[position] << " ";
// //         iterator ++;
// //         position--;
// //     }
        
// //     for (auto it = vec.rbegin() + std::distance(vec.rbegin()+vec.size() - start_position); it != vec.rend(); ++it) {
// //         std::cout << *it << " ";
// //     }
// //     // Iterate forward from the beginning of the vector until the start_position
// //     for (auto it = vec.begin()+ start_position; it != vec.end() ; ++it) {
// //         std::cout << *it << " "<< std::endl;
// //     }


// //     return 0;
// // }