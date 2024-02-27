#include <iostream>
#include <set>
#include <vector>
#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_2_algorithms.h>
#include "WavefrontOBJ.h"
#include "LineSegmentIntersection.h"

using TScalar = float;
using TKernel = CGAL::Cartesian<TScalar>;
using TPoint = TKernel::Point_2;
using TSegment = TKernel::Segment_2;
using TSegments = std::vector<TSegment>;
using Polygon = std::set<TPoint>;
using PointPosition = std::pair<TPoint, int>;
using TPolygon = CGAL::Polygon_2<TKernel>;

PointPosition find_nearest_intersection(const TPoint &source,const std::vector<PointPosition> &intersections);
// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool direction,int &current_segment_polygon,Polygon &intersection_polygon);
bool iterateOverPolygon(const TSegments &polygon1,const TSegments &polygon2,bool direction,int &current_segment_polygon,TPolygon &intersection_polygon);
bool isInside(const TPoint &p, const TSegment &edge);
bool contains(const TPolygon &polygon, const TPoint &point);
bool check_inside(TPoint p,const TSegments pgn);
TScalar Area(const TPolygon &polygon);
TScalar percentage_area(const TSegments &polygon1,const TSegments &polygon2,const TPolygon &intersection_polygon);
int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <polygon_1.obj> <polygon_2.obj>" << std::endl;
        return 1;
    }

    TSegments polygon_1, polygon_2;
    WavefrontOBJ::read(argv[1], std::back_inserter(polygon_1));
    WavefrontOBJ::read(argv[2], std::back_inserter(polygon_2));

    TPolygon intersection_polygon;
    bool direction = true;  
    int current_segment_polygon = 0;
    
    bool finished = false;
    while (!finished) {
        finished = iterateOverPolygon(polygon_1, polygon_2,
                                      direction, current_segment_polygon,
                                      intersection_polygon);
        if (!finished)
            direction = !direction;
        finished = iterateOverPolygon(polygon_2, polygon_1,
                                      direction, current_segment_polygon,
                                      intersection_polygon);
    }
    TPolygon Final_result;
    for (auto rit = boost::rbegin(intersection_polygon); rit != boost::rend(intersection_polygon); ++rit){
        if(contains(Final_result,*rit))
            break;
        Final_result.push_back(*rit);

    }

    std::cout << "Percentage of area: " << percentage_area(polygon_1,polygon_2,Final_result) << std::endl;
    // for(auto vit = Final_result.begin(); vit != Final_result.end(); ++vit){
    //     std::cout << *vit << std::endl;
    // }

    return 0;
}


TScalar percentage_area(const TSegments &polygon1, const TSegments &polygon2, const TPolygon &intersection_polygon) {
    TPolygon poly1, poly2;
    for (const auto &seg : polygon1)
        poly1.push_back(seg.source());
    for (const auto &seg : polygon2)
        poly2.push_back(seg.source());
    TScalar area_polygon1 = std::abs(Area(poly1));
    TScalar area_polygon2 = std::abs(Area(poly2));
    TScalar area_intersection = std::abs(Area(intersection_polygon));
    // std::cout << "Area of polygon 1: " << area_polygon1 << std::endl;
    // std::cout << "Area of polygon 2: " << area_polygon2 << std::endl;
    // std::cout << "Area of intersection: " << area_intersection << std::endl;
    // TScalar area_polygon1 = CGAL::polygon_area_2(poly1.vertices_begin(), poly1.vertices_end());
    // TScalar area_polygon2 = CGAL::polygon_area_2(poly2.vertices_begin(), poly2.vertices_end());
    // TScalar area_intersection = CGAL::polygon_area_2(intersection_polygon.vertices_begin(), intersection_polygon.vertices_end());
    return (area_intersection / (area_polygon1 + area_polygon2 - area_intersection)) * 100;
}
TScalar Area(const TPolygon &polygon){
    TScalar area = 0;
    for (auto vit = polygon.vertices_begin(); vit != polygon.vertices_end(); ++vit){
        auto next = vit;
        next++;
        if(next == polygon.vertices_end())
            next = polygon.vertices_begin();
        area += (vit->x() * next->y() - next->x() * vit->y());
    }
    return area*0.5;
}

bool iterateOverPolygon(const TSegments &polygon1,const TSegments &polygon2,bool direction,int &current_segment_polygon,TPolygon &intersection_polygon){
    int increment = direction ? 1 : -1;
    bool finished = true;
    int iterator_polygon = 0;
    TPoint intersection_p;
    while(iterator_polygon < polygon1.size()){
        if (direction && current_segment_polygon == polygon1.size())
            current_segment_polygon = 0;
        else if (!direction && current_segment_polygon == -1)
            current_segment_polygon = polygon1.size() - 1;
        TSegment segment_polygon = polygon1[current_segment_polygon];
        std::vector<PointPosition> all_intersections_with_polygon2;
        LineSegmentIntersection::find_intersections(segment_polygon,
                                                    polygon2.begin(),
                                                    polygon2.end(),
                                                    std::back_inserter(all_intersections_with_polygon2));
        if (!all_intersections_with_polygon2.empty())
        {
            PointPosition nearest_intersection = find_nearest_intersection(segment_polygon.source(), all_intersections_with_polygon2);
            intersection_p = nearest_intersection.first;
            if (!contains(intersection_polygon,intersection_p))
            {  
                if(check_inside(segment_polygon.source(),polygon2)){
                    intersection_polygon.push_back(polygon1[current_segment_polygon].source());
                    intersection_polygon.push_back(intersection_p);
                }
                else if(check_inside(segment_polygon.target(),polygon2)){
                    intersection_polygon.push_back(intersection_p);
                    intersection_polygon.push_back(polygon1[current_segment_polygon].target());
                }
                else
                    intersection_polygon.push_back(intersection_p);
                current_segment_polygon = nearest_intersection.second;
                finished = false;
                break;
            }
            else{
                if(check_inside(segment_polygon.source(),polygon2)){
                    intersection_polygon.push_back(polygon1[current_segment_polygon].source());
                    intersection_polygon.push_back(intersection_p);
                }
                else if(check_inside(segment_polygon.target(),polygon2)){
                    intersection_polygon.push_back(intersection_p);
                    intersection_polygon.push_back(polygon1[current_segment_polygon].target());
                }
                else
                    intersection_polygon.push_back(intersection_p);


                iterator_polygon++;
                current_segment_polygon += increment;
            }
        }
        else{
            iterator_polygon++;
            current_segment_polygon += increment;
        }
    }

    return finished;
}

PointPosition find_nearest_intersection(const TPoint &source, const std::vector<PointPosition> &intersections){
    TScalar min_distance = std::numeric_limits<TScalar>::max(); // set a large value for the minimum distance
    TPoint nearest_intersection;
    int position;
    for (const auto &intersection : intersections)
    {
        TScalar distance = CGAL::squared_distance(source, intersection.first);
        if (distance < min_distance)
        {
            min_distance = distance;
            nearest_intersection = intersection.first;
            position = intersection.second;
        }
    }
    return std::make_pair(nearest_intersection,position);
}

bool isInside(const TPoint &p, const TSegment &edge){
    auto orient = CGAL::orientation(edge.source(),edge.target(),p);
    return orient != CGAL::RIGHT_TURN;
}

bool check_inside(TPoint p,const TSegments pgn){
    TPolygon poly;
    for (const auto &segment : pgn)
        poly.push_back(segment.source());

    switch (CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), p, TKernel())){
    case (CGAL::ON_BOUNDED_SIDE):
        return true;
    case (CGAL::ON_BOUNDARY) :
        return true;
    case CGAL::ON_UNBOUNDED_SIDE:
        return false;
    }
}
//this function should check if a point is belongs to a polygon

bool contains(const TPolygon &polygon, const TPoint &point){
    for(auto vit = polygon.vertices_begin(); vit != polygon.vertices_end(); ++vit){
        if(*vit == point){
            return true;
        }
    }
    return false;
}
// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool &direction,int &current_segment_polygon,Polygon &intersection_polygon){
//     direction = !direction;
//     std::cout << "rotating on direction" << direction<< std::endl;
//     int increment = direction ? 1 : -1;
//     bool finished = true;
//     int iterator_polygon = 0;
//     while(iterator_polygon < polygon1.size()){
//         if (direction && current_segment_polygon == polygon1.size())
//             current_segment_polygon = 0;
//         else if (!direction && current_segment_polygon == -1)
//             current_segment_polygon = polygon1.size() - 1;
//         TSegment segment_polygon = polygon1[current_segment_polygon];
//         std::vector<PointPosition> all_intersections_with_polygon2;
//         LineSegmentIntersection::find_intersections(segment_polygon,
//                                                     polygon2.begin(),
//                                                     polygon2.end(),
//                                                     std::back_inserter(all_intersections_with_polygon2));
//         if (!all_intersections_with_polygon2.empty())
//         {
//             PointPosition nearest_intersection = find_nearest_intersection(segment_polygon.source(), all_intersections_with_polygon2);
//             if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end())
//             {
//                 intersection_polygon.insert(nearest_intersection.first);
//                 direction = !direction;
//                 current_segment_polygon = nearest_intersection.second;
//                 finished = false;
//                 break;
//             }
//             else{
//                 iterator_polygon++;
//                 current_segment_polygon += increment;
//             }
//         }
//         else{
//             iterator_polygon++;
//             current_segment_polygon += increment;
//         }
//     }



//     return finished;
// }

// std::pair<TPoint, int> find_nearest_intersection(const TPoint& source, const std::vector<std::pair<TPoint, int>>& intersections);

// void process_polygon_intersection(const TSegments& polygon_1, const TSegments& polygon_2) {
//     Polygon intersection_polygon;
//     bool direction = true;

//     for (int current_segment_Polygon1 = 0; current_segment_Polygon1 < polygon_1.size(); ++current_segment_Polygon1) {
//         const TSegment& segment_polygon1 = polygon_1[current_segment_Polygon1];
//         std::vector<std::pair<TPoint, int>> all_intersections_with_polygon2;

//         LineSegmentIntersection::find_intersections(segment_polygon1, polygon_2.begin(), polygon_2.end(), std::back_inserter(all_intersections_with_polygon2));

//         if (!all_intersections_with_polygon2.empty()) {
//             const std::pair<TPoint, int>& nearest_intersection = find_nearest_intersection(segment_polygon1.source(), all_intersections_with_polygon2);
//             if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end()) {
//                 intersection_polygon.insert(nearest_intersection.first);
//                 direction = !direction;
//                 current_segment_Polygon1 = nearest_intersection.second;
//             }
//         }
//     }

//     // Print the resulting polygon
//     for (const auto& point : intersection_polygon) {
//         std::cout << point << std::endl;
//     }
// }

// std::pair<TPoint, int> find_nearest_intersection(const TPoint& source, const std::vector<std::pair<TPoint, int>>& intersections) {
//     TScalar min_distance = std::numeric_limits<TScalar>::max();
//     TPoint nearest_intersection;
//     int position;

//     for (const auto& intersection : intersections) {
//         TScalar distance = CGAL::squared_distance(source, intersection.first);
//         if (distance < min_distance) {
//             min_distance = distance;
//             nearest_intersection = intersection.first;
//             position = intersection.second;
//         }
//     }
//     return std::make_pair(nearest_intersection, position);
// }


// #include <iostream>
// #include <set>
// #include <vector>
// #include <CGAL/Cartesian.h>
// #include "WavefrontOBJ.h"
// #include "LineSegmentIntersection.h"

// using TScalar = float;
// using TKernel = CGAL::Cartesian<TScalar>;
// using TPoint = TKernel::Point_2;
// using TSegment = TKernel::Segment_2;
// using TPoints = std::vector<TPoint>;
// using TSegments = std::vector<TSegment>;
// using Polygon = std::set<TPoint>;
// std::pair<TPoint,int> find_nearest_intersection(const TPoint &source, const std::vector<std::pair<TPoint, int>> &intersections);
// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool &direction,int current_segment_polygon,Polygon &intersection_polygon);
// int main(int argc, char **argv)
// {
//     // Read the two polygons from the input files
//     TSegments polygon_1;
//     WavefrontOBJ::read(argv[1], std::back_inserter(polygon_1));
//     TSegments polygon_2;
//     WavefrontOBJ::read(argv[2], std::back_inserter(polygon_2));
//     // Indicator of the direction of the iteration
//     bool direction = true;
//     // Index of the current segment
//     int current_segment_Polygon1 = 0;
//     int current_segment_Polygon2 = 0;
//     int increment = 1;
//     int iterator_polygon1 = 0;
//     int iterator_polygon2 = 0;
//     // resulting polygon
//     Polygon intersection_polygon;
//     while (iterator_polygon1 < polygon_1.size()) // we iterate over the first polygon
//     {
//         if (direction && current_segment_Polygon1 == polygon_1.size())
//             current_segment_Polygon1 = 0;
//         else if (!direction && current_segment_Polygon1 == -1)
//             current_segment_Polygon1 = polygon_1.size() - 1;
//         // Create a segment from the current segment of the first polygon
//         TSegment segment_polygon1 = polygon_1[current_segment_Polygon1];
//         std::vector<std::pair<TPoint, int>> all_intersections_with_polygon2; // vector to store the intersections, starts empty

//         LineSegmentIntersection::find_intersections(segment_polygon1,
//                                                     polygon_2.begin(),
//                                                     polygon_2.end(),
//                                                     std::back_inserter(all_intersections_with_polygon2));

//         if (!all_intersections_with_polygon2.empty())
//         {

//             std::pair<TPoint, int> nearest_intersection = find_nearest_intersection(segment_polygon1.source(), all_intersections_with_polygon2);
//             if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end())
//             {
//                 iterator_polygon1 = 0;
//                 direction = !direction;
//                 intersection_polygon.insert(nearest_intersection.first);
//                 current_segment_Polygon2 = nearest_intersection.second;
//                 increment = direction ? 1 : -1;
//                 while (iterator_polygon2 < polygon_2.size())
//                 {
//                     if (direction && current_segment_Polygon2 == polygon_2.size())
//                         current_segment_Polygon2 = 0;
//                     else if (!direction && current_segment_Polygon2 == -1)
//                         current_segment_Polygon2 = polygon_2.size() - 1;

//                     std::vector<std::pair<TPoint, int>> all_intersections_with_polygon1;
//                     TSegment segment_polygon2 = polygon_2[current_segment_Polygon2];
//                     LineSegmentIntersection::find_intersections(segment_polygon2,
//                                                                 polygon_1.begin(), polygon_1.end(),
//                                                                 std::back_inserter(all_intersections_with_polygon1));
//                     // There is at least one intersection for the first iteration
//                     if (!all_intersections_with_polygon1.empty())
//                     {
//                         std::cout << "Intersection found in, "<<segment_polygon2<<" in position "<<current_segment_Polygon2<<" nearest intersection "<< nearest_intersection.first<< std::endl;
//                         std::pair<TPoint, int> nearest_intersection = find_nearest_intersection(nearest_intersection.first, all_intersections_with_polygon1);
//                         std::cout << "nearest intersection "<< nearest_intersection.first<< std::endl;
//                         if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end())
//                         {
//                             intersection_polygon.insert(nearest_intersection.first);
//                             direction = !direction;
//                             current_segment_Polygon1 = nearest_intersection.second;
//                             iterator_polygon2 = 0;
//                             break;
//                         }
//                         else{
//                             iterator_polygon2++;
//                             std::cout << "Intersection already in the polygon, continue" << std::endl;
//                         }
//                     }
//                     else{
//                         iterator_polygon2++;
//                         std::cout << "No intersection found in, " << segment_polygon2 << " in position " << current_segment_Polygon2 << " ,continue" << std::endl;
//                     }

//                     current_segment_Polygon2 += increment;
//                 }
//             }
//             else{

//                 iterator_polygon1++;
//                 current_segment_Polygon1 += increment;
//                 std::cout << "Intersection already in the polygon, continue" << std::endl;
//             }

//         }
//         else{
//             iterator_polygon1++;
//             current_segment_Polygon1 += increment;
//             std::cout << "No intersection found in, " << segment_polygon1 << " in position " << current_segment_Polygon1 << " ,continue" << std::endl;
//         }
//         // current_segment_Polygon1 += increment;

//     }
//     // print the resulting polygon
//     for (const auto &point : intersection_polygon)
//         std::cout << point << std::endl;


//     return (EXIT_SUCCESS);
// }

// std::pair<TPoint,int> find_nearest_intersection(const TPoint &source, const std::vector<std::pair<TPoint, int>> &intersections){
//     TScalar min_distance = std::numeric_limits<TScalar>::max(); // set a large value for the minimum distance
//     TPoint nearest_intersection;
//     int position;
//     for (const auto &intersection : intersections)
//     {
//         TScalar distance = CGAL::squared_distance(source, intersection.first);
//         if (distance < min_distance)
//         {
//             min_distance = distance;
//             nearest_intersection = intersection.first;
//             position = intersection.second;
//         }
//     }
//     return std::make_pair(nearest_intersection,position);
// }

// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool &direction,int current_segment_polygon,Polygon &intersection_polygon){
//     direction = !direction;
//     int increment = direction ? 1 : -1;
//     bool finished = true;
//     int iterator_polygon = 0;
//     while(iterator_polygon < polygon1.size()){
//         if (direction && current_segment_polygon == polygon1.size())
//             current_segment_polygon = 0;
//         else if (!direction && current_segment_polygon == -1)
//             current_segment_polygon = polygon1.size() - 1;
//         TSegment segment_polygon = polygon1[current_segment_polygon];
//         std::vector<std::pair<TPoint, int>> all_intersections_with_polygon2;
//         LineSegmentIntersection::find_intersections(segment_polygon,
//                                                     polygon2.begin(),
//                                                     polygon2.end(),
//                                                     std::back_inserter(all_intersections_with_polygon2));
//         if (!all_intersections_with_polygon2.empty())
//         {
//             std::pair<TPoint, int> nearest_intersection = find_nearest_intersection(segment_polygon.source(), all_intersections_with_polygon2);
//             if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end())
//             {
//                 intersection_polygon.insert(nearest_intersection.first);
//                 direction = !direction;
//                 current_segment_polygon = nearest_intersection.second;
//                 finished = false;
//                 break;
//             }
//             else{
//                 iterator_polygon++;
//                 current_segment_polygon += increment;
//             }
//         }
//         else{
//             iterator_polygon++;
//             current_segment_polygon += increment;
//         }
//     }



//     return finished;
// }
            //     direction = !direction;
            //     intersection_polygon.insert(nearest_intersection.first);
            //     current_segment_Polygon2 = nearest_intersection.second;
            //     increment = direction ? 1 : -1;
            //     while (iterator_polygon2 < polygon_2.size())
            //     {
            //         if (direction && current_segment_Polygon2 == polygon_2.size())
            //             current_segment_Polygon2 = 0;
            //         else if (!direction && current_segment_Polygon2 == -1)
            //             current_segment_Polygon2 = polygon_2.size() - 1;

            //         std::vector<std::pair<TPoint, int>> all_intersections_with_polygon1;
            //         TSegment segment_polygon2 = polygon_2[current_segment_Polygon2];
            //         LineSegmentIntersection::find_intersections(segment_polygon2,
            //                                                     polygon_1.begin(), polygon_1.end(),
            //                                                     std::back_inserter(all_intersections_with_polygon1));
            //         // There is at least one intersection for the first iteration
            //         if (!all_intersections_with_polygon1.empty())
            //         {
            //             std::cout << "Intersection found in, "<<segment_polygon2<<" in position "<<current_segment_Polygon2<<" nearest intersection "<< nearest_intersection.first<< std::endl;
            //             std::pair<TPoint, int> nearest_intersection = find_nearest_intersection(nearest_intersection.first, all_intersections_with_polygon1);
            //             std::cout << "nearest intersection "<< nearest_intersection.first<< std::endl;
            //             if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end())
            //             {
            //                 intersection_polygon.insert(nearest_intersection.first);
            //                 direction = !direction;
            //                 current_segment_Polygon1 = nearest_intersection.second;
            //                 iterator_polygon2 = 0;
            //                 break;
            //             }
            //             else{
            //                 iterator_polygon2++;
            //                 std::cout << "Intersection already in the polygon, continue" << std::endl;
            //             }
            //         }
            //         else{
            //             iterator_polygon2++;
            //             std::cout << "No intersection found in, " << segment_polygon2 << " in position " << current_segment_Polygon2 << " ,continue" << std::endl;
            //         }

            //         current_segment_Polygon2 += increment;
            //     }
            // }



// #include <iostream>
// #include <set>
// #include <vector>
// #include <CGAL/Cartesian.h>
// #include "WavefrontOBJ.h"
// #include "LineSegmentIntersection.h"

// using TScalar = float;
// using TKernel = CGAL::Cartesian<TScalar>;
// using TPoint = TKernel::Point_2;
// using TSegment = TKernel::Segment_2;
// using TSegments = std::vector<TSegment>;
// using Polygon = std::set<TPoint>;
// using PointPosition = std::pair<TPoint, int>;

// PointPosition find_nearest_intersection(const TPoint &source,const std::vector<PointPosition> &intersections);
// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool &direction,int &current_segment_polygon,Polygon &intersection_polygon);

// int main(int argc, char** argv) {
//     if (argc < 3) {
//         std::cerr << "Usage: " << argv[0] << " <polygon_1.obj> <polygon_2.obj>" << std::endl;
//         return 1;
//     }

//     TSegments polygon_1, polygon_2;
//     WavefrontOBJ::read(argv[1], std::back_inserter(polygon_1));
//     WavefrontOBJ::read(argv[2], std::back_inserter(polygon_2));

//     // process_polygon_intersection(polygon_1, polygon_2);
//     Polygon intersection_polygon;
//     bool direction = true;  
//     int current_segment_polygon = 0;
    
//     std::cout << "current_segment_polygon: " << current_segment_polygon << std::endl;
//     std::cout << "direction: " << direction << std::endl;
//     bool finished = iterate_over_polygon(polygon_1,polygon_2,
//                                         direction,current_segment_polygon,
//                                         intersection_polygon);
//     std::cout << "current_segment_polygon: " << current_segment_polygon << std::endl;
//     std::cout << "direction: " << direction << std::endl;
//     std::cout << "finished: " << finished << std::endl;
//     for(const auto &point : intersection_polygon){
//         std::cout << point << std::endl;
//     }
//     return 0;
// }


// PointPosition find_nearest_intersection(const TPoint &source, const std::vector<PointPosition> &intersections){
//     TScalar min_distance = std::numeric_limits<TScalar>::max(); // set a large value for the minimum distance
//     TPoint nearest_intersection;
//     int position;
//     for (const auto &intersection : intersections)
//     {
//         TScalar distance = CGAL::squared_distance(source, intersection.first);
//         if (distance < min_distance)
//         {
//             min_distance = distance;
//             nearest_intersection = intersection.first;
//             position = intersection.second;
//         }
//     }
//     return std::make_pair(nearest_intersection,position);
// }

// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool &direction,int &current_segment_polygon,Polygon &intersection_polygon){
//     // direction = !direction;
//     std::cout << "rotating on direction" << direction<< std::endl;
//     int increment = direction ? 1 : -1;
//     bool finished = true;
//     int iterator_polygon = 0;
//     while(iterator_polygon < polygon1.size()){
//         if (direction && current_segment_polygon == polygon1.size())
//             current_segment_polygon = 0;
//         else if (!direction && current_segment_polygon == -1)
//             current_segment_polygon = polygon1.size() - 1;
//         TSegment segment_polygon = polygon1[current_segment_polygon];
//         std::vector<PointPosition> all_intersections_with_polygon2;
//         LineSegmentIntersection::find_intersections(segment_polygon,
//                                                     polygon2.begin(),
//                                                     polygon2.end(),
//                                                     std::back_inserter(all_intersections_with_polygon2));
//         if (!all_intersections_with_polygon2.empty())
//         {
//             PointPosition nearest_intersection = find_nearest_intersection(segment_polygon.source(), all_intersections_with_polygon2);
//             if (intersection_polygon.find(nearest_intersection.first) == intersection_polygon.end())
//             {
//                 intersection_polygon.insert(nearest_intersection.first);
//                 direction = !direction;
//                 current_segment_polygon = nearest_intersection.second;
//                 finished = false;
//                 break;
//             }
//             else{
//                 iterator_polygon++;
//                 current_segment_polygon += increment;
//             }
//         }
//         else{
//             iterator_polygon++;
//             current_segment_polygon += increment;
//         }
//     }



//     return finished;
// }

