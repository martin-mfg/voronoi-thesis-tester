#include <QtGui>
#include "Geometry.cpp"


#define WIDTH 600
#define HEIGHT 400

#define CB_X	0
#define CB_Y	0
#define CB_WIDTH 150
#define CB_HEIGHT 50

#define EB_X	151
#define EB_Y	0
#define EB_WIDTH 150
#define EB_HEIGHT 50

#define OB_X	301
#define OB_Y	0
#define OB_WIDTH 150
#define OB_HEIGHT 50

#define SB_X	451
#define SB_Y	0
#define SB_WIDTH 150
#define SB_HEIGHT 50



#define BT_X	30
#define BT_Y	30

#define MESS_X	30
#define MESS_Y	380

class MyGraphics : public QWidget {
	Q_OBJECT

public:
	MyGraphics (QWidget* obj=0);
	virtual void paintEvent(QPaintEvent *);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mousePressEvent(QMouseEvent *);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);

	Geometry geometry;
};

