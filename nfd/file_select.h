#pragma once

#include "raylib.h"
#include <nfd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

std::string FileSelectDialog();
std::vector<int> ReadNumbersFromFile(const std::string& filePath);