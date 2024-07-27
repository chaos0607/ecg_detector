# tests/CMakeLists.txt
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(test_source ${TEST_SOURCES})
  get_filename_component(test_name ${test_source} NAME_WE)
  add_executable(${test_name} ${test_source})
  target_link_libraries(${test_name} iir::iir)
endforeach()


target_sources(nwa_test PRIVATE ../src/detectors/common.cpp)