cmake_minimum_required(VERSION 3.17)
project(app CXX)
message("----------------${PROJECT_NAME}----------------")
message("Extra includes: " ${FunctionLibb_INCLUDES})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC FunctionLibb)
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_BINARY_DIR}
        ${FunctionLibb_INCLUDES}
        )
