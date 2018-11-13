#pragma once

#include <sstream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>

std::string timestamp();
void LogToManifest(std::string log, std::ofstream& manifest);
