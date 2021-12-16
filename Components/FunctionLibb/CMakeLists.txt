cmake_minimum_required(VERSION 3.17)
project(FunctionLibb CXX)
message("----------------${PROJECT_NAME}----------------")

set(${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR} CACHE INTERNAL "extra_includes")

include(ExternalProject)

message("Adding gtest")
ExternalProject_Add(googletest
        GIT_REPOSITORY git://github.com/google/googletest
        GIT_TAG main
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(${PROJECT_NAME} SHARED Functions.cpp)
add_dependencies(${PROJECT_NAME} googletest)
target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main pthread)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${EXTERNAL_INSTALL_LOCATION}/include
        )

enable_testing()

add_executable(RunTests test/test_ValidateLibraryFunctions.cpp)

target_link_libraries(RunTests ${PROJECT_NAME} gtest gtest_main)
target_include_directories(RunTests PRIVATE
        ${EXTERNAL_INSTALL_LOCATION}/include
        .
        )

include(GoogleTest)
gtest_discover_tests(RunTests)
