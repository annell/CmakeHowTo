cmake_minimum_required(VERSION 3.17)
project(FunctionLibb CXX)
message("----------------${PROJECT_NAME}----------------")

set(${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR} CACHE INTERNAL "extra_includes")


add_library(${PROJECT_NAME} SHARED
        Functions.cpp
        )

message("-- Adding gtest")

include(ExternalProject)
ExternalProject_Add(googletest
        GIT_REPOSITORY git://github.com/google/googletest
        GIT_TAG main
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )

enable_testing()

add_executable(RunTests
        test/test_ValidateLibraryFunctions.cpp
        )
add_dependencies(RunTests
        googletest
        )

target_link_libraries(RunTests PRIVATE
        ${PROJECT_NAME}
        gtest
        gtest_main
        )
target_include_directories(RunTests PRIVATE
        ${EXTERNAL_INSTALL_LOCATION}/include
        .
        )

include(GoogleTest)
gtest_discover_tests(RunTests)
