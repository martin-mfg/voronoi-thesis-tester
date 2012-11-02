#include "problem.h"

class Problem{
public:
	std::vector<Point> red_points;
	std::vector<Point> blue_points;
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

	/*TODO: Set a bounding box of size min(x)-1, max(x+1)... */
	void set_red_points(std::vector<Point> p){
	red_points=p;
	}
	void set_blue_points(std::vector<Point> p){
		blue_points=p;
	}

	void add_red_point(Point p){red_points.push_back(p);}
	void add_blue_point(Point p){red_points.push_back(p);}
	std::vector<Point> get_red_points() {	return red_points;	}
	std::vector<Point> get_blue_points(){	return blue_points;	}	
	
	void create_triangulations(){
		red_t.insert(red_points.begin(),red_points.end());
		blue_red_t.insert(red_points.begin(),red_points.end());
		blue_red_t.insert(blue_points.begin(),blue_points.end());
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


	/*  PRINTING SECTION */
	void print_voronoi(Voronoi vor, Triangulation tri){		
		vor.reset();
		tri.draw_dual(vor);
		std::copy(vor.m_cropped_vd.begin(),vor.m_cropped_vd.end(),
		std::ostream_iterator<Segment>(std::cout,"\n"));
	}


/* TODO: How do I see which ones are blocked? */
/*	void print_red_neighbours(){
	Triangulation dummy;
	Triangulation::Vertex_iterator it,it2;
	Triangulation::Face_handle f;
	Triangulation::Vertex_handle v1,v2;
	for(it=red_t.vertices_begin(); it != red_t.vertices_end();it++){
		std::cout << std::endl << "Neighbours of (" << *it << ")" << std::endl;
		for(it2=red_t.vertices_begin(); it2 != red_t.vertices_end();it2++){
			v1 =blue_red_t.insert(it.point(),f);
			if (blue_red_t.is_edge(it,it2))
				std::cout << " - (" << *it2 << ")";
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
			std::cout << "Circle center located at: " << p << " and with radius: "
				  << squared_distance(p, tri.vertex(0)) << std::endl;
		}
	}

	void print_triangles(Triangulation dt){ 
		Point p;
		Triangle tri;
		Triangulation::Finite_faces_iterator it;
		for (it = dt.finite_faces_begin(); it != dt.finite_faces_end(); it++) {
			tri = dt.triangle(it);
			std::cout << "Triangle: " << tri << std::endl;
		}
	}	

	void print_all(){
		std::cout << "Triangles in red: " << std::endl;
		print_triangles(red_t);
		std::cout << std::endl << "Triangles in blue_red: " << std::endl;
		print_triangles(blue_red_t);
		std::cout << std::endl << "Circles in red: " << std::endl;
		print_circles(red_t);
		std::cout << std::endl << "Circles in blue_red: " << std::endl;
		print_circles(blue_red_t);

		std::cout << std::endl << "Voronoi diagram for red: " << std::endl;
		print_voronoi(red_vor,red_t);
		std::cout << std::endl << "Voronoi diagram for blue_red: " << std::endl;
		print_voronoi(blue_red_vor, blue_red_t);

//		print_red_neighbours();

		std::cout << std::endl << std::endl << std::endl << std::endl;
	}
};


int main(){

	std::vector <Point> rp, bp;
	Problem prob;
	prob.print_all();

	rp.push_back(Point(0,0));
	rp.push_back(Point(2,2));
	rp.push_back(Point(0,2));
	prob.set_red_points(rp);
	prob.print_all();

	prob.update();
	prob.print_all();

	bp.push_back(Point(0,1));
	bp.push_back(Point(1,1));
	bp.push_back(Point(1,2));
	prob.set_blue_points(bp);	
	prob.print_all();

	prob.update();
	prob.print_all();




return 0;
}
