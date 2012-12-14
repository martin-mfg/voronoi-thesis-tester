#include "basicDefinitions.h"
#include <iterator>
#include "ColoredEdge.cpp"

using ::std::vector;
using namespace std;

class VoronoiDiagram{
public:
	vector<Point> red_points;
	vector<Point> blue_points;
	Triangulation blue_red_t;

	VoronoiDiagram (){
	}

	void update(){ create_triangulations();}

	void set_red_points(vector<Point> p){	red_points=p; }
	void set_blue_points(vector<Point> p){	blue_points=p; }

	void add_red_point(Point p){	red_points.push_back(p); }
	void add_blue_point(Point p){	red_points.push_back(p); }
	vector<Point> get_red_points() {	return red_points; }
	vector<Point> get_blue_points(){	return blue_points; }	
	
	void create_triangulations(){
		blue_red_t.insert(red_points.begin(), red_points.end());
		blue_red_t.insert(blue_points.begin(), blue_points.end());
	}

 	void remove_red_point(Point p){
        	red_points.erase(std::remove(red_points.begin(), red_points.end(), p), red_points.end());
		blue_red_t.remove(blue_red_t.nearest_vertex(p));
	}

 	void remove_blue_point(Point p){
        	blue_points.erase(std::remove(blue_points.begin(), blue_points.end(), p), blue_points.end());
		blue_red_t.remove(blue_red_t.nearest_vertex(p));
	}

	QColor getColor( Point p ) {
		for(std::vector<Point>::iterator it = red_points.begin(); it != red_points.end(); ++it) {
			if( p.x() == it -> x() && p.y() == it -> y() ) {
				return RED;
			}
		}
		return BLUE;
	}

	vector <ColoredEdge> get_voronoi_edges(){
		Triangulation::Finite_edges_iterator it;
		vector <ColoredEdge> edges;
		Point source, target, delaunay_source, delaunay_target;
		for (it = blue_red_t.finite_edges_begin(); it != blue_red_t.finite_edges_end(); it++) {
			CGAL::Object edge = blue_red_t.dual(it);
			
			Segment delaunay_segment = blue_red_t.segment(*it);
			QColor color1 = getColor( delaunay_segment.point(0) );
			QColor color2 = getColor( delaunay_segment.point(1) );
			
			if(color1 != color2) {
				color1=BLACK;
			}
			
			edges.push_back( ColoredEdge( edge, color1) );
		}
		return edges;
	}
	
	vector <Circle> get_new_circles() {
		Triangulation::Finite_edges_iterator it;
		vector <Circle> circles;
		Point source, target, delaunay_source, delaunay_target;
		// iterate over all delaunay-edges...
		for (it = blue_red_t.finite_edges_begin(); it != blue_red_t.finite_edges_end(); it++) {
			CGAL::Object voronoi_edge = blue_red_t.dual(it);
			Segment delaunay_segment = blue_red_t.segment(*it);
			QColor color1 = getColor( delaunay_segment.point(0) );
			QColor color2 = getColor( delaunay_segment.point(1) );
			
			//...if the corresponding voronoi edge is red...
			if(color1 == RED && color2 == RED) {
				const Point * center;
				
				//assign to "center" the point on this voronoi edge that is closest to the delaunay edge
				const Segment * s=CGAL::object_cast<Segment>( & (voronoi_edge) );
				if (s) { 
					CGAL::Object intersection = CGAL::intersection( delaunay_segment, *s );
					const Point * intersection_point = CGAL::object_cast<Point>(&intersection);
					if( intersection_point ) {
						center = intersection_point;
					} else {
				// It seems that the problem was to use a const pointer
				// to a temporary object. So this should work for both
				Point p2 = Point( ( s->point(0).x() + s->point(1).x() ) / 2, ( s->point(0).y() + s->point(1).y() ) / 2 );
				K::FT squared_radius = CGAL::squared_distance( p2, delaunay_segment.point(0) );
				Circle new_circle = Circle( p2, squared_radius );
				circles.push_back( new_circle );
				continue;						
					}
				} else {
					const Ray * r=CGAL::object_cast<Ray>( & (voronoi_edge) );
					if(r) {
						CGAL::Object intersection = CGAL::intersection( delaunay_segment, *r );
						const Point * intersection_point = CGAL::object_cast<Point>(&intersection);
						if( intersection_point ) {
							center = intersection_point;
						} else {
							//TODO: choose bigger number for big, maybe dynamically according to WIDTH or HEIGHT
				K::FT squared_radius = CGAL::squared_distance( r->point(10), delaunay_segment.point(0) );
				Circle new_circle = Circle( r->point(10), squared_radius );
				circles.push_back( new_circle );
continue;
						}
					} else {
						const Line * l=CGAL::object_cast<Line>( & (voronoi_edge) );
						CGAL::Object intersection = CGAL::intersection( delaunay_segment, *l );
						const Point * intersection_point = CGAL::object_cast<Point>(&intersection);
						center = intersection_point;
					}
				}
				
				//add a circle to "circles" with center "center" and going through the delaunay-edges' endpoints
				K::FT squared_radius = CGAL::squared_distance( * center, delaunay_segment.point(0) );
				Circle new_circle = Circle( * center, squared_radius );
				circles.push_back( new_circle );
			}
		}
		return circles;
	}



};
