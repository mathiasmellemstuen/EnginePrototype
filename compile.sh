#!/bin/sh
ALL_CPP_FILES=$(find "$(cd src/; pwd)" | grep .cpp)
g++ -std=c++17 include/imgui/backends/imgui_impl_sdl.cpp include/imgui/backends/imgui_impl_vulkan.cpp include/imgui/imgui*.cpp  -I./include -I/vulkan-sdk/1.2.176.1/x86_64/include -L/vulkan-sdk/1.2.176.1/x86_64/lib -L./ $ALL_CPP_FILES -lvulkan -lSDL2main -lSDL2 -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -o engine
