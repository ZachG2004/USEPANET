#/bin/zsh
cd ~/Documents/USEPANET/project_01_Work/debug

g++ \
-Wall -O2 -v -std=c++20 -o prok01net_dbg \
~/Documents/USEPANET/project_01_Work/project01run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2 \
| tee -a ../logs/debug/"debug_$(date +%F_%R).log"  2>&1
