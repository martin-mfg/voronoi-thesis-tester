#include <QtGui>
#include "mygraphics.h"
#include <time.h>
//#include "gurobi_c++.h"
//#include "solver/solver.cpp"
//#include "CircleArrangement.cpp"


int main (int argc, char* argv[]){
QApplication app(argc,argv);
MyGraphics g(argv[1]);
g.resize(WIDTH,HEIGHT);
g.show();


srand((unsigned)time(0));
return app.exec();
}
