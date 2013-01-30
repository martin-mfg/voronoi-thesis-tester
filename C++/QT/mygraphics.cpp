#include <QtGui>
#include <iostream>
#include <sys/time.h>
#include "mygraphics.h"
#include "basicDefinitions.h"
timeval start;
bool timeout(){
  long mtime, seconds, useconds;    
  timeval end;
  gettimeofday(&end,NULL);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  if (mtime > 300000)
    return true;
  else
    return false;
}

void printtime(int numRed, int numBlue, timeval start, int NS1I, int NS2I){
  long mtime, seconds, useconds;    
  timeval end;
  gettimeofday(&end,NULL);
  seconds  = end.tv_sec  - start.tv_sec;
  useconds = end.tv_usec - start.tv_usec;
  mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
  
  cout << numRed << ", " << numBlue << ", " << mtime << ", " << NS1I << ", " << NS2I << endl;
  
}



MyGraphics::MyGraphics (const char * argv, QWidget* obj) :	QWidget(obj), geometry(WIDTH,HEIGHT){
	gettimeofday(&start,NULL);	

	if ( strlen(argv) > 0 ){
	repeat:
			geometry.readFile(argv);
			geometry.saveFile("latest.cnfg");

			double obj=-1;
			while (obj){
				if (obj/geometry.numPoints() < 300 && obj > 0 &&
					geometry.redEqualBlue()){
					obj=geometry.genetics(0,start);
					if (obj == -1)
						break;
					update();
				} else {
				  if (timeout()){
						obj = -1;
						break;
				  } else
					obj=geometry.solver(0);
				}
			}   
			if(geometry.numRedPoints() != geometry.numBluePoints() || obj == -1) {
				std::string s;
				std::stringstream out;
				out << start.tv_sec;
				s = out.str();
				string file = "testing/";
				file += s;
				file += ".cnfg";
				printtime(geometry.numRedPoints(), geometry.numBluePoints(), start,
						geometry.getNS1I(), geometry.getNS2I());
				geometry.remove_blue_points();
				geometry.saveFile(file.c_str());
			} else {
			printtime(geometry.numRedPoints(), geometry.numBluePoints(), start,
						geometry.getNS1I(), geometry.getNS2I());
}


			exit(0); // Makes it easier to take time with: time ./gui_demo
	}
}


int tempX = 0;
int tempY = 0;

void MyGraphics::paintEvent(QPaintEvent* ) {
	QPainter paint(this);
	int i;
	char tmp[50];	//Used to print
	strcpy (tmp, ""); 

	//draw points
	for (i=0;i<geometry.numPoints();i++){
		if (geometry.getColor(i) == RED)
			paint.setPen(QPen(RED, 10));
		else
			paint.setPen(QPen(BLUE, 10));
		if (geometry.getY(i) > CB_Y + CB_HEIGHT)
			paint.drawPoint(QPointF(geometry.getX(i),geometry.getY(i)));
	}

	//draw circles
	paint.setPen(QPen(RED, 1));
	vector <Circle> circs = geometry.getCircles(); 
	double x,y,c;
	while(circs.size()) {
		x = circs.back().center().x();
		y = circs.back().center().y();
		c = sqrt(circs.back().squared_radius());
		paint.drawEllipse( x - c, y - c, 2 * c, 2 * c );
		circs.pop_back();
	}
	
	//draw edges
	QColor edgeColor;
	const Segment * segment;
	int numEdges = geometry.getNumEdges();
	for(int i = 0; i < numEdges; ++i ) {
		segment = geometry.getEdge( i, edgeColor );
		if( segment == NULL ) {
			continue;
		}
		
		paint.setPen(QPen(edgeColor, 3));
		paint.drawLine( (int)segment->source().x(), (int)segment->source().y(), 			(int)segment->target().x(), (int)segment->target().y() );
	}
	
	//draw coordinates of the last point
	paint.setPen(QPen(RED, 3));
	if (geometry.numPoints() > 0)
		sprintf(tmp, "(%d,%d)", (int)geometry.getX(i-1), (int)geometry.getY(i-1) );

	paint.setFont ( QFont ("Courier", 16));
	paint.drawText(MESS_X, MESS_Y, tmp);

	//draw the buttons
	paint.setBrush(Qt::darkCyan);
	paint.setPen(QPen(Qt::black,3));
	paint.drawRect(CB_X,CB_Y,CB_WIDTH, CB_HEIGHT);
	paint.drawText(CB_X+BT_X,CB_Y+BT_Y,"Clear");
	paint.drawRect(EB_X,EB_Y,EB_WIDTH, EB_HEIGHT);
	paint.drawText(EB_X+BT_X,EB_Y+BT_Y,"Quit");
	paint.drawRect(OB_X,OB_Y,OB_WIDTH, OB_HEIGHT);
	paint.drawText(OB_X+BT_X,OB_Y+BT_Y,"Open");
	paint.drawRect(SB_X,SB_Y,SB_WIDTH, SB_HEIGHT);
	paint.drawText(SB_X+BT_X,SB_Y+BT_Y,"Save");
	paint.drawRect(SOB_X,SOB_Y,SOB_WIDTH, SOB_HEIGHT);
	paint.drawText(SOB_X+BT_X,SOB_Y+BT_Y,"Solve");
}

void MyGraphics::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
		geometry.mode=1;		//Set to delete mode
    }
}

void MyGraphics::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
		geometry.mode=0;		//Set to add mode
    }
    if(event->key() == Qt::Key_Tab) {
		geometry.current_color=1-geometry.current_color;		//Set to add mode
    }

}

void MyGraphics::mouseMoveEvent (QMouseEvent* event){
	//Things here are run if the right button is pressed down
	//and then moved.
	if(!geometry.mode){
		geometry.remove(tempX,tempY);
		tempX=(event->x());
		tempY=(event->y());
		geometry.addPoint(tempX,tempY);
		double bbox[] ={0.0,0.0,WIDTH,HEIGHT};
		//geometry.getEdges(bbox);
	}
	repaint();
}

void MyGraphics::mousePressEvent (QMouseEvent* event){
	int i;
	int x=(event->x());	//x- and y-coordinates of the mouse when pressed.
	int y=(event->y());
	tempX=x;
	tempY=y;
	if (!geometry.mode){	//If we are in add-mode
		if (x > CB_X && x < CB_X+CB_WIDTH &&
		    y > CB_Y && y < CB_Y+CB_HEIGHT) //If clear-button clicked
			geometry.clearPoints();
		else if (x > EB_X && x < EB_X+EB_WIDTH &&
				y > EB_Y && y < EB_Y+EB_HEIGHT) {//If Quit-button clicked
			exit(0);
		} else if (x > OB_X && x < OB_X+OB_WIDTH &&
				y > OB_Y && y < OB_Y+OB_HEIGHT) { //If Quit-button clicked
			QString filename = QFileDialog::getOpenFileName(
				this,
				tr("Open Document"),
				QDir::currentPath(),
				tr("configurations (*.cnfg);;All files (*.*)"),
				0,
				QFileDialog::DontUseNativeDialog );
			geometry.readFile(filename.toUtf8().constData());
		} else if (x > SB_X && x < SB_X+SB_WIDTH &&
				y > SB_Y && y < SB_Y+SB_HEIGHT) { //If Quit-button clicked
			QString filename = QFileDialog::getSaveFileName(
				this,
				tr("Save Document"),
				QDir::currentPath(),
				tr("configurations (*.cnfg);;All files (*.*)"),
				0,
				QFileDialog::DontUseNativeDialog );
			geometry.saveFile(filename.toUtf8().constData());
		} else if (x > SOB_X && x < SOB_X+SOB_WIDTH &&
				y > SOB_Y && y < SOB_Y+SOB_HEIGHT) {
			double obj=-1;
			geometry.saveFile("latest.cnfg");
			while (obj){
				if (obj/geometry.numPoints() < 300 && obj > 0 &&
					geometry.redEqualBlue()){
					obj = geometry.genetics(1,start);
					update();
					repaint();
				} else {
					//Moved here for verbosity reasons
					cout<<geometry.carr.getCircles()->size()<<" circles collected\n";
					cout<<geometry.getCircles().size()<<" circles to be inserted\n";
					cout<<geometry.numPoints()<<" points\n";
					obj=geometry.solver(0);
					repaint();//			geometry.solver();
				}
			}
			repaint();
		} else
			geometry.addPoint(x,y);	//Add a new point
	} else
		geometry.remove(x,y);	//Remove point
	repaint();
	repaint();
}

