#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> LabelList;
typedef std::vector<std::vector<std::string> > FileList;  // [[filename1, artifact1, artifact2]]

struct Manifest {
  std::string command;
  std::string timestamp;
  std::string user;
  LabelList labels;
  FileList files;
};


std::string timestamp();
Manifest LabelManifest(Manifest m, std::string label);
void PrintLabels(Manifest m);
void PrintManifest(Manifest m);
void WriteManifestToDisk(Manifest m);
