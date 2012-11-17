#include <QtGui>
#include <iostream>
#include "mygraphics.h"
#include "basicDefinitions.h"

MyGraphics::MyGraphics (QWidget* obj) :	QWidget(obj), geometry(WIDTH,HEIGHT) {
}


int tempX = 0;
int tempY = 0;

void MyGraphics::paintEvent(QPaintEvent* ) {
	QPainter paint(this);
	int i;
	char tmp[50];	//Used to print
	strcpy (tmp, ""); 


	for (i=0;i<geometry.numPoints();i++){		// Draw all points
		if (geometry.getColor(i) == RED)
			paint.setPen(QPen(RED, 10));
		else
			paint.setPen(QPen(BLUE, 10));
		if (geometry.getY(i) > CB_Y + CB_HEIGHT)
			paint.drawPoint(QPointF(geometry.getX(i),geometry.getY(i)));
	}

	vector <Circle> circs = geometry.getCircles(); 
	double x,y,c;
	while(circs.size()){
		x = circs.back().center().x();
		y = circs.back().center().y();
		c = sqrt(circs.back().squared_radius());
		paint.drawEllipse(x,y,c,c);
		circs.pop_back();
	}
	QColor edgeColor;
	const Segment * segment;
	int numEdges = geometry.getNumEdges();
	for(int i = 0; i < numEdges; ++i ) {
		segment = geometry.getEdge( i, edgeColor );
		if( segment == NULL ) {
			continue;
		}
		
		paint.setPen(QPen(edgeColor, 3));
		paint.drawLine( (int)segment->source().x(), (int)segment->source().y(), (int)segment->target().x(), (int)segment->target().y() );
	}
	
	paint.setPen(QPen(RED, 3));

	if (geometry.numPoints() > 0)	//The coordinate of the last point
		sprintf(tmp, "(%d,%d)", geometry.getX(i-1), geometry.getY(i-1) );

	paint.setFont ( QFont ("Courier", 16));
	paint.drawText(MESS_X, MESS_Y, tmp);

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
				y > EB_Y && y < EB_Y+EB_HEIGHT) //If Quit-button clicked
			exit(0);
		else if (x > OB_X && x < OB_X+OB_WIDTH &&
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
			QString filename = QFileDialog::getOpenFileName(
				this,
				tr("Save Document"),
				QDir::currentPath(),
				tr("configurations (*.cnfg);;All files (*.*)"),
				0,
				QFileDialog::DontUseNativeDialog );
			geometry.saveFile(filename.toUtf8().constData());
		} else
			geometry.addPoint(x,y);	//Add a new point

		//TODO: Should be WIDHT and HEIGHT from main.cpp
		double bbox[] ={-1000.0,1000.0,-1000.0,1000.0};
		//geometry.getEdges(bbox);

	} else
		geometry.remove(x,y);	//Remove point
	repaint();
	repaint();
}

