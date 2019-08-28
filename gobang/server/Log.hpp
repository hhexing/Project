#pragma once

#include <iostream>
#include <string>
#include <vector>

#define INFO 0
#define WARNING 1
#define FATAL 2

using namespace std;

vector<string> error_level = {
    "INFO",
    "WARNING",
    "FATAL",
};
void Log(int level, string message, string file, int line)
{
    cout << "[" << error_level[level] << "][" << message << "]:" << file << ":" << line << endl;
}

#define LOG(level, message) Log(level, message, __FILE__, __LINE__)
