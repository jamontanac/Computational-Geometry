#ifndef __PUJ_CGAL__Triangulation_2__h__
#define __PUJ_CGAL__Triangulation_2__h__

#include <vector>

namespace PUJ_CGAL
{
  /**
   */
  template< class _TKernel >
  class Triangulation_2
  {
  public:
    using Self      = Triangulation_2;
    using TKernel   = _TKernel;
    using TPoint    = typename TKernel::Point_2;
    using TGeometry = std::vector< TPoint >;
    using TTopology = std::vector< bool >;

  public:
    Triangulation_2( );
    Triangulation_2( const Self& other );
    Self& operator=( const Self& other );
    virtual ~Triangulation_2( ) = default;

    TGeometry& geometry( );
    const TGeometry& geometry( ) const;

    template< class _TPoint >
    unsigned long long add_point( const _TPoint& p );

    template< class _TIt >
    void add_points( _TIt b, _TIt e );

    unsigned long long number_of_points( ) const;

    void add_edge(
      const unsigned long long& a, const unsigned long long& b
      );
    void remove_edge(
      const unsigned long long& a, const unsigned long long& b
      );
    bool edge(
      const unsigned long long& a, const unsigned long long& b
      ) const;

    template< class _TIt >
    void build_from_polygon( _TIt b, _TIt e );

  protected:
    template< class _TPolygon >
    bool _build_from_polygon_self_intersection(
      const _TPolygon& P, const long long& a, const long long& b
      ) const;

    template< class _TPolygon >
    void _build_from_polygon_monotone( const _TPolygon& P );

    void _to_stream( std::ostream& out ) const;

  protected:
    TGeometry m_Geometry;
    TTopology m_Topology;

  public:
    friend std::ostream& operator<<( std::ostream& out, const Self& t )
      {
        t._to_stream( out );
        return( out );
      }
  };
} // end namespace

#include <PUJ_CGAL/Triangulation_2.hxx>

#endif // __PUJ_CGAL__Triangulation_2__h__

// eof - Triangulation_2.h
