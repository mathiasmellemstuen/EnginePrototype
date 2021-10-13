cd build/
cmake ..
cmake --build . --target ENGINEPROTOTYPE --config Release
cd Release/
copy /B "ENGINEPROTOTYPE.exe" "../../engine.exe"
cd ../../
engine.exe