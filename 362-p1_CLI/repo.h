#pragma once

#include <algorithm>
#include "util.h"

void Create(std::string source, std::string destination, std::string commands);
void CheckIn(std::string source, std::string destination, std::string commands);
void CheckOut(std::string source, std::string destination, std::string manifest, std::string commands);
void Merge(std::string source, std::string manifest, std::string target);
void RepoifyDirectory(fs::path src_root, fs::path des_root, std::ofstream& manifest);
void InitLabels(std::string destination);
void LabelManifest(std::string repo, std::string manifest, std::string label);
