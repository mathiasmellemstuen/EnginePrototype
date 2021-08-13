@echo off
cd src/
set objs=
for /R %%f in (*.cpp) do call set objs=%%objs%% %%f
cd ..
echo Taskkilling engine.exe if it's running
Taskkill /IM engine.exe /F
echo Starting compilation...
g++ -w -std=c++17 include/imgui/backends/imgui_impl_sdl.cpp include/imgui/backends/imgui_impl_vulkan.cpp include/imgui/imgui*.cpp %objs% -I./include -IC:/VulkanSDK/1.2.176.1/Include -LC:\VulkanSDK\1.2.176.1\Lib -L./lib -o engine.exe -lSDL2main -lSDL2 -lvulkan-1