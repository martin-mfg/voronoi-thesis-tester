#include <QtGui>
#include "mygraphics.h"
//#include "problem.h"


int main (int argc, char* argv[]){
QApplication app(argc,argv);
MyGraphics g;
g.resize(WIDTH,HEIGHT);
g.show();

return app.exec();
}
