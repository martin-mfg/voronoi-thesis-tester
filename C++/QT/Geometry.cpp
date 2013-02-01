#include "basicDefinitions.h"
#include "VoronoiDiagram.cpp"
#include <fstream>
#include <sys/time.h>
#include "CircleArrangement.cpp"

// point "hitbox"
#define HB	6
#define N_POINTS 1000
using namespace std;

class Geometry {
	private:
		int numStep1Iterations;
		int numStep2Iterations;
		double gx[N_POINTS], gy[N_POINTS];	//x- and y-coordinates
		QColor gc[N_POINTS];//color of the point
		int gi;	//Number of points
		vector <ColoredEdge> edges;	//Voronoi-edges
		vector <Circle> circles;	//the red circles that shall be covered
		Rectangle bounding_box;
		VoronoiDiagram voronoi;

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
			Converter_to_normal conv;
			
			const CircleArr_Segment * s=CGAL::object_cast<CircleArr_Segment>( & (edge.edge) );
			if (s) { 
				segment_obj = CGAL::intersection( conv( *s ),bounding_box); 
			}
			else {
				const Circle_Ray * r=CGAL::object_cast<Circle_Ray>( & (edge.edge) );
				if (r) { 
					segment_obj = CGAL::intersection( conv( *r ),bounding_box); 
				}
				else { 
					const CircleArr_Line * l = CGAL::object_cast<CircleArr_Line>( & (edge.edge) ); 
					segment_obj = CGAL::intersection( conv( *l ) ,bounding_box); 
				}
			}
			
			const Segment * segment=CGAL::object_cast<Segment>(&segment_obj);
			return segment;
		}
		
		void update() {
			calculateCircles();
			calculateEdges();
		}
	public:
		
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

		int getNS1I () {
			return numStep1Iterations;
		}

		int getNS2I () {
			return numStep2Iterations;
		}

		int current_color;
		int mode;
		CircleArrangement carr;

		Geometry (int bbox_width, int bbox_height) {
			gi=0;
			mode=0;
			current_color=0;
			bounding_box = Rectangle( 0, 0, bbox_width, bbox_height );
		}

		//This function returns true if the number of blue points equal
		//the number of red points
		bool redEqualBlue(){
			int counter = 0;
			for (int i=0; i<gi; i++){
				if (gc[i] == BLUE)
					counter++;
				else
					counter--;
			}
			if (counter)
				return false;
			else
				return true;
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
			VoronoiDiagram empty_voronoi;
			voronoi = empty_voronoi;
			CircleArrangement empty_carr;
			carr = empty_carr;
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

		int numRedPoints(){
			int counter=0;
			for (int i=0;i<numPoints();i++)
				if(getColor(i) == RED)
					counter++;
			return counter;
		}		

		int numBluePoints(){
			int counter=0;
			for (int i=0;i<numPoints();i++)
				if(getColor(i) == BLUE)
					counter++;
			return counter;
		}		

		void readFile(const char * filename){
			std::ifstream inFile(filename);
			double x,y,c;

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
	
		double fRand(double fMin, double fMax) {
		   double f = (double)rand() / RAND_MAX;
   		return fMin + f * (fMax - fMin);
		}

		//v==0 means no print
		double genetics(int verbose,timeval t){
			double minimum = voronoi.objF();
			vector <Point> reds;
			vector <Point> blues;
			VoronoiDiagram v;
			double temp=0;
			long mtime, seconds, useconds;
			timeval s;
			while (minimum){
			numStep2Iterations++;
			gettimeofday(&s,NULL);
			seconds  = s.tv_sec  - t.tv_sec;
			mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
			if (mtime > 300000){
				cout << "Time out" << endl;
				return -1;
			}
				if (minimum == 0){
					return 0;
				}
				for (int j=0;j<gi;j++)	{		
					if (gc[j] == RED)
						reds.push_back(Point(gx[j],gy[j]));
					else if (rand()%2)
						blues.push_back(Point(fRand(-1.5,1.5)+gx[j],fRand(-1.5,1.5)+gy[j]));
					else
						blues.push_back(Point(gx[j],gy[j]));
				}
				v.set_red_points(reds);
				v.set_blue_points(blues);
				v.update();
				temp = v.objF();
				if (temp < minimum){
					for (int k=gi-1;k!=0;k--){
						if (gc[k]==RED){
							reds.pop_back();
							continue;
						} else {
						gx[k]=blues.back().x();
						gy[k]=blues.back().y();
						blues.pop_back();
						}
					}
					voronoi = v;
					minimum = temp;	
					update();		
					if (verbose)
						cout << minimum << endl;
					break;
				}
				reds.clear();
				blues.clear();
			}
			return minimum;
		}

		int solver(int verbose) {
			update();
			vector<PointInCircles>::iterator it;
			while( circles.size() > 0 ) {
				numStep1Iterations++;
				//add circles,
				//calculate candidate blue points
				carr.addCircles( circles );
				vector <vector <double> > coefficients_matrix;
				vector<PointInCircles> result = carr.get_Points();
				
				//select blue points
				for (it=result.begin();it != result.end();it++){
					/* Convert from bool vector to double vector */
					vector <double> temp(it->circles.begin(),it->circles.end());
					coefficients_matrix.push_back(temp);

//					result.pop_back();
				}
				vector <double> sol;
				sol = solve(verbose,coefficients_matrix);	/* Gurobi solver */

				//add the selected blue points, remove the old ones before
				remove_blue_points();
				while (sol.size()) {
					if (sol.back() > 0) {		/* add new points */
						addColoredPoint(result.back().point.x(),
							result.back().point.y(), BLUE);
					if (verbose)
						cout << "New point. x= " << result.back().point.x() 
							<< "   y= " << result.back().point.y()<< endl;
					}
					result.pop_back();
					sol.pop_back();
				}
			break;
			}
			return voronoi.objF();
		}
};	
