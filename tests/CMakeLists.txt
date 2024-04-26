cmake_minimum_required(VERSION 3.13)

project(test)

set(GOOGLETEST_VERSION 1.14.0)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT}/bin/tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT}/bin/tests)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT}/bin/tests)

add_subdirectory(${THIRD_PARTY_DIR}/googletest googletest)

add_executable(test_utils ${CMAKE_CURRENT_SOURCE_DIR}/test_utils.cpp)
target_include_directories(test_utils PUBLIC ${THIRD_PARTY_DIR}/googletest/include)
target_include_directories(test_utils PUBLIC ${SRC_DIR}/utils)
target_link_libraries(test_utils gtest gtest_main Utils)
add_test(NAME test_utils COMMAND test_utils)

add_executable(test_http ${CMAKE_CURRENT_SOURCE_DIR}/test_http.cpp)
target_include_directories(test_http PUBLIC ${THIRD_PARTY_DIR}/googletest/include)
target_include_directories(test_http PUBLIC ${SRC_DIR}/http)
target_link_libraries(test_http gtest gtest_main Http)
add_test(NAME test_http COMMAND test_http)

