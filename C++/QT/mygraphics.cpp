#include <QtGui>
#include <iostream>
//#include "problem.h"
#include "mygraphics.h"

MyGraphics::MyGraphics (QWidget* obj) :	QWidget(obj) {
	//p=0;
}

void MyGraphics::paintEvent(QPaintEvent* ) {
	QPainter paint(this);
	int i;
	char tmp[50];
	
	strcpy (tmp, "");

	paint.setPen(QPen(Qt::red, 10));
	for (i=0;i<p.numPoints();i++){
		if (p.getY(i) > CB_Y + CB_HEIGHT)
			paint.drawPoint(QPointF(p.getX(i),p.getY(i)));
	}

	vector <Point> edges = p.getEdges();
	Point p1,p2;
	while(edges.size()){
		p1=edges.back();		
		edges.pop_back();
		p2=edges.back();
		paint.drawLine((int)p1.x(),(int)p1.y(),(int)p2.x(),(int)p2.y());
		edges.pop_back();
	}	
	if (p.numPoints() > 0)
		sprintf(tmp, " COORDS ARE - %d,%d", p.getX(i-1), p.getY(i-1) );

	paint.setFont ( QFont ("Courier", 16));
	paint.drawText(MESS_X, MESS_Y, tmp);

	paint.setPen(QPen(Qt::black,3));
	paint.drawRect(CB_X,CB_Y,CB_WIDTH, CB_HEIGHT);
	paint.drawText(CB_X+BT_X,CB_Y+BT_Y,"clear");
	paint.drawRect(EB_X,EB_Y,EB_WIDTH, EB_HEIGHT);
	paint.drawText(EB_X+BT_X,EB_Y+BT_Y,"exit");

}

void MyGraphics::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
	mode=1;
    }
}

void MyGraphics::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Control)
    {
	mode=0;
    }
}


void MyGraphics::mouseMoveEvent (QMouseEvent* event){
/*	gx[gi] = (event->x());
	gy[gi] = (event->y());
	if (gi<N_POINTS-1)
		++gi;
	repaint();
*/
	repaint();
}

void MyGraphics::mousePressEvent (QMouseEvent* event){
	int i;
	int x=(event->x());
	int y=(event->y());
	if (!mode){
		p.add(x,y);
		double bbox[] ={-1000.0,1000.0,-1000.0,1000.0};
		p.getEdges(bbox	);
		if (x > CB_X && x < CB_X+CB_WIDTH &&
		    y > CB_Y && y < CB_Y+CB_HEIGHT)
			p.clear();

		if (x > EB_X && x < EB_X+EB_WIDTH &&
		    y > EB_Y && y < EB_Y+EB_HEIGHT)
			exit(0);

//		if (gi <N_POINTS-1)
//			++gi;
	} else {
		p.remove(x,y);
		}
	repaint();
}

