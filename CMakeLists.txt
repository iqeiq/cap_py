cmake_minimum_required(VERSION 2.8)

project(capy CXX)

ENABLE_LANGUAGE(C)

set(TARGET ${PROJECT_NAME})

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif()

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(CMAKE_CXX_FLAGS "-Wall /W2 /EHsc /MP")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /MTd /Zi")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG /O2 /MT")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost REQUIRED COMPONENTS filesystem python)

include_directories(
  ${Boost_INCLUDE_DIR}
  C:/boost.numpy/include
)

link_directories(
  ${Boost_LIBRARY_DIRS}
  C:/boost.numpy/lib
)


include_directories(
  $ENV{PYTHON_DIR}/include
)
link_directories(
  $ENV{PYTHON_DIR}/libs
)

# GLOB_RECURSE
file(GLOB CXX_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.hpp
)
add_library(${TARGET} SHARED ${CXX_SOURCE_FILES})

target_link_libraries(${TARGET} 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_PYTHON_LIBRARY}
  boost_numpy
  python34
)
