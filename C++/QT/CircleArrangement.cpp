#include "basicDefinitions.h"
#include "PointInCircles.cpp"
#include <CGAL/Arr_circle_segment_traits_2.h>


class Face_split_observer : public CGAL::Arr_observer<Circle_Arrangement_Type>
{
public:
  Circle_Arrangement_Type::Face_iterator selected_face;
  Halfedge_handle halfedge;
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
	vector<CircleArr_Circle> circles;
	
	Circle_Kernel::FT leftmost;
	Circle_Kernel::FT rightmost;
	Circle_Kernel::FT topmost;
	Circle_Kernel::FT bottommost;
	
	CircleArr_Point to_Point( CircleTrait_Point p )  {
		return CircleArr_Point( CGAL::to_double( p.x() ), CGAL::to_double( p.y() ) );
	}
	
	CircleArr_Point get_middle_Point( CircleArr_Point p1, CircleArr_Point p2 ) {
		return CircleArr_Point( ( p1.x() + p2.x() ) / 2 , ( p1.y() + p2.y() ) / 2 );
	}
	
	vector<CircleArr_Circle> make_circles_exact( vector<Circle> circles_in ) {
		vector<CircleArr_Circle> result;
		vector<Circle>::iterator cit;
		for( cit = circles_in.begin(); cit != circles_in.end(); ++cit ) {
			Converter_to_circle conv;
			CircleArr_Point center = conv( cit->center() );
			Circle_Kernel::FT squared_radius = conv( cit -> squared_radius() );
			CircleArr_Circle new_circ( center, squared_radius );
			result.push_back( new_circ );
		}
		return result;
	}
	
public:
	CircleArrangement() {
		leftmost = 0;
		rightmost = 0;
		topmost = 0;
		bottommost = 0;
	}
	
	void addCircles( vector<Circle> new_circles_in ) {
		vector<CircleArr_Circle> new_circles = make_circles_exact( new_circles_in );;
		
		vector<CircleArr_Circle> new_circles2 = new_circles;
		while ( new_circles2.size() ) {
			insert ( arrangement, Circle_Traits::Curve_2(new_circles2.back()) );
			new_circles2.pop_back();
			cout << arrangement.number_of_vertices() << endl;
		}
		
		circles.insert( circles.end(), new_circles.begin(), new_circles.end() );

		//update leftmost, rightmost, topmost, bottommost
		vector<CircleArr_Circle>::iterator cit;
		for( cit = new_circles.begin(); cit != new_circles.end(); ++cit ) {
			CircleArr_Point center = cit->center();
			Circle_Kernel::FT squared_radius = cit -> squared_radius() + 1;
			Circle_Kernel::FT left = center.x() - squared_radius;
			Circle_Kernel::FT right = center.x() + squared_radius;
			Circle_Kernel::FT top = center.y() + squared_radius;
			Circle_Kernel::FT bottom = center.y() - squared_radius;
			if( leftmost > left || leftmost == 0) {
				leftmost = left;
			}
			if( rightmost < right || rightmost == 0) {
				rightmost = right;
			}
			if( topmost < top || topmost == 0) {
				topmost = top;
			}
			if( bottommost > bottom || bottommost == 0 ) {
				bottommost = bottom;
			}
		}
	}
	
	const vector<CircleArr_Circle> * getCircles() {
		return & circles;
	}
	
	vector<PointInCircles> get_Points() {
		CircleArr_Point p1;
		CircleArr_Point p2;
		CircleArr_Point middle;
		CircleArr_Line line;
		vector<PointInCircles> result = vector<PointInCircles>();
		Circle_Arrangement_Type::Face_iterator fit;
		
		for( int face_index = 0 ; face_index < arrangement.number_of_faces() ; ++face_index ) {
			Circle_Arrangement_Type temp_arrangement = Circle_Arrangement_Type( arrangement );
			fit = temp_arrangement.faces_begin();
			for( int i = 0; i < face_index; ++i ) {
				++fit;
			}
			if( fit->is_unbounded() == false ) { // ignore the outer face 
				//calculate a Line going through the face
				Circle_Arrangement_Type::Ccb_halfedge_circulator halfedge = fit->outer_ccb();
				
				Circle_Arrangement_Type::Ccb_halfedge_circulator original_halfedge = halfedge;
				do {
					p1 = to_Point( halfedge->source()->point() );
					p2 = to_Point( halfedge->target()->point() );

					++halfedge;
					if( halfedge == original_halfedge ) {
						break;
					}
				} while( abs( p1.x()-p2.x() ) + abs( p1.y() - p2.y() ) < 0.0001 );
				//if the face consits of only 1 point, skip it
				if( halfedge == original_halfedge ) {
					continue;
				}
				
				middle = get_middle_Point( p1, p2 );
				line = CircleArr_Line( p1, p2 );
				line = line.perpendicular( middle );

				//get the segment of the line that's inside the selected face
				CGAL::Object intersection = CGAL::intersection( line, CircleArr_Rectangle( leftmost, bottommost, rightmost, topmost ) );
				const CircleArr_Segment * line_segment = CGAL::object_cast<CircleArr_Segment>( &intersection );
				Face_split_observer observer ( temp_arrangement, fit );
				CircleTrait_Segment mySegment( line_segment->source(), line_segment->target() );
				insert( temp_arrangement,mySegment  );

				//calculate the middle point of that segment
				CircleArr_Point exact_found_point = get_middle_Point( to_Point( observer.halfedge->source()->point() ), to_Point( observer.halfedge->target()->point() ) );
				PointInCircles found_Point;
				Converter_to_normal conv;
				found_Point.point = conv( exact_found_point );
				found_Point.circles = vector<bool>();
				
				//check which circles it lies in
				vector<CircleArr_Circle>::iterator cit;
				for( cit = circles.begin(); cit != circles.end(); ++cit ) {
					bool b = false;
					if( cit->squared_radius() > CGAL::squared_distance( cit->center(), exact_found_point ) ) {
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
