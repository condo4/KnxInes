cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(KnxdObject)
add_subdirectory(UnixConfig)

project(KnxInes)
add_executable(${PROJECT_NAME} "main.cpp" "ines.cpp" "ines.h" "JSON.cpp" "JSON.h" "JSONValue.cpp" "JSONValue.h")
target_link_libraries(${PROJECT_NAME} UnixConfig KnxdObject eibclient)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
install(FILES knxines.service DESTINATION /lib/systemd/system)

SET(CPACK_PACKAGE_VERSION "1.0.0")
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "knxd")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Fabien Proriol") #required

INCLUDE(CPack)
