# A developer's take on How to use cmake
The buildsystem was always my softspot when writing code, my approach was always to google how to do it and copy paste code and try to get it to run. This is an attempt to take a step back and actually try to learn how cmake work, so that I can actually understand what's going on.
So I will continue to add in random methods of linking in librarys to try them out, just to learn how to organize bigger projects with several types of external components and will use this as a reference for the future.


## Structure

```
CmakeHowTo
|-- CMakeList.txt
    |
    Components
        |
        |-- App
        |   |-- CmakeList.txt
        |   |-- main.cpp
        |
        |-- FunctionLibb
        |   |-- CmakeList.txt
        |   |-- Functions.cpp
        |   |-- Functions.h
        |   |
        |   |-- test
        |       |-- test_ValidateLibraryFunctions.cpp
        |
        |-- YamlLibb
            |-- CmakeList.txt
            |-- Yaml.cpp
            |-- Yaml.h
```

So the idea is to have all the different components split up into their own parts, and the `root` `CMakeList.txt` includes all components, the components are then themself responsible of linking up against it's dependencies.

## Use cases
The specific use cases are described below for my own convenience, or else I will forget about it.
<details><summary><font size="4"><b>Order of component include matters</b></font></summary><p>
Since I want to use the functionlibb in the app, I need to in the root add the FunctionLibb before I add App, such as:

```
add_subdirectory(Components/FunctionLibb)
add_subdirectory(Components/app)
```

</p></details>

<details><summary><font size="4"><b>Use FunctionLibb in App</b></font></summary><p>

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

</p></details>

<details><summary><font size="4"><b>Add a external library from github</b></font></summary><p>

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

</p></details>

<details><summary><font size="4"><b>Add Google test to CMake</b></font></summary><p>

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
</p></details>

<details><summary><font size="4"><b>Dynamic vs Static library</b></font></summary><p>

The two main library types (that I know of?) are:
- Dynamic library, called `libgtest.dylib` on mac, `libgtest.dll` on windows and `libgtest.so` on UNIX.
- Static library, called `libgtest.a` on mac / UNIX and `libtest.lib` on windows.

Where static libraries are a little bit easier to use as they compile everything together to a big binary file with no external links.
For dynamic libraries, you also need the libraries external references as it will not be compiled into the same binary.

This reduces overall size and increases flexibility of the dynamic library, but makes it a bit more annoying to work with.
In cmake it is very easy to build your library as a dynamic (shared library), just do the following:
```
add_library(${PROJECT_NAME} SHARED Functions.cpp)
```

And for building the a static library change the `SHARED` keyword to `STATIC` such as:
```
add_library(${PROJECT_NAME} SHARED Functions.cpp````)
```

And when linking in the shared vs static library into your app you do exactly the same thing for both of them:
```
add_executable(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME}
    YamlLibb
    FunctionLibb
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    FunctionLibb
    YamlLibb
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${FunctionLibb_INCLUDES}
    ${YamlLibb_INCLUDES}
)
```

With the caveat that the shared library (`FunctionLibb` is shared and `YamlLibb` is static) also would need it's external dependencies also linked in, but in this example `FunctionLibb` does not have any external dependencies.

</p></details>

<details><summary><font size="4"><b>Target "yaml-cpp" of type UTILITY may not be linked into another target</b></font></summary><p>

When I was linking in the Yaml-cpp library I kept running into
> CMake Error at Components/YamlLibb/CMakeLists.txt:24 (target_link_libraries):
Target "yaml-cpp" of type UTILITY may not be linked into another target.
One may link only to INTERFACE, OBJECT, STATIC or SHARED libraries, or to
executables with the ENABLE_EXPORTS property set

And when searching around for it, the explanation I found was that cmake couldn't resolve the path to the yaml-cpp lib
So instead of doing:

```
target_link_libraries(${PROJECT_NAME} PRIVATE
    yaml-cpp
)
```
I had to explicitly point out to cmake where the static library was like so:
```
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${EXTERNAL_INSTALL_LOCATION}/lib/libyaml-cpp.a
)
```
But then it worked like a charm!

</p></details>

<details><summary><font size="4"><b>Make IDE detect externally downloaded libraries</b></font></summary><p>

It is very nice if you can setup your IDE to work with all external libraries, so your intellisense works correctly and your includes work.
How I made this work in this project was to use the `include_directories(...)` directive in cmake.
So in my root's cmake i do the following, to setup the path where I put all my external libraries and then tell the IDE where to find all includes:
```
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
```

So with this small maneuver I was able to get the IDE understand about my includes.

</p></details>
