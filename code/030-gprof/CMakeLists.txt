
cmake_minimum_required (VERSION 3.8)
project ("test")
include_directories("./include")
link_directories("./lib")
add_executable (${PROJECT_NAME} "test.cpp")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")