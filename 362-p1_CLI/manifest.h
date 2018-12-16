#pragma once

#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_set>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <unordered_map>

namespace fs = boost::filesystem;

std::string timestamp();
void LogToManifest(std::string log, std::ofstream& manifest);
std::vector<std::string> GetLinearHistory(std::string manifest, std::string repo);
std::string GetParent(std::string manifest);
