cmake_minimum_required(VERSION 2.8)
project( Gazo-san )
set (CMAKE_CXX_STANDARD 11)
set(GTEST OFF CACHE BOOL "Test flag")
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

if(GTEST)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
  if(NOT EXISTS "${CMAKE_SOURCE_DIR}/tests/googletest")
    execute_process(
      COMMAND bash tests/build_googletest.sh
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  endif()
  set (gtest ${CMAKE_SOURCE_DIR}/tests/googletest/libgtest.a)
  set (gtest_main ${CMAKE_SOURCE_DIR}/tests/googletest/libgtest_main.a)
  set (LIBRARIES_FOR_TEST ${OpenCV_LIBS} ${gtest} ${gtest_main} -lpthread)
  include_directories( src/ )
  include_directories( include/ )
  include_directories( tests/googletest/include/ )
  file(DOWNLOAD https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp ${CMAKE_SOURCE_DIR}/include/cxxopts.hpp)

  if( ${TEST_LEVEL} STREQUAL "unit" )
    add_executable(gazosan_unit_test tests/unit_tests/imageDiffCalc_test.cpp)
    target_link_libraries( gazosan_unit_test ${LIBRARIES_FOR_TEST} )
  elseif( ${TEST_LEVEL} STREQUAL "integration" )
    add_executable( gazosan_integration_test tests/integration_tests/imageDiffCalc_test.cpp )
    target_link_libraries( gazosan_integration_test ${LIBRARIES_FOR_TEST} )
  elseif( ${TEST_LEVEL} STREQUAL "system" )
    add_executable( gazosan_system_test tests/system_tests/imageDiffCalc_test.cpp )
    target_link_libraries( gazosan_system_test ${LIBRARIES_FOR_TEST} )
  endif()
else()
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
  set (BIN_NAME gazosan )

  if(EXISTS "${CMAKE_SOURCE_DIR}/libimageDiffCalc.a")
    # Use static link library file
    # Works only on ubuntu
    add_executable( ${BIN_NAME} src/main.cpp )
    target_link_libraries( ${BIN_NAME} ${OpenCV_LIBS} ${CMAKE_SOURCE_DIR}/libimageDiffCalc.a )
  else()
    # Build with source code
    # Works on linux machine
    file(DOWNLOAD https://raw.githubusercontent.com/jarro2783/cxxopts/master/include/cxxopts.hpp ${CMAKE_SOURCE_DIR}/include/cxxopts.hpp)
    include_directories( include/ )
    add_library(imageDiffCalc STATIC src/imageDiffCalc.cpp )
    add_executable( ${BIN_NAME} src/main.cpp )
    target_link_libraries( ${BIN_NAME} ${OpenCV_LIBS} imageDiffCalc )
  endif()
endif()