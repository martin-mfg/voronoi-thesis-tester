#include <QtGui>
#include "mygraphics.h"
#include <time.h>
//#include "gurobi_c++.h"
//#include "solver/solver.cpp"
//#include "CircleArrangement.cpp"


int main (int argc, char* argv[]){
QApplication app(argc,argv);
MyGraphics g;
g.resize(WIDTH,HEIGHT);
g.show();


/*
GRBEnv env;

vector <vector <double> > M;
vector <double> p1;
vector <double> p2;
vector <double> p3;
vector <double> p4;
vector <double> p5;
vector <double> p6;
vector <double> p7;
p1.push_back(1);
p1.push_back(0);
p1.push_back(0);
p2.push_back(1);
p2.push_back(1);
p2.push_back(0);
p3.push_back(0);
p3.push_back(1);
p3.push_back(0);
p4.push_back(1);
p4.push_back(0);
p4.push_back(1);
p5.push_back(1);
p5.push_back(1);
p5.push_back(1);
p6.push_back(0);
p6.push_back(1);
p6.push_back(1);
p7.push_back(0);
p7.push_back(0);
p7.push_back(1);
M.push_back(p1);
M.push_back(p2);
M.push_back(p3);
M.push_back(p4);
M.push_back(p5);
M.push_back(p6);
M.push_back(p7);
vector <double> sol;
sol = solve(M);
double temp;
//cout << sol.size() << endl;
while (sol.size()){
cout << sol.back() << endl;
sol.pop_CircleArrangement carr;
();
}



backvector<Circle> myVector;

myVector.push_back(Circle(Point(0,0), 5 ));
myVector.push_back(Circle(Point(1,0), 5 ));
myVector.push_back(Circle(Point(0,2), 5 ));

carr.addCircles( myVector );
vector<PointInCircles> result = carr.get_Points();

while(result.size()>0) {
	PointInCircles p = result.back();
	cout <<"x: "<< p.point.x() << "\n";
	cout <<"y: "<< p.point.y() << "\n";
	for(vector<bool>::iterator bit=p.circles.begin(); bit != p.circles.end(); ++bit)
		cout << (*bit) << "\n";
	cout << "\n";
	result.pop_back();
}
*/
srand((unsigned)time(0));
return app.exec();
}
