#include <QtGui>
#include <iostream>
//#include "problem.h"
#include "mygraphics.h"

MyGraphics::MyGraphics (QWidget* obj) :	QWidget(obj) {
	//p=0;
}

int tempX = 0;
int tempY = 0;

void MyGraphics::paintEvent(QPaintEvent* ) {
	QPainter paint(this);
	int i;
	char tmp[50];	//Used to print
	strcpy (tmp, ""); 


	for (i=0;i<p.numPoints();i++){		// Draw all points
		if (p.getColor(i) == Qt::red)
			paint.setPen(QPen(Qt::red, 10));
		else
			paint.setPen(QPen(Qt::blue, 10));
		if (p.getY(i) > CB_Y + CB_HEIGHT)
			paint.drawPoint(QPointF(p.getX(i),p.getY(i)));
	}

	vector <Point> edges = p.getEdges();
	Point p1,p2;
	paint.setPen(QPen(Qt::red, 3));
	while(edges.size()){	//Draw all edges
		p1=edges.back();edges.pop_back();p2=edges.back();
		paint.drawLine((int)p1.x(),(int)p1.y(),(int)p2.x(),(int)p2.y());
		edges.pop_back();
	}
	
	if (p.numPoints() > 0)	//The coordinate of the last point
		sprintf(tmp, "(%d,%d)", p.getX(i-1), p.getY(i-1) );

	paint.setFont ( QFont ("Courier", 16));
	paint.drawText(MESS_X, MESS_Y, tmp);

	paint.setBrush(Qt::darkCyan);
	paint.setPen(QPen(Qt::black,3));
	paint.drawRect(CB_X,CB_Y,CB_WIDTH, CB_HEIGHT);
	paint.drawText(CB_X+BT_X,CB_Y+BT_Y,"Clear");
	paint.drawRect(EB_X,EB_Y,EB_WIDTH, EB_HEIGHT);
	paint.drawText(EB_X+BT_X,EB_Y+BT_Y,"Quit");

}

void MyGraphics::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
		p.mode=1;		//Set to delete mode
    }
}

void MyGraphics::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control) {
		p.mode=0;		//Set to add mode
    }
    if(event->key() == Qt::Key_Tab) {
		p.current_color=1-p.current_color;		//Set to add mode
    }

}

void MyGraphics::mouseMoveEvent (QMouseEvent* event){
	//Things here are run if the right button is pressed down
	//and then moved.
	if(!p.mode){
		p.remove(tempX,tempY);
		tempX=(event->x());
		tempY=(event->y());
		p.add(tempX,tempY);
		double bbox[] ={0.0,0.0,WIDTH,HEIGHT};
		p.getEdges(bbox);
	}
	repaint();
}

void MyGraphics::mousePressEvent (QMouseEvent* event){
	int i;
	int x=(event->x());	//x- and y-coordinates of the mouse when pressed.
	int y=(event->y());
	tempX=x;
	tempY=y;
	if (!p.mode){	//If we are in add-mode
		if (x > CB_X && x < CB_X+CB_WIDTH &&
		    y > CB_Y && y < CB_Y+CB_HEIGHT) //If clear-button clicked
			p.clear();
		else if (x > EB_X && x < EB_X+EB_WIDTH &&
				y > EB_Y && y < EB_Y+EB_HEIGHT) //If Quit-button clicked
			exit(0);
		else
			p.add(x,y);	//Add a new point

		//TODO: Should be WIDHT and HEIGHT from main.cpp
		double bbox[] ={-1000.0,1000.0,-1000.0,1000.0};
		p.getEdges(bbox);

	} else
		p.remove(x,y);	//Remove point
	repaint();
	repaint();
}

