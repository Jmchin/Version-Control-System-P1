# Version-Control-System Project 3 #

### General Information ###
* CPSC 362 - #12846/12847
* Project 1
* Team CLI
  * Kevin Molina
  * Justin Chin
  * James Mabe
  * Alex Lavrov

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
    - Run versionControl.cpp with ***g++ -std=c++11 -lboost_filesystem
    -lboost_system versionControl.cpp manifest.cpp repo.cpp util.cpp***
    - Run output with ***./a.out***

#### Create: ####
     - ./a.out create <project directory> <repo directory>
     - creates a repository from the project directory

#### Check-in: ####
     - ./a.out checkin <project directory> <repo directory>
     - checks in a project tree into an existing repository

#### Check-out: ####
     - ./a.out checkout <repo directory> <manifest/label> <target
     directory>
     - checks out a specific version of a repository, specified by the
     manifest into the target directory

#### Label: ####
     - ./a.out label <repo directory> <manifest> <label>
     - labels a manifest with a string

#### Merge: ####
     - ./a.out merge <repo directory> <manifest/label> <target directory>
     - merges a specific version of a repository into an existing
     target directory
