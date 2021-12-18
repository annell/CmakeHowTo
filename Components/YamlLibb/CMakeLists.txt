cmake_minimum_required(VERSION 3.17)
project(YamlLibb CXX)
message("----------------${PROJECT_NAME}----------------")

set(${PROJECT_NAME}_INCLUDES ${PROJECT_SOURCE_DIR} CACHE INTERNAL "extra_includes")


message("-- Adding yaml-cpp")

include(ExternalProject)
ExternalProject_Add(yaml-cpp
        GIT_REPOSITORY git@github.com:jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.7.0
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )

add_library(${PROJECT_NAME} STATIC Yaml.cpp)
add_dependencies(${PROJECT_NAME} yaml-cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        ${EXTERNAL_INSTALL_LOCATION}/include
        )

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${EXTERNAL_INSTALL_LOCATION}/lib/libyaml-cpp.a
        )
