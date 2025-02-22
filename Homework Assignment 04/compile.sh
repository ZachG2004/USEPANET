#/bin/zsh
cd ~/Documents/USEPANET/Homework\ Assignment\ 04/builds

g++ \
-o hw4net_cmp \
~/Documents/USEPANET/Homework\ Assignment\ 04/homework4run.cpp \
~/Documents/USEPANET/include/handlerforEPANET.cpp \
-L/Users/zachfrost/Applications/EPANET/build/lib \
-lepanet2
