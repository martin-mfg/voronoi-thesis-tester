#include "basicDefinitions.h"

class ColoredEdge {
public:
	CGAL::Object edge;
	QColor color;
	
	ColoredEdge(CGAL::Object _edge, QColor _color) {
		edge=_edge;
		color=_color;
	}
};
