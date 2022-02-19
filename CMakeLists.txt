cmake_minimum_required(VERSION 3.13)
project(ENGINEPROTOTYPE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
add_definitions(-DSDL_MAIN_HANDLED)

file(GLOB_RECURSE SRC "${CMAKE_SOURCE_DIR}/src/*.h" "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE IMGUI "${CMAKE_SOURCE_DIR}/thirdparty/imgui/*.h" "${CMAKE_SOURCE_DIR}/thirdparty/imgui/*.cpp")
# file(GLOB_RECURSE IMPLOT "${CMAKE_SOURCE_DIR}/thirdparty/implot/*.h" "${CMAKE_SOURCE_DIR}/thirdparty/implot/*.cpp")
file(GLOB_RECURSE CPPDATAPARSER "${CMAKE_SOURCE_DIR}/thirdparty/cpp-data-parsing/src/*.h" "${CMAKE_SOURCE_DIR}/thirdparty/cpp-data-parsing/src/*cpp")

add_executable(ENGINEPROTOTYPE ${SRC} ${IMGUI} ${CPPDATAPARSER} src/graphics/camera.cpp src/graphics/camera.h src/graphics/renderInstance.h src/graphics/renderInstance.cpp src/mathematics/lerp.h src/mathematics/lerp.cpp src/graphics/layoutBinding.h src/graphics/Face.cpp src/graphics/Face.h)

target_include_directories(ENGINEPROTOTYPE PUBLIC "${CMAKE_SOURCE_DIR}/include")
target_include_directories(ENGINEPROTOTYPE PUBLIC "${CMAKE_SOURCE_DIR}/thirdparty/SDL/include")

find_package(Vulkan REQUIRED)
# find_package(SDL2 REQUIRED)

set(SDL2_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/thirdparty/SDL2-2.0.20/x86_64-w64-mingw32/lib")

# target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
# target_include_directories(${PROJECT_NAME} PRIVATE "/vulkan-sdk/1.2.176.1/x86_64/include")


#target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
if (${WIN32})
    #set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/SDL2.lib;${CMAKE_SOURCE_DIR}/SDL2main.lib")
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY_DIR}/libSDL2main.a ${SDL2_LIBRARY_DIR}/libSDL2.dll.a)
endif()
if (${APPLE})
    set(SDL2_LIBRARIES "/usr/local/lib/libSDL2.dylib;/usr/local/lib/libSDL2main.a")
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
endif()
# if (${UNIX} AND NOT ${APPLE})
#     # TODO: Set SDL2_LIBRARIES for linux
# endif()

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)