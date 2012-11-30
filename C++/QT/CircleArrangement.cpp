#include "basicDefinitions.h"
#include "PointInCircles.cpp"
#include <CGAL/Arr_circle_segment_traits_2.h>


typedef CGAL::Arr_circle_segment_traits_2<K> Circle_Traits;
typedef CGAL::Arrangement_2<Circle_Traits> Circle_Arrangement_Type;
typedef Circle_Traits::Point_2 CircleTrait_Point;
typedef Circle_Traits::X_monotone_curve_2 CircleTrait_Segment;

class Face_split_observer : public CGAL::Arr_observer<Circle_Arrangement_Type>
{
public:
  Circle_Arrangement_Type::Face_iterator selected_face;
  Halfedge_handle halfedge;
  Halfedge_handle invalid;
  Face_split_observer (Circle_Arrangement_Type& arr, Circle_Arrangement_Type::Face_iterator face_iterator) :
    CGAL::Arr_observer<Circle_Arrangement_Type> (arr)
  {
    selected_face = face_iterator;
  }

  virtual void before_split_face (Face_handle f, Halfedge_handle e)
  {
	  if( f == selected_face ) {
		  halfedge = e;
	  }
  }
};

class CircleArrangement {
private:
	Circle_Arrangement_Type arrangement;
	vector<Circle> circles;
	
/*	K::FT leftmost;
	K::FT rightmost;
	K::FT topmost;
	K::FT bottommost;
	*/
	
	double leftmost;
	double rightmost;
	double topmost;
	double bottommost;
	
	Point to_Point( CircleTrait_Point p )  {
		return Point( CGAL::to_double( p.x() ), CGAL::to_double( p.y() ) );
	}
	
	Point get_middle_Point( Point p1, Point p2 ) {
		return Point( ( p1.x() + p2.x() ) / 2 , ( p1.y() + p2.y() ) / 2 );
	}
	
public:
	CircleArrangement() {
		leftmost = 0;
		rightmost = 0;
		topmost = 0;
		bottommost = 0;
	}
	
	void addCircles( vector<Circle> new_circles ) {
		cerr<<"before inserting\n";
		//insert( arrangement, new_circles.begin(), new_circles.end() );
/*
		for(vector<Circle>::iterator cit = new_circles.begin(); cit != new_circles.end(); ++cit) {
			cerr<<"x: "<< (cit->center()).x() <<"\n";			
			cerr<<"y: "<< (cit->center()).y() <<"\n";			
			cerr<<"r²: "<< cit->squared_radius() <<"\n\n";			
			vector<Circle>::iterator cit2=cit;
			++cit2;			
			insert( arrangement, cit, cit2 );
		}
*/
		vector<Circle> new_circles2 = new_circles;
		while (new_circles2.size()){
			insert ( arrangement, Circle_Traits::Curve_2(new_circles2.back()) );
			new_circles2.pop_back();
			cout << arrangement.number_of_vertices() << endl;
		}
		
		circles.insert( circles.end(), new_circles.begin(), new_circles.end() );

		//update leftmost, rightmost, topmost, bottommost
		vector<Circle>::iterator cit;
		for( cit = new_circles.begin(); cit != new_circles.end(); ++cit ) {
			Point center = cit->center();
			double squared_radius = cit -> squared_radius() + 1;
			double left = center.x() - squared_radius;
			double right = center.x() + squared_radius;
			double top = center.y() + squared_radius;
			double bottom = center.y() - squared_radius;
			if( leftmost > left || leftmost == 0) {
				leftmost = 1.000001*sqrt(left);
			}
			if( rightmost < right || rightmost == 0) {
				rightmost = 1.000001*sqrt(right);
			}
			if( topmost < top || topmost == 0) {
				topmost = 1.000001*sqrt(top);
			}
			if( bottommost > bottom || bottommost == 0 ) {
				bottommost = 1.000001*sqrt(bottom);
			}
		}
		cout << "Done" << endl;
	}
	
	const vector<Circle> * getCircles() {
		return & circles;
	}
	
	vector<PointInCircles> get_Points() {
		Point p1;
		Point p2;
		Point middle;
		Line line;
		vector<PointInCircles> result = vector<PointInCircles>();
		Circle_Arrangement_Type::Face_iterator fit;
		cerr<<"before outer for\n";
		for( int face_index = 0 ; face_index < arrangement.number_of_faces() ; ++face_index ) {
		cerr<<"new round in the outer for\n";
			Circle_Arrangement_Type temp_arrangement = Circle_Arrangement_Type( arrangement );
			fit = temp_arrangement.faces_begin();
			for( int i = 0; i < face_index; ++i ) {
				++fit;
			}
			cerr<<"check whether face is unbounded\n";
			if( fit->is_unbounded() == false ) { // ignore the outer face 
				cerr<<"face is bounded\n";
				//calculate a Line going through the face
				/*
				Circle_Arrangement_Type::Ccb_halfedge_circulator halfedge = fit->outer_ccb();
				p1 = to_Point( halfedge->source()->point() );
				p2 = to_Point( halfedge->target()->point() );
				middle = get_middle_Point( p1, p2 );
				line = Line( p1, p2 );
				line = line.perpendicular( middle );

				if(abs(p1.x()-p2.x() + p1.y() - p2.y()) < 0.1){
					continue;
					cout << "Source and target were the same point\n";
                }
				cout << "halfedge: " << halfedge->curve() << endl;
				cerr<<"line calculated\n";

				CGAL::Object intersection = CGAL::intersection( line, Rectangle( leftmost, bottommost, rightmost, topmost ) );
				const Segment * line_segment;// = CGAL::object_cast<Segment>( &intersection );
				Face_split_observer observer ( temp_arrangement, fit );

                if (line_segment = CGAL::object_cast<Segment>(&intersection)) {
					cout << "Is segment\n";
                } else {
					cout << "Is not segment\n";
					continue;
                }


//				insert( temp_arrangement, CircleTrait_Segment( line_segment2.source(), line_segment2.target() ) );
				cerr<<"inserted segment\n";
			    if (observer.halfedge == observer.invalid){
					cout << "\n\n\n\nobserver was invalid\n\n\n\n" << endl;
					 continue;
				}
*/
				Circle_Arrangement_Type::Ccb_halfedge_circulator halfedge = fit->outer_ccb();
				PointInCircles found_Point;
//				found_Point.point = get_middle_Point( to_Point( observer.halfedge->source()->point() ), to_Point( observer.halfedge->target()->point() ) );
				found_Point.point = to_Point( halfedge->source()->point());
				found_Point.circles = vector<bool>();
				
				vector<Circle>::iterator cit;
				cerr<<"before inner for\n";
				for( cit = circles.begin(); cit != circles.end(); ++cit ) {
					bool b = false;
					if( cit->squared_radius() > CGAL::squared_distance( cit->center(), found_Point.point ) ) {
						b = true;
					}
					found_Point.circles.push_back( b );
				}
				
				result.push_back( found_Point );
			}
		
		}
	return result;
	}
	
};

