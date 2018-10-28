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
Manifest LabelManifest(const Manifest& m, std::string label);
void PrintLabels(const Manifest& m);
void PrintFiles(const Manifest& m);
void PrintManifest(const Manifest& m);
void WriteManifestToPath(const Manifest& m, std::string path);
