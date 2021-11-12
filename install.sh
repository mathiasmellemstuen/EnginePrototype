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
cd ../imgui/imgui
cp backends/imgui_impl_sdl.cpp imgui_impl_sdl.cpp
cp backends/imgui_impl_sdl.h imgui_impl_sdl.h
cp backends/imgui_impl_vulkan.cpp imgui_impl_vulkan.cpp
cp backends/imgui_impl_vulkan.h imgui_impl_vulkan.h
rm -r backends/
rm -r examples/
rm -r docs/
rm -r misc/

echo "Setting up cpp-data-parser"
cd ../../cpp-data-parsing/src
rm main.cpp