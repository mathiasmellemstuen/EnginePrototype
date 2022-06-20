@echo off
del engine.exe
cd build/
cmake ..
make
copy /B "ENGINEPROTOTYPE.exe" "../engine.exe"
cd ../
start /B engine.exe