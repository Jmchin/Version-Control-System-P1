#pragma once

#include <string>
#include <fstream>
#include <vector>

typedef std::vector<std::string> FileList;  // [[filename1, artifact1, artifact2]]

// TODO: Store the arguments used to operate the command (e.g Check-In
// with project2/ as the source directory and repo2/ as the target
// destination

struct Manifest {
  std::string command;
  std::string timestamp;
  std::string user;
  FileList files;
};


std::string timestamp();
// Manifest LabelManifest(const Manifest& m, std::string label);
std::string ManifestToString(const Manifest& m);
void LogToManifest(std::string log, std::ofstream& manifest);
// std::string mFilesToString(const Manifest& m);
void WriteManifestToPath(const Manifest& m, std::string path);
