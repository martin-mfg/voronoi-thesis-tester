#include "basicDefinitions.h"
#include "PointInCircles.cpp"
#include <CGAL/Arr_circle_segment_traits_2.h>


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
	
	//the next 2 methods were copied from the cgal doc ( http://www.cgal.org/Manual/3.3/doc_html/cgal_manual/Arrangement_2_ref/Class_Arrangement_2.html#Cross_link_anchor_796 ):
	void print_ccb (Circle_Arrangement_Type::Ccb_halfedge_const_circulator circ) {
		Circle_Arrangement_Type::Ccb_halfedge_const_circulator curr = circ;
		std::cout << "(" << curr->source()->point() << ")";
		do {
			//Circle_Arrangement_Type::Halfedge_const_handle he = curr->handle();
			std::cout << "   [" << curr->curve() << "]   " <<  "(" << curr->target()->point() << ")";
		} while (++curr != circ);
		std::cout << std::endl;
	}

	void print_face (Circle_Arrangement_Type::Face_const_handle f) {
		// Print the outer boundary.
		if (f->is_unbounded())
			std::cout << "Unbounded face. " << std::endl;
		else {
			std::cout << "Outer boundary: ";
			print_ccb (f->outer_ccb());
		}

		// Print the boundary of each of the holes.
		Circle_Arrangement_Type::Hole_const_iterator hi;
		int index = 1;
		for (hi = f->holes_begin(); hi != f->holes_end(); ++hi, ++index) {
			std::cout << "    Hole #" << index << ": ";
			print_ccb (*hi);
		}

		// Print the isolated vertices.
		Circle_Arrangement_Type::Isolated_vertex_const_iterator iv;
		for (iv = f->isolated_vertices_begin(), index = 1; iv != f->isolated_vertices_end(); ++iv, ++index) {
			std::cout << "    Isolated vertex #" << index << ": " << "(" << iv->point() << ")" << std::endl;
		}
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
		
		cerr<<"before inserting\n"<<flush;
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
		cout << "Done" << endl;
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
		
		cerr<<"before outer for\n";
		for( int face_index = 0 ; face_index < arrangement.number_of_faces() ; ++face_index ) {
		cout << "new round in the outer for\n" << flush;
			Circle_Arrangement_Type temp_arrangement = Circle_Arrangement_Type( arrangement );
			fit = temp_arrangement.faces_begin();
			for( int i = 0; i < face_index; ++i ) {
				++fit;
			}
			cerr<<"check whether face is unbounded\n" << flush;
			if( fit->is_unbounded() == false ) { // ignore the outer face 
				cerr<<"face is bounded\n" << flush;
				//calculate a Line going through the face
				Circle_Arrangement_Type::Ccb_halfedge_circulator halfedge = fit->outer_ccb();
				cout << "got outer_ccb\n" << flush;
				
				do {
					print_face(fit);

		vector<CircleArr_Circle>::iterator cit;
		for( cit = circles.begin(); cit != circles.end(); ++cit ) {
			cout << cit->center() << "   " << sqrt( CGAL::to_double( cit->squared_radius() ) ) << "\n";
		}

					
					p1 = to_Point( halfedge->source()->point() );
					p2 = to_Point( halfedge->target()->point() );
					cout << "got points\n" << flush;
					cout << "( " << p1.x() << ", " << p1.y() << " ) and ( " << p2.x() << ", " << p2.y() << " )\n" << flush;

					++halfedge;
					cout << "incremented halfedge iterator\n" << flush;
				} while( abs( p1.x()-p2.x() ) + abs( p1.y() - p2.y() ) < 0.0001 );
				cout << "after do loop\n" << flush;
				
				middle = get_middle_Point( p1, p2 );
				cout << "got middle\n" << flush;
				line = CircleArr_Line( p1, p2 );
				cout << "got line\n" << flush;
				line = line.perpendicular( middle );
				cout << "got perpendicular line\n" << flush;

				cout << "halfedge: " << halfedge->curve() << endl << flush;

				CGAL::Object intersection = CGAL::intersection( line, CircleArr_Rectangle( leftmost, bottommost, rightmost, topmost ) );
				const CircleArr_Segment * line_segment = CGAL::object_cast<CircleArr_Segment>( &intersection );
				Face_split_observer observer ( temp_arrangement, fit );

                if (line_segment) {
					cout << "Is segment\n" << flush;
                } else {
					cout << "Is not segment\n" << flush;
					continue;
                }

				CircleTrait_Segment mySegment( line_segment->source(), line_segment->target() );
				insert( temp_arrangement,mySegment  );
				cerr<<"inserted segment\n" << flush;
			    if (observer.halfedge == observer.invalid){
					cout << "\n\nobserver was invalid\n\n" << endl << flush;
					 continue;
				}

				CircleArr_Point exact_found_point = get_middle_Point( to_Point( observer.halfedge->source()->point() ), to_Point( observer.halfedge->target()->point() ) );
				PointInCircles found_Point;
				Converter_to_normal conv;
				found_Point.point = conv( exact_found_point );
				found_Point.circles = vector<bool>();
				
				vector<CircleArr_Circle>::iterator cit;
				cerr<<"before inner for\n" << flush;
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

