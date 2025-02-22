#/bin/zsh
cd ~/Documents/USEPANET/Homework\ Assignment\ 04/builds/logs

g++ \
-Wall -O2 -v -std=c++20 \
~/Documents/USEPANET/Homework\ Assignment\ 04/homework4run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2 \
| tee -a "debug_$(date +%F_%R).log"  2>&1
