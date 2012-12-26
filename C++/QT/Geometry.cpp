#include "basicDefinitions.h"
#include "VoronoiDiagram.cpp"
#include <fstream>
#include "CircleArrangement.cpp"

// point "hitbox"
#define HB	6
#define N_POINTS 1000
using namespace std;

class Geometry {
	private:
		double gx[N_POINTS], gy[N_POINTS];	//x- and y-coordinates
		QColor gc[N_POINTS];//color of the point
		int gi;	//Number of points
		vector <ColoredEdge> edges;	//Voronoi-edges
		vector <Circle> circles;	//the red circles that shall be covered
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
		
		void update() {
			calculateCircles();
			calculateEdges();
		}
		
		void remove_blue_points() {
			vector<double> to_delete;
			for(int i = 0; i<numPoints(); ++i) {
				if( getColor(i) == BLUE ) {
					shift(i);
					remove_blue_points();
					break;
				}
			}
		}

	public:
		int current_color;
		int mode;

		Geometry (int bbox_width, int bbox_height) {
			gi=0;
			mode=0;
			current_color=0;
			bounding_box = Rectangle( 0, 0, bbox_width, bbox_height );
			
			
			rounds=1;
		}

		/* GET FUNCTIONS */
		int getNumEdges(){	return edges.size();	}
		
		const Segment * getEdge( int index, QColor & outColor ) {
			ColoredEdge edge = edges.at(index);
			outColor = edge.color;
			return crop( edge );
		}

		int numPoints(){	return gi;	}
		double getX(int i){	return gx[i];	}
		double getY(int i){	return gy[i];	}
		QColor getColor(int i)	{	return gc[i];	}

		vector <Circle> getCircles() {
			//calculateCircles();
			return circles;
		}

		/*
		vector<Circle> allCircles;
		vector <Circle> getDrawCircles() {
			return allCircles;
		}
		*/

		/* SET FUNCTION */
		void addPoint(double x,double y){		//Add point
			bool exists = false;
			for(int i=0;i<gi;i++)	
				if(x>=gx[i]-4 && x <=gx[i]+HB && y>=gy[i]-4 && y<=gy[i]+HB)
					exists=true;
			if (!exists)		//If point doesn't allready exist, add
				if (!current_color){
					gx[gi]=x;gy[gi]=y;gc[gi]=RED;gi++;
				} else {
					gx[gi]=x;gy[gi]=y;gc[gi]=BLUE;gi++;
				}
			
			update();
		}

		void addColoredPoint(double x,double y,QColor c){		//Add point
			bool exists = false;
			for(int i=0;i<gi;i++)	
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					exists=true;
			if (!exists)		//If point doesn't allready exist, add
					gx[gi]=x;gy[gi]=y;gc[gi]=c;gi++;
			
			update();
		}
	
		void clearPoints() {	//"Remove" all points
			gi=0;
			edges.clear();
			circles.clear();
			update();
		}

		// Can remove a point directly, if the index is known
		void remove(int i){
			shift(i);

		}
		// If point removed, shift the array one step
		void remove(double x, double y) {
			for(int i=0;i<gi;i++)
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					shift(i);
			
			update();
		}

		void readFile(const char * filename){
			std::ifstream inFile(filename);
			int x,y,c;

			if(!inFile) {
				std::cerr << "Unable to open file datafile.txt";
			} else {
				clearPoints();		//clear all old points
				while(!inFile.eof()) { 
					inFile >> x;
					if (!inFile.eof())
						inFile >> y;
					else
						break;
					if (!inFile.eof())
						inFile >> c;
					else
						break;					
					if (c == 1)
						addColoredPoint(x,y,RED);
					else if (c==0)
						addColoredPoint(x,y,BLUE);					
				}
			}
		}
		void saveFile(const char * filename){
			std::ofstream outFile;

			outFile.open(filename, ios::trunc);

			if(!outFile) {
				std::cerr << "Unable to open file datafile.txt";
			} else {
				for(int i=0;i<gi;i++) {
					if (gc[i] == RED)
						outFile << gx[i] << " " <<gy[i] << " 1" << endl;
					else
						outFile << gx[i] << " " <<gy[i] << " 0" << endl;
				}
			}
		}
		
		CircleArrangement carr;
		int rounds;
		int solver() {
			int myRounds=0;
			//allCircles=vector<Circle>();
			update();
			//vector<Circle> allCircles_last;
			
			while( circles.size() > 0 ) {
				cout<<carr.getCircles()->size()<<" circles collected\n";
				cout<<circles.size()<<" circles to be inserted\n";
				cout<<numPoints()<<" points\n";
				
				//add circles,
				//calculate candidate blue points
				carr.addCircles( circles );
				vector <vector <double> > coefficients_matrix;
				vector<PointInCircles> result = carr.get_Points();
				vector<PointInCircles> result2 = result; /* I'm popping the first one, and need a backup */
				
				//select blue points
				while(result.size()>0) {
					/* Print the problem gurobi is to solve */
					PointInCircles p = result.back();

					/* Convert from bool vector to double vector */
					vector <double> temp(result.back().circles.begin(),result.back().circles.end());
					coefficients_matrix.push_back(temp);

					result.pop_back();
				}
				reverse(coefficients_matrix.begin(), coefficients_matrix.end());
				vector <double> sol;
				sol = solve(coefficients_matrix);	/* let gurobi solve the problem */

				//allCircles_last=allCircles;

				//add the selected blue points, remove the old ones before
				remove_blue_points();
				while (sol.size()) {
					if (sol.back() > 0) {		/* If gurobi suggests this point as a solution, add it */
						addColoredPoint(result2.back().point.x(),result2.back().point.y(), BLUE);
						cout << "New point. x= " << result2.back().point.x() << "   y= " << result2.back().point.y()<< endl;
					}
					result2.pop_back();
					sol.pop_back();
				}
			
			myRounds++;
			if(myRounds==rounds)
			{rounds+=1;break;}
			
			break;
			}
			return circles.size();
		}
};	
