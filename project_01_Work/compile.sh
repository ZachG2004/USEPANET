#/bin/zsh
cd ~/Documents/USEPANET/project_01_Work/builds

g++ \
-o proj01net_cmp -std=c++20 \
~/Documents/USEPANET/project_01_Work/project01run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2 \
| tee -a ../logs/builds/"compile_$(date +%F_%R).log"  2>&1
