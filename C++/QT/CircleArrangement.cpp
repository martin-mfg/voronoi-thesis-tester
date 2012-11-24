#include "basicDefinitions.h"
#include "PointInCircles.cpp"

typedef CGAL::Arr_circle_segment_traits_2<K> Circle_Traits;
typedef CGAL::Arrangement_2<Circle_Traits> Circle_Arrangement_Type;
typedef Circle_Traits::Point_2 CircleTrait_Point;
typedef Circle_Traits::X_monotone_curve_2 CircleTrait_Segment;

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
	vector<Circle> circles;
	
	K::FT leftmost;
	K::FT rightmost;
	K::FT topmost;
	K::FT bottommost;
	
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
		insert( arrangement, new_circles.begin(), new_circles.end() );
		circles.insert( circles.end(), new_circles.begin(), new_circles.end() );

		//update leftmost, rightmost, topmost, bottommost
		vector<Circle>::iterator cit;
		for( cit = new_circles.begin(); cit != new_circles.end(); ++cit ) {
			Point center = cit->center();
			K::FT squared_radius = cit -> squared_radius() + 1;
			K::FT left = center.x() - squared_radius;
			K::FT right = center.x() + squared_radius;
			K::FT top = center.y() + squared_radius;
			K::FT bottom = center.y() - squared_radius;
			if( leftmost > left ) {
				leftmost = left;
			}
			if( rightmost < right ) {
				rightmost = right;
			}
			if( topmost < top ) {
				topmost = top;
			}
			if( bottommost > bottom ) {
				bottommost = bottom;
			}
		}
	}
	
	const vector<Circle> * getCircles() {
		return & circles;
	}
	
	vector<PointInCircles> get_Points() {
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
				Point p1 = to_Point( halfedge->source()->point() );
				Point p2 = to_Point( halfedge->target()->point() );
				Point middle = get_middle_Point( p1, p2 );
				Line line = Line( p1, p2 );
				line = line.perpendicular( middle );

				CGAL::Object intersection = CGAL::intersection( line, Rectangle( leftmost, bottommost, rightmost, topmost ) );
				const Segment * line_segment = CGAL::object_cast<Segment>( &intersection );
				Face_split_observer observer ( temp_arrangement, fit );
				insert( temp_arrangement, CircleTrait_Segment( line_segment->source(), line_segment->target() ) );
				
				PointInCircles found_Point;
				found_Point.point = get_middle_Point( to_Point( observer.halfedge->source()->point() ), to_Point( observer.halfedge->target()->point() ) );
				found_Point.circles = vector<bool>();
				
				vector<Circle>::iterator cit;
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

