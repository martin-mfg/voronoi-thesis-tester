project(gui)

cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()

find_package(CGAL QUIET COMPONENTS Core Qt4 )
if ( CGAL_FOUND )

  include( ${CGAL_USE_FILE} )
  
  find_package(Qt4 REQUIRED )  
  if(CGAL_Qt4_FOUND AND QT4_FOUND)
    	# I commented this line out because it threw errors
	#include( Qt4Macros )
    SET( QT_USE_QT3SUPPORT TRUE )
    SET( QT_USE_QTXML TRUE )
    qt4_automoc( main.cpp problem.cpp moc_mygraphics.cpp mygraphics.cpp mygraphics.h )
    
    include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
  
    set (FOO_MOC_HEADERS mygraphics.h)
    qt4_wrap_cpp (FOO_MOC_OUTFILES ${FOO_MOC_HEADERS})

    #add_library (foo SHARED ${FOO_SOURCE_FILES} ${FOO_MOC_OUTFILES})
    include_directories(${CMAKE_BINARY_DIR} ${QT4_INCLUDE_DIR} )
    INCLUDE( ${QT_USE_FILE} )

    add_executable  (gui_demo moc_mygraphics.cxx mygraphics.cpp main.cpp )

    # Link the executable to CGAL and third-party libraries
    target_link_libraries(gui_demo ${QT4_LIBRARIES} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
  else()
    message(STATUS "NOTICE: This demo requires Qt4 and the CGAL Qt4 library, and will not be compiled.")  
  endif()
else()
  message(STATUS "NOTICE: This demo requires the CGAL library, and will not be compiled.")
  
endif()

