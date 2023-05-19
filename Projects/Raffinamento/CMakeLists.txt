cmake_minimum_required(VERSION 3.9)

project(raffinamento LANGUAGES CXX C VERSION 1.0.0)

# Set C++ Standard
################################################################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# IMPOSE WARNINGS ON DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic-errors")

# IMPOSE CXX FLAGS FOR WINDOWS
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
endif (WIN32)

# Add dependencies
################################################################################
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../externals/Main_Install/eigen3)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/../../externals/Main_Install/googletest)

## Eigen3
find_package(Eigen3 CONFIG REQUIRED)
list(APPEND raffinamento_LINKED_LIBRARIES PUBLIC Eigen3::Eigen)

## Threads
find_package(Threads REQUIRED)
list(APPEND raffinamento_LINKED_LIBRARIES PRIVATE Threads::Threads)

## GTest
find_package(GTest REQUIRED)

list(APPEND raffinamento_test_LINKED_LIBRARIES PRIVATE GTest::gtest)
list(APPEND raffinamento_test_LINKED_LIBRARIES PRIVATE GTest::gtest_main)
list(APPEND raffinamento_test_LINKED_LIBRARIES PRIVATE GTest::gmock)
list(APPEND raffinamento_test_LINKED_LIBRARIES PRIVATE GTest::gmock_main)

# Insert Sources
################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

list(APPEND raffinamento_SOURCES ${raffinamento_sources})
list(APPEND raffinamento_HEADERS ${raffinamento_headers})
list(APPEND raffinamento_INCLUDE ${raffinamento_includes})

# Insert Tests
################################################################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)

list(APPEND raffinamento_test_HEADERS ${raffinamento_test_headers})
list(APPEND raffinamento_test_INCLUDE ${raffinamento_test_includes})

# Create executable
################################################################################

add_executable(raffinamento_test
	main_test.cpp
	${raffinamento_test_HEADERS}
	${raffinamento_test_INCLUDE}
	${raffinamento_SOURCES}
    ${raffinamento_HEADERS})

target_link_libraries(raffinamento_test ${raffinamento_LINKED_LIBRARIES})
target_link_libraries(raffinamento_test ${raffinamento_test_LINKED_LIBRARIES})
target_include_directories(raffinamento_test PRIVATE ${raffinamento_INCLUDE} ${raffinamento_test_INCLUDE})
target_compile_options(raffinamento_test PUBLIC -fPIC)

gtest_discover_tests(raffinamento_test)

add_executable(raffinamento_program
	main_program.cpp
	${raffinamento_SOURCES}
    ${raffinamento_HEADERS})

target_link_libraries(raffinamento_program ${raffinamento_LINKED_LIBRARIES})
target_include_directories(raffinamento_program PRIVATE ${raffinamento_INCLUDE})
target_compile_options(raffinamento_program PUBLIC -fPIC)
