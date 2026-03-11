cmake_minimum_required(VERSION 3.23)

set (CMAKE_CXX_STANDARD 23)

project(ProjectName)

add_executable(MyProgram)

target_include_directories(MyProgram PRIVATE ./lib)

target_sources(MyProgram
  PRIVATE
    main.cpp
)


