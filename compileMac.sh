#!/bin/sh
ALL_CPP_FILES=$(find "$(cd src/; pwd)" | grep .cpp)
g++ -std=c++17 include/imgui/backends/imgui_impl_sdl.cpp include/imgui/backends/imgui_impl_vulkan.cpp include/imgui/imgui*.cpp  -I./include -I/Users/tobiashallingstad/VulkanSDK/1.2.189.0/macOS/include -L/Users/tobiashallingstad/VulkanSDK/1.2.189.0/macOS/lib -L./ $ALL_CPP_FILES -lvulkan -lSDL2main -lSDL2 -ldl -lpthread -o engine
