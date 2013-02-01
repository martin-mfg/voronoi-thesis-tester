#include "basicDefinitions.h"
#include <iterator>
#include "ColoredEdge.cpp"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using ::std::vector;
using namespace std;

class VoronoiDiagram{
public:
	vector<CircleArr_Point> red_points;
	vector<CircleArr_Point> blue_points;
	Circle_Triangulation blue_red_t;
	VoronoiDiagram (){
	}

	void update(){ create_triangulations();}

	void set_red_points(vector<Point> p){
	  Converter_to_circle conv;
	  red_points.clear();
	  for(std::vector<Point>::iterator it = p.begin();
			  it != p.end(); ++it) {
	    red_points.push_back( conv( *it ) );
	  }
	}
	void set_blue_points(vector<Point> p){
	  Converter_to_circle conv;
	  blue_points.clear();
	  for(std::vector<Point>::iterator it = p.begin();
			  it != p.end(); ++it) {
	    blue_points.push_back( conv( *it ) );
	  }
	}

/*	void add_red_point(Point p){	red_points.push_back(p); }
	void add_blue_point(Point p){	red_points.push_back(p); }
	vector<Point> get_red_points() {	return red_points; }
	vector<Point> get_blue_points(){	return blue_points; }	
*/	
	void create_triangulations(){
		Circle_Triangulation brt;
		blue_red_t = brt;
		blue_red_t.insert(red_points.begin(), red_points.end());
		blue_red_t.insert(blue_points.begin(), blue_points.end());
	}

/* 	void remove_red_point(Point p){
		red_points.erase(std::remove(red_points.begin(), red_points.end(), p), 				red_points.end());
		blue_red_t.remove(blue_red_t.nearest_vertex(p));
	}

 	void remove_blue_point(Point p){
		blue_points.erase(std::remove(blue_points.begin(), blue_points.end(), p), 				blue_points.end());
		blue_red_t.remove(blue_red_t.nearest_vertex(p));
	}
*/
	QColor getColor( CircleArr_Point p ) {
		for(std::vector<CircleArr_Point>::iterator it = red_points.begin();
				it != red_points.end(); ++it) {
			if( p.x() == it -> x() && p.y() == it -> y() )
				return RED;
		}
		return BLUE;
	}

	vector <ColoredEdge> get_voronoi_edges(){
		Circle_Triangulation::Finite_edges_iterator it;
		vector <ColoredEdge> edges;
		for (it = blue_red_t.finite_edges_begin(); 
				it != blue_red_t.finite_edges_end(); it++) {
			CGAL::Object edge = blue_red_t.dual(it);
			
			CircleArr_Segment delaunay_segment = blue_red_t.segment(*it);
			QColor color1 = getColor( delaunay_segment.point(0) );
			QColor color2 = getColor( delaunay_segment.point(1) );
			
			if(color1 != color2) {
				color1=BLACK;
			}
			
			edges.push_back( ColoredEdge( edge, color1) );
		}
		return edges;
	}

	double objF() {
		Circle_Triangulation::Finite_edges_iterator it;
		double w = 0;
		// iterate over all delaunay-edges...
		for (it = blue_red_t.finite_edges_begin();
				it != blue_red_t.finite_edges_end(); it++) {
			CGAL::Object voronoi_edge = blue_red_t.dual(it);
			CircleArr_Segment delaunay_segment = blue_red_t.segment(*it);
			QColor color1 = getColor( delaunay_segment.point(0) );
			QColor color2 = getColor( delaunay_segment.point(1) );
			//...if the corresponding voronoi edge is red...
			if(color1 == RED && color2 == RED) {
				if (const CircleArr_Segment * s=CGAL::object_cast<CircleArr_Segment>( & (voronoi_edge) )) { 
					w = w + CGAL::to_double( s -> squared_length() );
				} else if(const Circle_Ray * r=CGAL::object_cast<Circle_Ray>( & (voronoi_edge) ) ) {
					w=100000; // Large enough number
				}	else {
					w=-1;
				}
			}
		}
		return w;
	}
	

	vector <Circle> get_new_circles() {
		Circle_Triangulation::Finite_edges_iterator it;
		vector <Circle> circles;
		K::FT squared_radius;
		Converter_to_normal conv;
		// iterate over all delaunay-edges...
		for (it = blue_red_t.finite_edges_begin(); it != blue_red_t.finite_edges_end(); it++) {
			CGAL::Object voronoi_edge = blue_red_t.dual(it);
			CircleArr_Segment delaunay_segment_exact = blue_red_t.segment(*it);
			Segment delaunay_segment = conv( delaunay_segment_exact );
			QColor color1 = getColor( delaunay_segment_exact.point(0) );
			QColor color2 = getColor( delaunay_segment_exact.point(1) );
			Circle new_circle;
			int random = rand();
			//...if the corresponding voronoi edge is red...
			if(color1 == RED && color2 == RED) {
				const Point * center;
				
				//assign to "center" the point on this voronoi edge
				//that is closest to the delaunay edge
				if (const CircleArr_Segment * s_exact=CGAL::object_cast<CircleArr_Segment>( & (voronoi_edge) )) { 
					const Segment s = conv( * s_exact );
					CGAL::Object intersection = CGAL::intersection( delaunay_segment, s );
					const Point * intersection_point =
						CGAL::object_cast<Point>(&intersection);
					if( intersection_point && false) {
						center = intersection_point;
					} else {
				// It seems that the problem was to use a const pointer
				// to a temporary object. So this should work for both
						Point p2;
							p2 = Point( ( s.point(0).x() + s.point(1).x() ) / 2,
								 ( s.point(0).y() + s.point(1).y() ) / 2 );
						squared_radius=
							CGAL::squared_distance( p2, delaunay_segment.point(0) );
						new_circle = Circle( p2, squared_radius );
						circles.push_back( new_circle );
						continue;						
					}
				} else {
					if(const Circle_Ray * r_exact=CGAL::object_cast<Circle_Ray>( & (voronoi_edge) )) {
							Ray r = conv( *r_exact );
							CGAL::Object intersection =
							CGAL::intersection( delaunay_segment, r );
						const Point * intersection_point =
							CGAL::object_cast<Point>(&intersection);
						if( intersection_point && false) {
							center = intersection_point;
						} else {
							random = rand() % 20;
							squared_radius = CGAL::squared_distance
								( r.point(random), delaunay_segment.point(0) );
							new_circle = Circle( r.point(random), squared_radius );
							circles.push_back( new_circle );
							continue;
						}
					} else {
						const CircleArr_Line * l=CGAL::object_cast<CircleArr_Line>( & (voronoi_edge) );
						CGAL::Object intersection =
							CGAL::intersection( delaunay_segment, conv( *l ) );
						const Point * intersection_point =
							CGAL::object_cast<Point>(&intersection);
						center = intersection_point;
					}
				}				
				squared_radius =
					CGAL::squared_distance( * center, delaunay_segment.point(0) );
				new_circle = Circle( * center, squared_radius );
				circles.push_back( new_circle );
			}
		}
		return circles;
	}
};
