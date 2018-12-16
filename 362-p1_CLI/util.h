#pragma once

#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace fs = boost::filesystem;

#define WEIGHT_ONE 1
#define WEIGHT_TWO 3
#define WEIGHT_THREE 7
#define WEIGHT_FOUR 11
#define WEIGHT_FIVE 17
/* #define INT_MAX 2147483647 */

class CreationObject {

public:
  std::string source;
  std::string destination;
  bool isFolder;

  CreationObject(std::string s, std::string d, bool isF) {
    source = s;
    destination = d;
    isFolder = isF;
  }
  bool operator==(const CreationObject &x) const {
    if(source != x.source || destination != x.destination || isFolder != x.isFolder) {
      return false;
    }
    else {
      return true;
    }
  }
  bool operator<(const CreationObject &x) const {
    return source < x.source;
  }
};

std::vector<CreationObject> GetCreationOrder(std::string manifestFile);
std::vector<std::string> GetPaths(std::string manifestFile);
std::vector<std::string> Split(std::string str, std::string sep);
std::string CheckSum(std::string fileName);
std::string FormatFileName(long long sum, int count, std::string extension);
std::string GetArguments(int argc, char* argv[]);
std::string getAliasIfExists(std::string name, std::string source);
int get_current_version(std::string repo_root);
void update_version(std::string repo_root);
std::string getCommonAncestor(std::vector<std::string> source, std::vector<std::string> target);
