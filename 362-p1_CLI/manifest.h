#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> LabelList;
typedef std::vector<std::vector<std::string> > FileList;  // [[filename1, artifact1, artifact2]]

// TODO: Store the arguments used to operate the command (e.g Check-In
// with project2/ as the source directory and repo2/ as the target
// destination

struct Manifest {
  std::string command;
  std::string timestamp;
  std::string user;
  LabelList labels;     // TODO: Does the manifest actually need to store its label?
  FileList files;
};


std::string timestamp();
Manifest LabelManifest(const Manifest& m, std::string label);
std::string ManifestToString(const Manifest& m);
std::string mLabelsToString(const Manifest& m);
std::string mFilesToString(const Manifest& m);
void WriteManifestToPath(const Manifest& m, std::string path);
