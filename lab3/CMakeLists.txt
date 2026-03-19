cmake_minimum_required(VERSION 3.23)
project(ProjectName)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.c"
)

file(GLOB_RECURSE LIB_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/lib/*.inl"
)

add_executable(MyProgram
  main.cpp
  ${LIB_SOURCES}
  ${LIB_HEADERS}
)

target_include_directories(MyProgram PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
