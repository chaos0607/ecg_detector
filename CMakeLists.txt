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

find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
set(MAIN_FILES
    ${PROJECT_SOURCE_DIR}/src/OfflineDetector.cpp
    ${PROJECT_SOURCE_DIR}/src/OnlineDetector.cpp
)
list(REMOVE_ITEM SOURCES ${MAIN_FILES})

set(ONLINE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/OnlineDetector.cpp
    ${SOURCES}
)
set(OFFLINE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/OfflineDetector.cpp
    ${SOURCES}
)

add_executable(OnlineEcgDetector ${ONLINE_SOURCES})
add_executable(OfflineEcgDetector ${OFFLINE_SOURCES})

target_link_libraries(OfflineEcgDetector iir ${FLTK_LIBRARIES})
target_link_libraries(OnlineEcgDetector iir ${FLTK_LIBRARIES})

add_definitions(-DPROJECT_ROOT="${PROJECT_ROOT}")

add_subdirectory(tests)
