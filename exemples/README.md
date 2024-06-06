# How to compile

`g++ main.cpp -o main -I/home/pi/pantilthatDriver/src/impl -L/home/pi/pantilthatDriver/build-dir -lpantilt -lwiringPi`

# How to run
```
export LD_LIBRARY_PATH=/home/pi/pantilthatDriver/build-dir:$LD_LIBRARY_PATH
./main
unset LD_LIBRARY_PATH
```
