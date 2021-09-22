cmake_minimum_required(VERSION 3.13)
project(ENGINEPROTOTYPE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.h" "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE IMGUI "${CMAKE_SOURCE_DIR}/thirdparty/imgui/*.h" "${CMAKE_SOURCE_DIR}/thirdparty/imgui/imgui/*.cpp")
file(GLOB_RECURSE CPPDATAPARSER "${CMAKE_SOURCE_DIR}/thirdparty/cpp-data-parser/*.h" "${CMAKE_SOURCE_DIR}/thirdparty/cpp-data-parser/*cpp")

add_executable(ENGINEPROTOTYPE ${SRC} ${IMGUI} ${CPPDATAPARSER})

target_include_directories(ENGINEPROTOTYPE  PUBLIC "${CMAKE_SOURCE_DIR}/src"
                                            PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/imgui/"
                                            PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/glm/"
                                            PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/rang/include/"
                                            PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/stb/"
                                            PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/tinyobjloader"
                                            PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/cpp-data-parser/")
find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "/vulkan-sdk/1.2.176.1/x86_64/include")
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

find_package(SDL2 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
