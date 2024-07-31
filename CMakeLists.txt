cmake_minimum_required(VERSION 3.8.0)
project (Engzee LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()


set(PROJECT_ROOT ${PROJECT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Wconversion)
endif()


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libs)

file(GLOB_RECURSE IIR_SOURCES ${PROJECT_SOURCE_DIR}/libs/iir/*.cpp)
file(GLOB_RECURSE IIR_HEADERS ${PROJECT_SOURCE_DIR}/libs/iir/*.h ${PROJECT_SOURCE_DIR}/libs/*.h)
add_library(iir STATIC ${IIR_SOURCES} ${IIR_HEADERS})

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(EcgDetector ${SOURCES})
target_link_libraries(EcgDetector iir)
add_definitions(-DPROJECT_ROOT="${PROJECT_ROOT}")

add_subdirectory(tests)
