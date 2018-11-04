#!/bin/bash

# clean the directory
rm -f *.out *.o *.s
rm -f cli_vcs
rm -f manifest-test

# compile the programn
g++ -Wall -std=c++11 -o cli_vcs -lboost_filesystem -lboost_system  manifest.cpp versionControl.cpp

# compile the manifest test file
# g++ -Wall -std=c++11 -o manifest-test -lboost_filesystem -lboost_system manifest.cpp

# run the progam
./cli_vcs
