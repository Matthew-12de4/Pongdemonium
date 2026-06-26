if [ -d "out" ]; then sudo rm -rf out;
fi
if [ !  -d "out" ]; then sudo mkdir out;
fi
cd out
sudo cmake ..
sudo cmake --build .
./pong
