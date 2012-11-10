#include <QtGui>
#include "problem.h"

#define N_POINTS 1000

#define CB_X	0
#define CB_Y	0
#define CB_WIDTH 200
#define CB_HEIGHT 50

#define EB_X	201
#define EB_Y	0
#define EB_WIDTH 200
#define EB_HEIGHT 50

#define BT_X	30
#define BT_Y	30

#define MESS_X	30
#define MESS_Y	380

// point "hitbox"
#define HB	10

class Points {
	private:
		int gx[N_POINTS], gy[N_POINTS];
		int gi=0;
		vector <Point> edges;
	public:
		int numPoints(){
			return gi;
		}
		vector <Point> getEdges(){
			return edges;
		}
		int x(int i){
			return gx[i];
		}
		int y(int i){
			return gy[i];
		}
		void add(int x,int y){
			bool exists = false;
			for(int i=0;i<gi;i++)	
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					exists=true;
			if (!exists){
				gx[gi]=x;
				gy[gi]=y;
				gi++;
			}
		}
		int getX(int i){
			return gx[i];
		}
		int getY(int i){
			return gy[i];
		}
		void clear(){
			gi=0;
		}
		void shift(int i){
			if(i==gi-1)
				gi--;
			else {
				gx[i]=gx[i+1];
				gy[i]=gy[i+1];
				shift(i+1);
			}
		}
		void remove(int x, int y){
			for(int i=0;i<gi;i++)
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					shift(i);
		}
		void getPointVector(){
			
		}

	void getEdges(double bBoxParams[]){
		vector <Point> red_points;
		vector <Point> seg_points;

		Problem prob;
//		Point bleft = Point(bBoxParams[0], bBoxParams[1]);
//		Point tright = Point(bBoxParams[2], bBoxParams[3]);

		Point bleft = Point(-1000.0, 1000.0);
		Point tright = Point(-1000.0, 1000.0);
		
		for (int i=0;i<gi;i++)
			red_points.push_back(Point(gx[i], gy[i]));

		prob.set_red_points(red_points);
		prob.set_bbox(bleft, tright);
		prob.update();
		edges = prob.get_voronoi_edges();


	}

};

class MyGraphics : public QWidget {
	Q_OBJECT

public:
	MyGraphics (QWidget* obj=0);
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	Points p;
	Problem prob;
	// add when 0, remove when 1
	int mode = 0;

};

