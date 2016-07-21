cmake_minimum_required(VERSION 2.8)
project(final)
set(HEADERS tapplication.h thttpserver.h tworker.h)
set(SOURCES main.cpp tapplication.cpp thttpserver.cpp tworker.cpp main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0 -pthread -DUNIX")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
