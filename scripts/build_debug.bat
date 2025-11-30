cd ../build

# Build in debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --parallel --config Debug

# Leave
cd ../scripts