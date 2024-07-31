# tests/CMakeLists.txt
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(test_source ${TEST_SOURCES})
  get_filename_component(test_name ${test_source} NAME_WE)
  add_executable(${test_name} ${test_source})
  target_link_libraries(${test_name} iir::iir)
endforeach()


target_sources(mwa_test PRIVATE ../src/algorithms/mwa.cpp)
target_sources(twoaverage_test PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/TwoAverage.cpp)
target_sources(wqrs_test PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/wqrs.cpp)
target_sources(engzee_test PRIVATE ../src/detectors/engzee.cpp)

file(GLOB_RECURSE SRC_FILES "../src/**/*.cpp")
target_sources(detect_on_test_dataset PRIVATE ${SRC_FILES} ../src/EcgDetector.cpp)