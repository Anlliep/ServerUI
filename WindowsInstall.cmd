rd /q /s build
rd /q /s bin
mkdir build 
cd build
conan install .. -s build_type=Debug --build missing
cmake ..
cmake --build . --config Debug
cd ..
pause