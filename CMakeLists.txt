cmake_minimum_required(VERSION 3.17)
project(CmakeHowTo CXX)
message("----------------${PROJECT_NAME}----------------")
set(CMAKE_CXX_STANDARD 17)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
message(EXTERNAL_INSTALL_LOCATION: ${EXTERNAL_INSTALL_LOCATION})

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

add_subdirectory(Components/FunctionLibb)
add_subdirectory(Components/YamlLibb)
add_subdirectory(Components/app)

message("----- Done -----")