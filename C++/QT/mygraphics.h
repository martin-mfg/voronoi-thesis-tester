#include <QtGui>
#include "problem.h"


#define WIDTH 500
#define HEIGHT 400
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
		int gx[N_POINTS], gy[N_POINTS];	//x- and y-coordinates
		QColor gc[N_POINTS];//color of the point
		int gi;	//Number of points

		vector <Point> edges;	//Voronoi-edges
	public:
		int numPoints(){	return gi;	}
		int mode;
		int current_color;
		Points () {gi=0;mode=0;current_color=0;}

		/* GET FUNCTIONS */
		vector <Point> getEdges(){	return edges;	}

		int getX(int i){	return gx[i];	}
		int getY(int i){	return gy[i];	}
		QColor getColor(int i)	{	return gc[i];	}
		/* SET FUNCTION */
		void add(int x,int y){		//Add point
			bool exists = false;
			for(int i=0;i<gi;i++)	
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					exists=true;
			if (!exists)		//If point doesn't allready exist, add
				if (!current_color){
					gx[gi]=x;gy[gi]=y;gc[gi]=Qt::red;gi++;
				} else {
					gx[gi]=x;gy[gi]=y;gc[gi]=Qt::blue;gi++;
				}
		}
	
		void clear(){	gi=0;	}	//"Remove" all points
		// If point removed, shift the array one step
		void shift(int i){
			if(i==gi-1)
				gi--;
			else {
				gx[i]=gx[i+1];
				gy[i]=gy[i+1];
				gc[i]=gc[i+1];
				shift(i+1);	//recursive call
			}
		}
		void remove(int x, int y){
			for(int i=0;i<gi;i++)
				if(x>=gx[i] && x <=gx[i]+HB && y>=gy[i] && y<=gy[i]+HB)
					shift(i);
		}
		
		void getEdges(double bBoxParams[]){
			vector <Point> red_points;

			Problem prob;	//create a problem-object

			Point tleft = Point(0, 0);
			Point bright = Point(WIDTH, HEIGHT);
		
			for (int i=0;i<gi;i++)
				red_points.push_back(Point(gx[i], gy[i]));

			prob.set_red_points(red_points);//add all points to problem
			prob.set_bbox(tleft, bright);
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
};

