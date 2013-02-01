#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/enum.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/number_utils.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Cartesian_converter.h>
#include <QtGui>
#include "gurobi_c++.h"
#include "solver/solver.h"


#define RED Qt::red
#define BLUE QColor(0,136,255)
#define BLACK Qt::black

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Iso_rectangle_2 Rectangle;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;
typedef K::Line_2 Line;
typedef CGAL::Triangle_2<K> 	Triangle; 
typedef CGAL::Circle_2<K> Circle;

typedef CGAL::Exact_predicates_exact_constructions_kernel Circle_Kernel;
typedef CGAL::Arr_circle_segment_traits_2<Circle_Kernel> Circle_Traits;
typedef CGAL::Arrangement_2<Circle_Traits> Circle_Arrangement_Type;
typedef Circle_Traits::Point_2 CircleTrait_Point;
typedef Circle_Traits::X_monotone_curve_2 CircleTrait_Segment;
typedef CGAL::Circle_2<Circle_Kernel> CircleArr_Circle;
typedef Circle_Kernel::Point_2 CircleArr_Point;
typedef Circle_Kernel::Line_2 CircleArr_Line;
typedef Circle_Kernel::Iso_rectangle_2 CircleArr_Rectangle;
typedef Circle_Kernel::Segment_2 CircleArr_Segment;
typedef CGAL::Delaunay_triangulation_2<Circle_Kernel>  Circle_Triangulation;
typedef Circle_Kernel::Ray_2 Circle_Ray;

typedef CGAL::Cartesian_converter<K,Circle_Kernel> Converter_to_circle;
typedef CGAL::Cartesian_converter<Circle_Kernel,K> Converter_to_normal;


