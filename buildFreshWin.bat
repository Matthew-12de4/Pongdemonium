if exist out ( rd /s /q "out" )
if not exist out (mkdir out)
cd out
cmake ..
cmake --build .
cd Debug
start pong.exe
