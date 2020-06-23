project(SerialPort-Assistant)
cmake_minimum_required(VERSION 2.4)
# find library
find_package(wxWidgets)
find_package(Boost)
find_path(wxMathPlot_INCLUDE_DIR mathplot.h)
find_path(wxMathPlot_LIB_DIR libmathplot.a)

# Compile source files
set(CMAKE_CXX_FLAGS "-static -std=c++11 -Wno-deprecated-declarations -mwindows")
set(PLATEFORM_LIBS)

set(wxWidgets_LIBS
    wxmsw30u_core
	wxbase30u
	wxpng
	wxzlib
)

IF(WIN32)
list(APPEND PLATEFORM_LIBS
    comctl32
	wsock32
	odbc32
	ws2_32
)
ENDIF()

aux_source_directory(src SRC_LIST)
include_directories( ${wxMathPlot_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )
link_directories( ${wxMathPlot_LIB_DIR} ${wxWidgets_LIB_DIR} )
add_executable( SerialPort-Assistant ${SRC_LIST} include/resource.rc)

target_link_libraries( SerialPort-Assistant 
    ${wxWidgets_LIBS} 
	${PLATEFORM_LIBS} 
	mathplot 
)

