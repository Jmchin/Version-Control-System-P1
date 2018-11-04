#pragma once

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

// TODO: Store the arguments used to operate the command (e.g Check-In
// with project2/ as the source directory and repo2/ as the target
// destination

struct Manifest {
  std::string command;
  std::string timestamp;
  std::string user;
};


std::string timestamp();
std::string ManifestToString(const Manifest& m);
void LogToManifest(std::string log, std::ofstream& manifest);
void WriteManifestToPath(const Manifest& m, std::string path);
