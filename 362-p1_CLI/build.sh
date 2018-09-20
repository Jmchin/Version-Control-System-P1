#!/bin/bash

# clean the directory
rm -f *.out *.o *.s

# compile the programn
g++ -std=c++11 -o cli_vcs versionControl.cpp -lboost_filesystem -lboost_system

# run the progam
./cli_vcs
