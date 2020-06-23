project(SerialPort-Assistant)
cmake_minimum_required(VERSION 2.4)
# find library
find_package(wxWidgets REQUIRED)
find_package(Boost REQUIRED)
find_path(wxMathPlot_INCLUDE_DIR mathplot.h REQUIRED)
find_path(wxMathPlot_LIB_DIR libmathplot.a REQUIRED)

# Compile source files
set(CMAKE_CXX_FLAGS "-static -std=c++11 -Wno-deprecated-declarations -mwindows") # specified flags

file(GLOB wxWidgets_LIBS # specified wxWidgets static library 
    ${wxWidgets_LIB_DIR}/libwx*core.a
	${wxWidgets_LIB_DIR}/libwxbase*u.a
	${wxWidgets_LIB_DIR}/libwxzlib.a
	${wxWidgets_LIB_DIR}/libwxpng.a
)

IF(WIN32) # following libraries must be linked on windows
list(APPEND PLATEFORM_LIBS
    comctl32
	wsock32
	odbc32
	ws2_32
)
ENDIF()

aux_source_directory(src SRC_LIST) # add source files
include_directories( ${wxMathPlot_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )
link_directories( ${wxMathPlot_LIB_DIR} ${wxWidgets_LIB_DIR} )
add_executable( SerialPort-Assistant ${SRC_LIST} include/resource.rc)

target_link_libraries( SerialPort-Assistant 
    -Wl,--start-group # if the libraries of wxWidgets don't linked by --start-group, link error occurred
    ${wxWidgets_LIBS} 
	${PLATEFORM_LIBS} 
	mathplot
	-Wl,--end-group
)

