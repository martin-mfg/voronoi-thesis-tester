//#include "problem.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iterator>


using ::std::cout;
using ::std::vector;
using ::std::endl;
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

//double * computeVoronoiEdges(double bBoxParams[], double points[]);
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

class Problem{
public:
	vector<Point> red_points;
	vector<Point> blue_points;
	Rectangle bbox;
	Triangulation red_t;
	Triangulation blue_red_t;
	Voronoi red_vor,blue_red_vor;

	Problem (){
		Rectangle box(-1000.0,-1000.0,1000.0,1000.0);
		red_vor.set_box(box);
		blue_red_vor.set_box(box);
	}

	void update(){ create_triangulations();}

	void set_red_points(vector<Point> p){	red_points=p; }
	void set_blue_points(vector<Point> p){	blue_points=p; }
	void set_bbox(Point bleft, Point tright){	bbox = Rectangle(bleft,tright); }

	void add_red_point(Point p){	red_points.push_back(p); }
	void add_blue_point(Point p){	red_points.push_back(p); }
	vector<Point> get_red_points() {	return red_points; }
	vector<Point> get_blue_points(){	return blue_points; }	
	
	void create_triangulations(){
		red_t.insert(red_points.begin(), red_points.end());
		blue_red_t.insert(red_points.begin(), red_points.end());
		blue_red_t.insert(blue_points.begin(), blue_points.end());
	}

 	void remove_red_point(Point p){
        	red_points.erase(std::remove(red_points.begin(), red_points.end(), p), red_points.end());
		red_t.remove(red_t.nearest_vertex(p));
		blue_red_t.remove(blue_red_t.nearest_vertex(p));
	}

 	void remove_blue_point(Point p){
        	blue_points.erase(std::remove(blue_points.begin(), blue_points.end(), p), blue_points.end());
		blue_red_t.remove(blue_red_t.nearest_vertex(p));
	}

	vector <Point> get_voronoi_edges(){		
		red_t.draw_dual(red_vor);
		vector <Point> edge_points;
		while(red_vor.m_cropped_vd.size()){
			edge_points.push_back(red_vor.m_cropped_vd.back().source());
			edge_points.push_back(red_vor.m_cropped_vd.back().target());
			red_vor.m_cropped_vd.pop_back();
		}
		return edge_points;
	}


	/*  PRINTING SECTION */
	void print_voronoi(Voronoi vor, Triangulation tri){		
		vor.reset();
		tri.draw_dual(vor);
		std::copy(vor.m_cropped_vd.begin(),vor.m_cropped_vd.end(),
		std::ostream_iterator<Segment>(cout,"\n"));
	}


/* TODO: How do I see which ones are blocked? */
/*	void print_red_neighbours(){
	Triangulation dummy;
	Triangulation::Vertex_iterator it,it2;
	Triangulation::Face_handle f;
	Triangulation::Vertex_handle v1,v2;
	for(it=red_t.vertices_begin(); it != red_t.vertices_end();it++){
		cout << endl << "Neighbours of (" << *it << ")" << endl;
		for(it2=red_t.vertices_begin(); it2 != red_t.vertices_end();it2++){
			v1 =blue_red_t.insert(it.point(),f);
			if (blue_red_t.is_edge(it,it2))
				cout << " - (" << *it2 << ")";
		}
	}
	}
*/

	void print_circles(Triangulation dt){ 
		Point p;
		Triangle tri;
		Triangulation::Finite_faces_iterator it;
		for (it = dt.finite_faces_begin(); it != dt.finite_faces_end(); it++) {
			p=dt.circumcenter(it);
			tri = dt.triangle(it);
			cout << "Circle center located at: " << p << " and with radius: "
				  << squared_distance(p, tri.vertex(0)) << endl;
		}
	}

	void print_triangles(Triangulation dt){ 
		Point p;
		Triangle tri;
		Triangulation::Finite_faces_iterator it;
		for (it = dt.finite_faces_begin(); it != dt.finite_faces_end(); it++) {
			tri = dt.triangle(it);
			cout << "Triangle: " << tri << endl;
		}
	}	

	void print_all(){
		cout << "Triangles in red: " << endl;
		print_triangles(red_t);
		cout << endl << "Triangles in blue_red: " << endl;
		print_triangles(blue_red_t);
		cout << endl << "Circles in red: " << endl;
		print_circles(red_t);
		cout << endl << "Circles in blue_red: " << endl;
		print_circles(blue_red_t);

		cout << endl << "Voronoi diagram for red: " << endl;
		print_voronoi(red_vor,red_t);
		cout << endl << "Voronoi diagram for blue_red: " << endl;
		print_voronoi(blue_red_vor, blue_red_t);
		cout << endl << endl;
	}
};

/*
int main(){

	vector <Point> rp, bp;
	Problem prob;

	rp.push_back(Point(0,0));
	rp.push_back(Point(2,2));
	rp.push_back(Point(0,2));
	prob.set_red_points(rp);
	prob.update();
	prob.print_all();

	double testBBox[] = {-2.0,-2.0,2.0,2.0};
	double points[] = {1.0,1.0,1.0,0.0,0.0,0.0,1.5,1.5,0.0};
	cout << computeVoronoiEdges(testBBox,points)[5] << endl;

//return 0;
}
*/
