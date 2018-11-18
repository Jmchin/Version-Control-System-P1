# Version-Control-System Project 1 #

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
    - Run versionControl.cpp with ***g++ -std=c++11 -o cli_vcs -lboost_filesystem -lboost_system manifest.cpp versionControl.cpp util.cpp repo.cpp creationOrder.cpp mapfile.cpp***
    - Run output with ***./a.out***
    

### Features: ###

* Creates a repository by copying the existing file directory structure given an existing project directory
  * Creates a new directory for every non-directory file in the original
  * project tree stores artifacts in these new folders, named by the weighted checksum of the file's contents, as well as its size in bytes, e.g "6648-L11.txt"
    * Usage: cli_vcs create <project> <repository>
    
* Allows the creation of labels to a manifest file to make it easier to track and use when issuing commands
  * uniquely IDs a manifest under .labels, the program will then check said file when a user uses another command regarding manifests
    * Usage: cli_vcs label <repo> <manifest> <label>
    
* Check-out a specific version of the project tree by providing a specific manifest
  * Reads through the manifest and decides which specific versions of files should be applied
    *Usage: cli_vcs checkin <project> <repository>
    
* Check-in a new version of the project by providing a filepath to a project
  * Writes a new manifest related to the new version of the project
    * Usage: cli_vcs checkout <repo> <manifest> <directory>
    
### Member Contributions: ###

* Kevin: Project Design, Create(), Boost implementation.
* Justin: FolderifLeaf(), Readme, testing.
* James: CheckSum(), testing.
* Alex: DeepCopyDir(), FormatFileName()
