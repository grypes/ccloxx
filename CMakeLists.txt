cmake_minimum_required (VERSION 3.0.0)
 
project (ccloxx)

# set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

# set(SOURCE src/main.cpp)
 
# add_executable (${PROJECT_NAME} ${SOURCE})

# include_directories(${CMAKE_SOURCE_DIR}/src)

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

include_directories(src)

add_executable (${PROJECT_NAME} ${SOURCES})