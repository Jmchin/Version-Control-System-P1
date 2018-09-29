# Version-Control-System Project 1 #

### General Information ###
* CPSC 362 - #12846/12847
* Project 1
* Team CLI
  * Kevin Molina
  * Justin Chin
  * James Mabe
  * Alex Lebrov

### Introduction: ###

We were tasked with developing a program that can traverse a project
tree, and create a new repository. This repository will hold all
versions (artifacts) of every file in the project, so that we can
manage changes throughout the lifetime of the project.

Since C++ did not add support for filesystems until C++ 17, we have
opted to use the more readily avaiable Boost library to handle all
directory walking. Our implementation walks the entirety of a given
project tree, and copies the existing directory structure. Whenever a
"leaf" node, or normal file is found, we create a new folder in the
repository, sharing the file's name. We then loop over every character
in the file, creating a weighted checksum that we then use as the
artifactID for that specific file.

### Dependencies: ###

* Boost library
    * Get with ***sudo apt-get install libboost-all-dev***
* C++ 11
    * Compile with ***-std=c++11*** asmentioned in usage


### Usage: ###
    - Run versionControl.cpp with ***g++ -std=c++11 versionControl.cpp -lboost_filesystem -lboost_system***
    - Run output with ***./a.out***

### Features: ###

* Given an existing project directory, creates a repository by copying
  the existing file directory structure

* Creates a new directory for every non-directory file in the original
  * project tree Stores artifacts in these new folders, named by the weighted checksum of the file's contents, as well as its size in bytes
    * e.g "6648-L11.txt"