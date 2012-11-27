#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iterator>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Iso_rectangle_2 Rectangle;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;
typedef K::Line_2 Line;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;
typedef CGAL::Triangle_2<K> 	Triangle; 
typedef CGAL::Circle_2<K> Circle;

//A class to recover Voronoi diagram from stream.
//Rays, lines and segments are cropped to a rectangle
//so that only segments are stored
struct Voronoi{
  std::list<Segment> m_cropped_vd;
  Rectangle m_bbox;

  Voronoi(){}  
  void set_box(const Rectangle &bbox){
	m_bbox = bbox;
}
    template <class RSL>
  void crop_and_extract_segment(const RSL& rsl){
    CGAL::Object obj = CGAL::intersection(rsl,m_bbox);
    const Segment* s=CGAL::object_cast<Segment>(&obj);
    if (s) m_cropped_vd.push_back(*s);
  }
  
  void reset(){
	m_cropped_vd.erase(m_cropped_vd.begin(), m_cropped_vd.end());
}

  void operator<<(const Ray& ray)    { crop_and_extract_segment(ray); }
  void operator<<(const Line& line)  { crop_and_extract_segment(line); }
  void operator<<(const Segment& seg){ crop_and_extract_segment(seg); }
};
