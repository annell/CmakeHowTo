# A developer's take on How to use cmake
The buildsystem was always my softspot when writing code, my approach was always to google how to do it and copy paste code and try to get it to run. This is an attempt to take a step back and actually try to learn how cmake work, so that I can actually understand what's going on.
So I will continue to add in random methods of linking in librarys to try them out, just to learn how to organize bigger projects with several types of external components and will use this as a reference for the future.


## Structure

```
root
    CMakeList.txt
    |
    Components
        |
        |-- App
        |   |-- CmakeList.txt
        |   |-- main.cpp
        |
        |-- FunctionLibb
            |-- CmakeList.txt
            |-- Functions.cpp
            |-- Functions.h
            |
            |-- test
                |-- test_ValidateLibraryFunctions.cpp
```

So the idea is to have all the different components split up into their own parts, and the `root` `CMakeList.txt` includes all components, the components are then themself responsible of linking up against it's dependencies.

## Use cases
The specific use cases are described below for my own convenience, or else I will forget about it.
### Order of component include matters
Since I want to use the functionlibb in the app, I need to in the root add the FunctionLibb before I add App, such as:
```
add_subdirectory(Components/FunctionLibb)
add_subdirectory(Components/app)
```
### Use FunctionLibb in App
The simple use case of linking a component together with another is done as such:
```
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC FunctionLibb)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${FunctionLibb_INCLUDES}
        )
```

Where the `${FunctionLibb_INCLUDES}` variable is something that is defined and cached in the FunctionLibb components `CMakeList.txt` by doing the following:
```
set(${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR} CACHE INTERNAL "extra_includes")
```
I've found that it's an okay way of communicating the components includes.

And building the library is as easy as:
```
add_library(${PROJECT_NAME} SHARED Functions.cpp)
```

### Add a external library from github
The external library I wanted to add was [googletest](https://github.com/google/googletest), to do that I used the CMake module `ExternalProject`, but then it was as easy as:
```
include(ExternalProject)
ExternalProject_Add(googletest
        GIT_REPOSITORY git://github.com/google/googletest
        GIT_TAG main
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )
```
and after calling `add_dependencies(RunTests googletest)`, the cmake system will try to download the specified tag and build it in the cmake build folders when the `RunTests` project is built - Super simple and neat!


### Add Google test to CMake
When building google tests, there are a few parts that needs to be done.
1. Create a executable to run the tests.
2. Add googletest as a dependency for the executable.
3. Link together executable, googletest and the library that you want to test.
4. Add the gtest framework boilerplate code to the `CMakeList.txt` file.

In my case I chose to place the tests in a `test` folder to make it more easily overviewable - And this all could look something like:
```
enable_testing()

add_executable(RunTests test/test_ValidateLibraryFunctions.cpp)
add_dependencies(RunTests googletest)

target_link_libraries(RunTests ${PROJECT_NAME} gtest gtest_main)
target_include_directories(RunTests PRIVATE
        ${EXTERNAL_INSTALL_LOCATION}/include
        .
        )

include(GoogleTest)
gtest_discover_tests(RunTests)
```
