#pragma once

#include "util.h"

void Create(std::string source, std::string destination, std::string commands);
void CheckIn(std::string source, std::string destination, std::string commands);
void CheckOut(std::string manifest, std::string destination);
