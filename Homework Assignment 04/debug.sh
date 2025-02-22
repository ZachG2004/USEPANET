#/bin/zsh
cd ~/Documents/USEPANET/Homework\ Assignment\ 04/builds/debug

g++ \
-Wall -O2 -v \
~/Documents/USEPANET/Homework\ Assignment\ 04/homework4run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2 \
2>&1 | tee -a "debug_$(date +%F_%R).log"
