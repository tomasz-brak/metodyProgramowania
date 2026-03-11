cmake_minimum_required(VERSION 3.23)

set (CMAKE_CXX_STANDARD 23)

project(ProjectName)

add_executable(MyProgram)

target_include_directories(MyProgram PRIVATE ./lib)

target_sources(MyProgram
  PRIVATE
    Brak_Tomasz_Program_01.cpp
    lib/Engine.h
    lib/Engine.cpp
    lib/Engine.inl
    lib/EngineSEAK.cpp
    lib/EngineSEAK.h
    lib/EngineSEB.cpp
    lib/EngineSEB.h
    lib/Files.h
    lib/Files.cpp
    lib/Files.inl
    lib/array.h
)
