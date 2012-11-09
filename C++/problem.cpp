#include "problem.h"

using ::std::cout;
using ::std::vector;
using ::std::endl;


class Problem{
public:
	vector<Point> red_points;
	vector<Point> blue_points;
	Rectangle bbox;
	Triangulation red_t;
	Triangulation blue_red_t;
	Voronoi red_vor,blue_red_vor;

	Problem (){
		Rectangle box(-10,-10,10,10);
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

	vector <double> get_voronoi_edges(){		
		red_t.draw_dual(red_vor);
		vector <double> edge_points;

		while(red_vor.m_cropped_vd.size()){
			edge_points.push_back(red_vor.m_cropped_vd.back().source().x());
			edge_points.push_back(red_vor.m_cropped_vd.back().source().y());
			edge_points.push_back(red_vor.m_cropped_vd.back().target().x());
			edge_points.push_back(red_vor.m_cropped_vd.back().target().y());
			edge_points.push_back(0.0);//Color
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

double * computeVoronoiEdges(double bBoxParams[], double points[]){
	vector <Point> red_points;
	vector <double> seg_points;

	Problem prob;
	Point bleft = Point(bBoxParams[0], bBoxParams[1]);
	Point tright = Point(bBoxParams[2], bBoxParams[3]);

	for (int i=0;i<9;i=i+3)
		red_points.push_back(Point(points[i], points[i+1]));

	prob.set_red_points(red_points);
	prob.set_bbox(bleft, tright);
	prob.update();

	seg_points = prob.get_voronoi_edges();
	int s = seg_points.size();
	double ps[s];
	for (int i=0;i<s;i++)
		ps[i] =seg_points[i];
	return ps;
}

int main(){
/*
	vector <Point> rp, bp;
	Problem prob;

	rp.push_back(Point(0,0));
	rp.push_back(Point(2,2));
	rp.push_back(Point(0,2));
	prob.set_red_points(rp);
	prob.update();
	prob.print_all();
*/	

	double testBBox[] = {-2.0,-2.0,2.0,2.0};
	double points[] = {1.0,1.0,1.0,0.0,0.0,0.0,1.5,1.5,0.0};
	cout << computeVoronoiEdges(testBBox,points)[5] << endl;

return 0;
}
