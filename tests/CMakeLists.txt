# tests/CMakeLists.txt
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)


#find_package(nlohmann_json 3.2.0 REQUIRED)


foreach(test_source ${TEST_SOURCES})
  get_filename_component(test_name ${test_source} NAME_WE)
  add_executable(${test_name} ${test_source})
  target_link_libraries(${test_name} iir::iir)
endforeach()


target_sources(mwa_test PRIVATE ../src/algorithms/mwa.cpp)
target_sources(twoaverage_test PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/TwoAverage.cpp)
target_sources(wqrs_test PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/wqrs.cpp)
target_sources(demo PRIVATE ../src/utils.cpp)
#target_sources(evaluate_detectors PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/wqrs.cpp ../src/detectors/TwoAverage.cpp)