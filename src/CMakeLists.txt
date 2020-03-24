cmake_minimum_required(VERSION 3.10)

project(tracer)

option(ENABLE_DEFAULT_WRITERS "Enables default console and file writers. Warning: This only enables compilation" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

if (ENABLE_DEFAULT_WRITERS)
  set(DEFAULT_WRITERS_SRC
    writers/ConsoleWriter.cpp
    writers/FileWriter.cpp)
endif()

add_library(libtracer
  Tracer.cpp
  TracerConfig.cpp
  TracerCommon.cpp
  WriterStorage.cpp
  GlobalTracer.cpp

  ${DEFAULT_WRITERS_SRC}
)

if (ENABLE_DEFAULT_WRITERS)
  target_include_directories(libtracer PUBLIC . writers)
endif()
