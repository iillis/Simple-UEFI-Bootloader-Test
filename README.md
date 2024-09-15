# Simple-UEFI-bootloader
A simple higher half x86_64 bootloader.
## Building
```
mkdir build
cd build
cmake ..
#an OVMF image will automatically be copied
cmake --build .
```
## Running the test
```
cmake --build . --target run
```
