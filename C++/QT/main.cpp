#include <QtGui>
#include "mygraphics.h"
#include <sys/time.h>
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
		std::cerr << "Unable to open file datafile.txt";
	} else {
		for(int i=0;i<numPoints;i++) {
				outFile << rand()%500 << " " << rand()%500 << " " << "1" << endl;
		}
	}
}


int main (int argc, char* argv[]){
	timeval t;
	gettimeofday(&t,NULL);
	QApplication app(argc,argv);
	if (argc > 1)
	if (strcmp(argv[1], "random") == 0) {
//		strcpy(argv[1], "random.cnfg");
		argv[1] = "random.cnfg";
		randomfile(atoi(argv[2]));
	}
	
	MyGraphics g(argv[1], t);
	
	g.resize(WIDTH,HEIGHT);
	g.show();
	srand((unsigned)time(0));
	app.exec();
	
return 0;

}
