mkdir build2
cd build2/

{
  cmake ..
  make
} && {
  cd ..
  ./build2/ENGINEPROTOTYPE
}
