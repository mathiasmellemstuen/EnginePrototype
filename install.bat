@echo off 
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"

echo "Downloading submodules." 
git submodule update --init --recursive
git pull --recurse-submodules

echo "Installing SDL library on your system"
cd thirdparty/SDL/include
mkdir SDL2
xcopy . SDL2
cd ../VisualC/SDL

msbuild /P:Configuration=Release /p:Platform=x64 SDL.vcxproj
cd x64/Release/
copy SDL2.lib "../../../../../../SDL2.lib"
copy SDL2.dll "../../../../../../SDL2.dll"
cd ../../../SDLmain/
msbuild /P:Configuration=Release /p:Platform=x64 SDLmain.vcxproj
cd x64/Release/
copy SDL2main.lib "../../../../../../SDL2main.lib"

echo "Setting up imgui"
cd ../../../../../imgui/
cd backends/
copy imgui_impl_sdl.cpp "../imgui_impl_sdl.cpp"
copy imgui_impl_sdl.h "../imgui_impl_sdl.h"
copy imgui_impl_vulkan.cpp "../imgui_impl_vulkan.cpp"
copy imgui_impl_vulkan.h "../imgui_impl_vulkan.h"
cd ..
Rmdir /S /q backends
Rmdir /S /q examples
Rmdir /S /q docs
Rmdir /S /q misc
cd ../
echo %cd%
echo "Setting up dataparser"
cd thirdparty/cpp-data-parsing/src
del main.cpp
cd ../../../

mkdir build

mkdir include
cd include/
mkdir SDL2
mkdir stb
mkdir cpp-data-parsing
mkdir imgui
mkdir rang
mkdir tinyobjloader
mkdir glm
mkdir implot

cd ../thirdparty

cd SDL/include
xcopy "*.h" "../../../include/SDL2/" /sy
cd ../../stb
xcopy "*.h" "../../include/stb/" /sy
cd ../cpp-data-parsing/src/
xcopy "*.h" "../../../include/cpp-data-parsing/" /sy
cd ../../imgui
xcopy "*.h" "../../include/imgui/" /sy
cd ../rang/include
xcopy "*.hpp" "../../../include/rang/" /sy
cd ../../glm/glm
xcopy "*.h" "../../../include/glm/" /sy
xcopy "*.hpp" "../../../include/glm/" /sy
xcopy "*.inl" "../../../include/glm/" /sy
cd ../../tinyobjloader
xcopy "*.h" "../../include/tinyobjloader/" /sy
cd ../implot
xcopy "*.h" "../../include/imgui/" /sy
xcopy "*.h" "../imgui/" /sy
xcopy "*.cpp" "../imgui/" /sy