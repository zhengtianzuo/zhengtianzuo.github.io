
cmake_minimum_required (VERSION 3.8)
project ("test")
include_directories("./include")
link_directories("./lib")
add_executable (${PROJECT_NAME} "test.cpp")
add_definitions(-DBENCHMARK_STATIC_DEFINE)
target_link_libraries(${PROJECT_NAME} benchmark benchmark_main)