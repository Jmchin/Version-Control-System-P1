#!/bin/bash

# clean the directory
rm -f *.out *.o *.s
rm -f cli_vcs

# compile the programn
g++ -Wall -std=c++11 -o cli_vcs -lboost_filesystem -lboost_system  manifest.cpp versionControl.cpp util.cpp repo.cpp
