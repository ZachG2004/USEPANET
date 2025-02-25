#/bin/zsh
cd ~/Documents/USEPANET/Homework_Assignment_04/debug

g++ \
-Wall -O2 -v -std=c++20 -o hw4net_dbg \
~/Documents/USEPANET/Homework_Assignment_04/homework4run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2 \
| tee -a ../logs/debug/"debug_$(date +%F_%R).log"  2>&1
