if [ ! -d "out" ]; then sudo mkdir out;
fi
cd out
sudo cmake ..
sudo cmake --build .
./pong