
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
bool iterateOverPolygon(const TSegments &polygon1,const TSegments &polygon2,bool direction,int &current_segment_polygon,TPolygon &intersection_polygon);
bool contains(const TPolygon &polygon, const TPoint &point);
bool check_inside(TPoint p,const TSegments pgn);
TScalar Area(const TPolygon &polygon);
TScalar percentage_area(const TSegments &polygon1,const TSegments &polygon2,const TPolygon &intersection_polygon);
// bool iterate_over_polygon(const TSegments &polygon1,const TSegments &polygon2,bool direction,int &current_segment_polygon,Polygon &intersection_polygon);
// bool isInside(const TPoint &p, const TSegment &edge);
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

    return 0;
}


/**
 * @brief Type representing a scalar value.
    using TScalar = double;
 */

/**
 * @brief Computes the percentage of the area of the intersection polygon between two polygons.
 * 
 * @param polygon1 The first polygon.
 * @param polygon2 The second polygon.
 * @param intersection_polygon The intersection polygon.
 * @return The percentage of the area of the intersection polygon.
 */
TScalar percentage_area(const TSegments &polygon1, const TSegments &polygon2, const TPolygon &intersection_polygon) {
    TPolygon poly1, poly2;
    for (const auto &seg : polygon1)
        poly1.push_back(seg.source());
    for (const auto &seg : polygon2)
        poly2.push_back(seg.source());
    TScalar area_polygon1 = std::abs(Area(poly1));
    TScalar area_polygon2 = std::abs(Area(poly2));
    TScalar area_intersection = std::abs(Area(intersection_polygon));
    return (area_intersection / (area_polygon1 + area_polygon2 - area_intersection)) * 100;
}
/**
 * Calculates the area of a polygon.
 *
 * @param polygon The polygon for which to calculate the area.
 * @return The area of the polygon, computed using the shoelace formula.
 */
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

/**
 * Iterates over the given polygons and finds the intersection points between them.
 * 
 * @param polygon1 The first polygon.
 * @param polygon2 The second polygon.
 * @param direction The direction of iteration. True for forward, false for backward.
 * @param current_segment_polygon The index of the current segment in polygon1.
 * @param intersection_polygon The resulting polygon containing the intersection points.
 * 
 * @return True if the iteration is finished, false otherwise.
 */
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


/**
 * Checks if a point is inside a polygon.
 * 
 * @param p The point to check.
 * @param pgn The polygon represented as a collection of line segments.
 * @return True if the point is inside the polygon, false otherwise.
 */
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

/**
 * Checks if a given point is contained within a polygon.
 * 
 * @param polygon The polygon to check.
 * @param point The point to check for containment.
 * @return True if the point is contained within the polygon, false otherwise.
 */
bool contains(const TPolygon &polygon, const TPoint &point){
    for(auto vit = polygon.vertices_begin(); vit != polygon.vertices_end(); ++vit){
        if(*vit == point){
            return true;
        }
    }
    return false;
}

// bool isInside(const TPoint &p, const TSegment &edge){
//     auto orient = CGAL::orientation(edge.source(),edge.target(),p);
//     return orient != CGAL::RIGHT_TURN;
// }