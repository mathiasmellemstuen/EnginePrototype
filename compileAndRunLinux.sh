source /vulkan-sdk/1.2.176.1/setup-env.sh
mkdir build
cd build/

{
  cmake ..
  make
} && {
  cd ..
  ./build/ENGINEPROTOTYPE
}
