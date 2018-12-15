#pragma once

#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

std::string timestamp();
void LogToManifest(std::string log, std::ofstream& manifest);
std::vector<std::string> GetLinearHistory(std::string manifest);
std::string GetParent(std::string manifest);
