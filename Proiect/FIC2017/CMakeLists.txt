cmake_minimum_required(VERSION 2.8)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++11")

project( VideoTool )
find_package( OpenCV REQUIRED )

include_directories ("${PROJECT_SOURCE_DIR}/Sockets")
add_subdirectory (Sockets)
set (EXTRA_LIBS ${EXTRA_LIBS} Sockets)

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
add_executable( VideoTool VideoTool.cpp )

target_link_libraries( VideoTool ${OpenCV_LIBS} ${EXTRA_LIBS})
