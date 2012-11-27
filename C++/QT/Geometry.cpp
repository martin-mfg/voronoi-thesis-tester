#include "basicDefinitions.h"
#include "VoronoiDiagram.cpp"
//#include "gurobi_c++.h"
#include <fstream>
#include "CircleArrangement.cpp"
#define N_POINTS 1000
// point "hitbox"
#define HB	10

using namespace std;

class Geometry {
	private:
		int gx[N_POINTS], gy[N_POINTS];	//x- and y-coordinates
		QColor gc[N_POINTS];//color of the point
		int gi;	//Number of points
		vector <ColoredEdge> edges;	//Voronoi-edges
		vector <Circle> circles;	//Voronoi-edges
		Rectangle bounding_box;

		void shift(int i){
			if(i==gi-1)
				gi--;
			else {
				gx[i]=gx[i+1];
				gy[i]=gy[i+1];
				gc[i]=gc[i+1];
				shift(i+1);	//recursive call
			}
		}
	
		void calculateEdges() {
			vector <Point> red_points;
			vector <Point> blue_points;

			VoronoiDiagram voronoi;	//create a VoronoiDiagram object

			for (int i=0;i<gi;i++) {
				if( getColor(i) == RED ) {
					red_points.push_back(Point(gx[i], gy[i]));
				} else {
					blue_points.push_back(Point(gx[i], gy[i]));
				}
			}

			voronoi.set_red_points(red_points);//add red points to the diagram
			voronoi.set_blue_points(blue_points);//add blue points to the diagram
			voronoi.update();
			edges = voronoi.get_voronoi_edges();
		}

		void calculateCircles() {
			vector <Point> red_points;
			vector <Point> blue_points;

			VoronoiDiagram voronoi;	//create a VoronoiDiagram object
			for (int i=0;i<gi;i++) {
				if( getColor(i) == RED ) {
					red_points.push_back(Point(gx[i], gy[i]));
				} else {
					blue_points.push_back(Point(gx[i], gy[i]));
				}
			}

			voronoi.set_red_points(red_points);//add red points to the diagram
			voronoi.set_blue_points(blue_points);//add blue points to the diagram
			voronoi.update();
			circles = voronoi.get_new_circles();
		}

		// crop returns NULL if the edge is completely outside the bounding box!
		const Segment * crop( ColoredEdge edge ) {
			CGAL::Object segment_obj;
			
			const Segment * s=CGAL::object_cast<Segment>( & (edge.edge) );
			if (s) { 
				segment_obj = CGAL::intersection(*s,bounding_box); 
			}
			else {
				const Ray * r=CGAL::object_cast<Ray>( & (edge.edge) );
				if (r) { 
					segment_obj = CGAL::intersection(*r,bounding_box); 
				}
				else { 
					const Line * l = CGAL::object_cast<Line>( & (edge.edge) ); 
					segment_obj = CGAL::intersection( *l ,bounding_box); 
				}
			}
			
			const Segment * segment=CGAL::object_cast<Segment>(&segment_obj);
			return segment;
		}

	public:
		int current_color;
		int mode;

		Geometry (int bbox_width, int bbox_height) {
			gi=0;
			mode=0;
			current_color=0;
			bounding_box = Rectangle( 0, 0, bbox_width, bbox_height );
		}

		/* GET FUNCTIONS */
		int getNumEdges(){	return edges.size();	}
		
		const Segment * getEdge( int index, QColor & outColor ) {
			ColoredEdge edge = edges.at(index);
			outColor = edge.color;
			return crop( edge );
		}

		int numPoints(){	return gi;	}
		int getX(int i){	return gx[i];	}
		int getY(int i){	return gy[i];	}
		QColor getColor(int i)	{	return gc[i];	}

		vector <Circle> getCircles() {
			calculateCircles();
			return circles;
		}

		/* SET FUNCTION */
		void addPoint(int x,int y){		//Add point
			bool exists = false;
			for(int i=0;i<gi;i++)	
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					exists=true;
			if (!exists)		//If point doesn't allready exist, add
				if (!current_color){
					gx[gi]=x;gy[gi]=y;gc[gi]=RED;gi++;
				} else {
					gx[gi]=x;gy[gi]=y;gc[gi]=BLUE;gi++;
				}
			
			calculateEdges();
		}
	
		void clearPoints() {	//"Remove" all points
			gi=0;
			
			calculateEdges();
		}

		// If point removed, shift the array one step
		void remove(int x, int y) {
			for(int i=0;i<gi;i++)
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					shift(i);
		
		calculateEdges();
		}

		void readFile(const char * filename){
			std::ifstream inFile(filename);
			double x,y,c;

			clearPoints();		//clear all old points

			if(!inFile) {
				std::cerr << "Unable to open file datafile.txt";
			} else {
				while(!inFile.eof()) { 
					inFile >> x >> y;
					addPoint(x,y);
				}
			}
		}
		void saveFile(const char * filename){
			std::ofstream outFile;
			double x,y,c;

			outFile.open(filename, ios::trunc);

			if(!outFile) {
				std::cerr << "Unable to open file datafile.txt";
			} else {
				for(int i=0;i<gi;i++) {
					outFile << gx[i] << " " <<gy[i] << endl;
				}
			}
		}
		
		void solver() {
			CircleArrangement carr;
			carr.addCircles( circles );
			//vector<PointInCircles> points = carr.get_Points();
		}
};	
