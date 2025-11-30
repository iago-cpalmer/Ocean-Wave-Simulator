cd ../
mkdir build
cd build

# Build in debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --parallel --config Debug

# Run
cd src/Debug/
.\OceanWavesSimulator

# Leave
cd ../../../scripts