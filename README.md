# Simple-UEFI-bootloader
A simple higher half x86_64 bootloader.
## Building
```
mkdir build
cd build
cmake ..
#either copy your OVMF image in build/bin or run cmake.. again
cmake --build .
```
## Running the test
```
cmake --build . --target run
```