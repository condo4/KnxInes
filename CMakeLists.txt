cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(KnxdObject)
add_subdirectory(UnixConfig)

project(KnxInes)
add_executable(${PROJECT_NAME} "main.cpp" "ines.cpp" "ines.h" "JSON.cpp" "JSON.h" "JSONValue.cpp" "JSONValue.h")
target_link_libraries(${PROJECT_NAME} UnixConfig KnxdObject eibclient)
