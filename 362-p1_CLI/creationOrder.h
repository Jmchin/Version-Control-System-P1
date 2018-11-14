#pragma once

#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

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
