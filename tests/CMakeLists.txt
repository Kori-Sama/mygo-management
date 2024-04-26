cmake_minimum_required(VERSION 3.13)

project(test)

set(GOOGLETEST_VERSION 1.14.0)
add_subdirectory(${THIRD_PARTY_DIR}/googletest googletest)

add_executable(test_http ${CMAKE_CURRENT_SOURCE_DIR}/test_http.cpp)
target_include_directories(test_http PUBLIC ${THIRD_PARTY_DIR}/googletest/include)
target_include_directories(test_http PUBLIC ${SRC_DIR}/http)
target_link_libraries(test_http gtest gtest_main Http)

add_test(NAME test_http COMMAND test_http)