#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_set>


void WriteCreationHierarchy(const std::string filePath, const std::string manifestName);

std::string FindMinimalCommonParent(std::string parentListOne, std::string parentListTwo);
