#ifndef __LineSegmentIntersection__h__
#define __LineSegmentIntersection__h__

namespace LineSegmentIntersection
{
  template <class _TInIt, class _segment, class _TOutIt>
  void find_intersections(_segment s, _TInIt b, _TInIt e, _TOutIt o)
  {
    using TSegment = typename std::iterator_traits<_TInIt>::value_type;
    using TKernel = typename CGAL::Kernel_traits<TSegment>::Kernel;
    using TPoint = typename TKernel::Point_2;
    int position = 0;
    for (_TInIt i = b; i != e; ++i)
    {
      if (const auto &x = CGAL::intersection(s, *i))
        if (const TPoint *p = boost::get<TPoint>(&*x))
          *o = std::make_pair(*p, position);
      position++;
    }
  }
} // end namespace

// #include "LineSegmentIntersection.hxx"

#endif // __LineSegmentIntersection__h__
