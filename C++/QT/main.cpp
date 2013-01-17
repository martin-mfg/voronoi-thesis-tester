#include <QtGui>
#include "mygraphics.h"
#include <unistd.h>
#include <string>
//#include "gurobi_c++.h"
//#include "solver/solver.cpp"
//#include "CircleArrangement.cpp"


void randomfile(int numPoints){
	std::ofstream outFile;
//	char filename[] = "random.cnfg";
	outFile.open("random.cnfg", ios::trunc);

	if(!outFile) {
		std::cerr << "Unable to open file random.cnfg";
	} else {
		for(int i=0;i<numPoints;i++) {
				outFile << rand()%500 << " " << rand()%500 << " " << "1" << endl;
		}
	}
}


int main (int argc, char* argv[]){
	QApplication app(argc,argv);
	std::string filename;

	if ( argc == 1 ) {
		filename = "";
	} else {
		filename = argv[1];
	}
	
	if (argc > 1)
	if (strcmp(argv[1], "random") == 0) {
		filename = "random.cnfg";
		randomfile(atoi(argv[2]));
	}
	
	MyGraphics g(filename.c_str());
	
	g.resize(WIDTH,HEIGHT);
	g.show();
	srand((unsigned)time(0));
	app.exec();
	
return 0;

}
