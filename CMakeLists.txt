cmake_minimum_required(VERSION 3.3)
project(bdt_generator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp BDTree.cpp BDTree.h BDTNode.cpp BDTNode.h BDTNode.cpp BDTNode.h managed_ptr.h BDTParent.cpp BDTParent.h)
add_executable(bdt_generator ${SOURCE_FILES})