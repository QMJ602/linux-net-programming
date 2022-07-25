#!/bin/bash
gcc -c main.cpp udp_client.cpp
g++ -o main main.o udp_client.o
./main
