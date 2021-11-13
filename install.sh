#!/bin/sh

echo "Downloading submodules." 
git submodule update --init --recursive
git pull --recurse-submodules

echo "Installing SDL library on your system"
cd thirdparty/SDL
./configure
make
sudo make install

echo "Setting up imgui"
cd ../imgui/
cp backends/imgui_impl_sdl.cpp imgui_impl_sdl.cpp
cp backends/imgui_impl_sdl.h imgui_impl_sdl.h
cp backends/imgui_impl_vulkan.cpp imgui_impl_vulkan.cpp
cp backends/imgui_impl_vulkan.h imgui_impl_vulkan.h
rm -r backends/
rm -r examples/
rm -r docs/
rm -r misc/

echo "Setting up cpp-data-parser"
cd ../cpp-data-parsing/src
rm main.cpp

cd ../../
# (cd src && find . -name '*.h' -print | tar --create --files-from -) | (cd dst && tar xvfp -)
mkdir ../include
mkdir ../include/SDL2
mkdir ../include/stb
mkdir ../include/cpp-data-parsing
mkdir ../include/imgui
mkdir ../include/rang
mkdir ../include/tinyobjloader
mkdir ../include/glm

(cd SDL/include && find . -name '*.h' -print | tar --create --files-from -) | (cd ../include/SDL2 && tar xvfp -)
(cd stb/ && find . -name '*.h' -print | tar --create --files-from -) | (cd ../include/stb/ && tar xvfp -)
(cd cpp-data-parsing/src/ && find . -name '*.h' -print | tar --create --files-from -) | (cd ../include/cpp-data-parsing/ && tar xvfp -)
(cd imgui/ && find . -name '*.h' -print | tar --create --files-from -) | (cd ../include/imgui/ && tar xvfp -)
(cd rang/include/ && find . -name '*.hpp' -print | tar --create --files-from -) | (cd ../include/rang/ && tar xvfp -)
(cd glm/glm/ && find . -name '*.hpp' -print | tar --create --files-from -) | (cd ../include/glm/ && tar xvfp -)
(cd glm/glm/ && find . -name '*.h' -print | tar --create --files-from -) | (cd ../include/glm/ && tar xvfp -)
(cd glm/glm/ && find . -name '*.inl' -print | tar --create --files-from -) | (cd ../include/glm/ && tar xvfp -)
(cd tinyobjloader/ && find . -name '*.h' -print | tar --create --files-from -) | (cd ../include/tinyobjloader/ && tar xvfp -)