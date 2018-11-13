#pragma once

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace fs = boost::filesystem;

#define WEIGHT_ONE 1
#define WEIGHT_TWO 3
#define WEIGHT_THREE 7
#define WEIGHT_FOUR 11
#define WEIGHT_FIVE 17
/* #define INT_MAX 2147483647 */

std::string CheckSum(std::string fileName);
void DeepCopyDir(fs::path src, fs::path des);
void FolderifyLeaf(std::string filePath);
std::string FormatFileName(long long sum, int count, std::string extension);
std::string GetArguments(int argc, char* argv[]);
void update_version(std::string repo_root);
