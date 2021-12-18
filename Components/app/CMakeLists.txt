cmake_minimum_required(VERSION 3.17)
project(app CXX)
message("----------------${PROJECT_NAME}----------------")
message("Extra includes: " ${FunctionLibb_INCLUDES})
message("Extra includes: " ${YamlLibb_INCLUDES})

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
