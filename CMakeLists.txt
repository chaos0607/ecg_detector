cmake_minimum_required(VERSION 3.8.0)
project (Engzee LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wconversion -Wextra -pedantic)


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libs)

find_package(iir)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

#add_executable(main ${SOURCES})
#target_link_libraries(main iir::iir)

add_executable(demo src/demo.cpp)
target_link_libraries(demo iir::iir)

add_subdirectory(tests)
