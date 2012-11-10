# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.


project( gui_demo )

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
  # FindQt3-patched.cmake is FindQt3.cmake patched by CGAL developers, so
  # that it can be used together with FindQt4: all its variables are prefixed
  # by "QT3_" instead of "QT_".
  
#  if(CGAL_Qt4_FOUND AND QT4_FOUND)
 if(true) 
#    include( Qt4Macros-patched )
#    qt4_automoc(  main.cpp problem.cpp moc_mygraphics.cpp mygraphics.cpp )

    # Make sure the compiler can find generated .moc files
    include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR})
  
    include_directories( ${QT4_INCLUDE_DIR} )
  INCLUDE( ${QT_USE_FILE} )

    add_executable  (gui_demo moc_mygraphics.cpp mygraphics.cpp main.cpp )
  
  
    # Link the executable to CGAL and third-party libraries
    target_link_libraries(gui_demo ${QT4_LIBRARIES} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
  else()
    message(STATUS "NOTICE: This demo requires Qt3 and the CGAL Qt3 library, and will not be compiled.")
  
  endif()
  
else()
  
    message(STATUS "NOTICE: This demo requires the CGAL library, and will not be compiled.")
  
endif()

