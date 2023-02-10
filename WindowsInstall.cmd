mkdir build 
cd build
conan install .. --build missing -s build_type=Release
cmake ..
cmake --build . --config Release
cd ..
pause