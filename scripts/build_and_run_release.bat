cd ../build

# Build in debug
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --parallel --config Release

# Run
cd src/Release/
.\OceanWavesSimulator

# Leave
cd ../../../scripts