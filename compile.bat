@echo off
cd src/
set objs=
for /R %%f in (*.cpp) do call set objs=%%objs%% %%f
cd ..
g++ -std=c++17 %objs% -I./include -IC:/VulkanSDK/1.2.176.1/Include -LC:\VulkanSDK\1.2.176.1\Lib -o engine.exe -L./ -lglfw3 -lvulkan-1