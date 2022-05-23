del engine.exe
@REM call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
cd build/
cmake ..
cmake --build . --target ENGINEPROTOTYPE --config Release
cd Release/
copy /B "ENGINEPROTOTYPE.exe" "../../engine.exe"
cd ../../
engine.exe