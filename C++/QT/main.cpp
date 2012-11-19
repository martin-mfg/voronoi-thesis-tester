#include <QtGui>
#include "mygraphics.h"
#include "gurobi_c++.h"
#include "solver/solver.cpp"
//#include "problem.h"


int main (int argc, char* argv[]){
QApplication app(argc,argv);
MyGraphics g;
g.resize(WIDTH,HEIGHT);
g.show();
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
	sol.pop_back();
}
return app.exec();
}
