# tests/CMakeLists.txt
file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/tests/*.cpp)

foreach(test_source ${TEST_SOURCES})
  get_filename_component(test_name ${test_source} NAME_WE)
  add_executable(${test_name} ${test_source})
  target_link_libraries(${test_name} iir)
  set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/output)
endforeach()


target_sources(mwa_test PRIVATE ../src/algorithms/mwa.cpp)
target_sources(twoaverage_test PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/TwoAverage.cpp)
target_sources(wqrs_test PRIVATE ../src/algorithms/mwa.cpp ../src/detectors/wqrs.cpp)
target_sources(engzee_test PRIVATE ../src/detectors/engzee.cpp)

file(GLOB_RECURSE SRC_FILES "../src/**/*.cpp")
target_sources(detect_on_test_dataset PRIVATE ${SRC_FILES} ../src/EcgDetector.cpp)