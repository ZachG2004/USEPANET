#/bin/zsh
cd ~/Documents/USEPANET/Homework_Assignment_04/builds

g++ \
-o hw4net_cmp -std=c++20 \
~/Documents/USEPANET/Homework_Assignment_04/homework4run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2 \
| tee -a ../logs/builds/"compile_$(date +%F_%R).log" 2>&1
