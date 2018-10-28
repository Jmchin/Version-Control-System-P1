#pragma once

#include <string>
#include <vector>

typedef std::vector<std::string> LabelList;
typedef std::vector<std::vector<std::string> > FileList;  // [[filename1, artifact1, artifact2]]

struct Manifest {
  std::string command;
  std::string timestamp;
  std::string user;
  LabelList* labels = new LabelList;
  FileList* files = new FileList;
};


std::string timestamp();
Manifest LabelManifest(Manifest m, std::string label);
void PrintLabels(Manifest& m);
void PrintFiles(Manifest& m);
void PrintManifest(Manifest& m);
void WriteManifestToPath(Manifest& m, std::string path);
