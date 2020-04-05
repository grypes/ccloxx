cmake_minimum_required (VERSION 3.0.0)
 
project (ccloxx)

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

include_directories(src)

add_executable (${PROJECT_NAME} ${SOURCES})