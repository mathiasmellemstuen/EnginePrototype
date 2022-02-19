@echo off

:: call :subModDown
:: call :install_SDL
:: call :setup_imgui
:: call :setup_dataParser
call :setup_dir
call :copy

EXIT /B

:: Functions ::
:subModDown
    echo Downloading submodules
    git submodule update --init --recursive
    git pull --recurse-submodules
    echo .
EXIT /B

:install_SDL
    echo Installing SDL
    cd thirdparty/SDL/include
    mkdir SDL2
    xcopy . SDL2
    cd ../VisualC/SDL

    :: msbuild /P:Configuration=Release /p:Platform=x64 SDL.vcxproj
    :: cd x64/Release/
    :: copy SDL2.lib "../../../../../../SDL2.lib"
    :: copy SDL2.dll "../../../../../../SDL2.dll"
    :: cd ../../../SDLmain/
    :: msbuild /P:Configuration=Release /p:Platform=x64 SDLmain.vcxproj
    :: cd x64/Release/
    :: copy SDL2main.lib "../../../../../../SDL2main.lib"
    
    :: Return to root
    cd ../../../../
    :: echo %CD%
    echo .
EXIT /B

:setup_imgui
    echo Setting up imgui
    cd thirdparty/imgui/
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
    
    :: Return to root
    cd ..
    echo .
EXIT /B

:setup_dataParser
    echo Setting up data parser
    cd thirdparty/cpp-data-parsing/src
    del main.cpp

    :: Return to root
    cd ../../../
    echo .
EXIT /B

:setup_dir
    echo Set up dir
    mkdir include
    cd thirdparty
    for /d %%d in (*.*) do call :create_dir %%d
    cd ..
    echo .
EXIT /B

:create_dir
    echo create dir %*
    cd ../include
    mkdir %*
    cd ../thirdparty
EXIT /B

:other_thing
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
EXIT /B

:: Copy stuf ::
:copy
    cd thirdparty
    call :copy_SDL
    call :copy_stb
    call :copy_cpp-data-parsing
    call :copy_imgui
    call :copy_rang
    call :copy_glm
    call :copy_tinyobjloader
    call :copy_implot
    call :copy_rapidxml
EXIT /B

:copy_SDL
    cd SDL
    mkdir include
    cd include
    xcopy "*.h" "../../../include/SDL2/" /sy 
    cd ../..
EXIT /B

:copy_stb
    cd stb
    xcopy "*.h" "../../include/stb/" /sy
    cd ..
EXIT /B

:copy_cpp-data-parsing
    cd cpp-data-parsing/src
    xcopy "*.h" "../../../include/cpp-data-parsing/" /sy
    cd ../..
EXIT /B

:copy_imgui
    cd imgui
    xcopy "*.h" "../../include/imgui/" /sy
    cd ..
EXIT /B

:copy_rang
    cd rang/include
    xcopy "*.hpp" "../../../include/rang/" /sy
    cd ../..
EXIT /B

:copy_glm
    cd glm/glm
    xcopy "*.h" "../../../include/glm/" /sy
    xcopy "*.hpp" "../../../include/glm/" /sy
    xcopy "*.inl" "../../../include/glm/" /sy
    cd ../..
EXIT /B

:copy_tinyobjloader
    cd tinyobjloader
    xcopy "*.h" "../../include/tinyobjloader/" /sy
    cd ..
EXIT /B

:copy_implot
    cd implot
    xcopy "*.h" "../../include/imgui/" /sy
    xcopy "*.h" "../imgui/" /sy
    xcopy "*.cpp" "../imgui/" /sy
    cd ..
EXIT /B

:copy_rapidxml
    cd rapidxml
    xcopy "*.hpp" "../../include/rapidxml/" /sya
    cd ..
EXIT /B