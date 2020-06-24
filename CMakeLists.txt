project(SerialPort-Assistant)
cmake_minimum_required(VERSION 2.4)
# find library
find_package(wxWidgets REQUIRED)
find_package(Boost REQUIRED)
find_path(wxMathPlot_INCLUDE_DIR mathplot.h REQUIRED)
find_path(wxMathPlot_LIB_DIR libmathplot.a REQUIRED)

# Compile source files
set(CMAKE_CXX_FLAGS "-static -std=c++11 -Wno-deprecated-declarations") # specified flags

IF(WIN32) # following libraries must be linked on windows
message(STATUS "Compiling under win32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
list(APPEND PLATEFORM_LIBS
	ws2_32
	comctl32
)
file(GLOB wxWidgets_LINK_FLAGS # specified wxWidgets static library 
    ${wxWidgets_LIB_DIR}/libwxmsw*core.a
	${wxWidgets_LIB_DIR}/libwxbase*u.a
	${wxWidgets_LIB_DIR}/libwxzlib.a
	${wxWidgets_LIB_DIR}/libwxpng.a
)

list(APPEND SRC_LIST include/resource.rc)
ELSEIF(UNIX)
message(STATUS "Compiling under unix")
execute_process (
    COMMAND bash -c "wx-config --cxxflags | tr '\n' ' ' "
    OUTPUT_VARIABLE CMAKE_CXX_FLAGS
)
execute_process (
    COMMAND bash -c "wx-config --libs | tr '\n' ' ' "
    OUTPUT_VARIABLE wxWidgets_LINK_FLAGS
)
set(Boost_LIBS boost_system boost_thread)
ENDIF()

aux_source_directory(src SRC_LIST) # add source files
include_directories(${wxMathPlot_INCLUDE_DIR} ${wxWidgets_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
link_directories(${wxMathPlot_LIB_DIR} ${wxWidgets_LIB_DIR})

add_executable(SerialPort-Assistant ${SRC_LIST})

target_link_libraries(SerialPort-Assistant 
    -Wl,--start-group # if the libraries of wxWidgets don't linked by --start-group, link error occurred
    ${PLATEFORM_LIBS} 
    ${Boost_LIBS}
    ${wxWidgets_LINK_FLAGS} 
    mathplot 
    -Wl,--end-group
)

