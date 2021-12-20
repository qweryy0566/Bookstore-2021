#ifndef BOOKSTORE_UTILS_H_
#define BOOKSTORE_UTILS_H_

#include <cctype>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "exception.hpp"
using std::cin;
using std::cout;
using std::pair;
using std::ostream;
using std::unordered_map;
using std::string;
using std::vector;

void SpiltString(const string &, vector<string> &, char c = ' ');
bool SpiltString(const string &, pair<string, string> &, char c = '=');

#endif  // BOOKSTORE_UTILS_H_