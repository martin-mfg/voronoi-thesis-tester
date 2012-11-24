#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/enum.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <QtGui>

#define RED Qt::red
#define BLUE QColor(0,136,255)
#define BLACK Qt::black

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
