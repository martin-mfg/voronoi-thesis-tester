#include <QtGui>
#include "mygraphics.h"
//#include "problem.h"

int main (int argc, char* argv[]){
QApplication app(argc,argv);
MyGraphics g;
g.resize(500,400);
g.show();

double testBBox[] = {0,0,500,400};
double points[] = {1.0,1.0,1.0,0.0,0.0,0.0,1.5,1.5,0.0};
double*p1 = testBBox;
double*p2 = points;


return app.exec();
}
