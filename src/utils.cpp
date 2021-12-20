#include "utils.h"

void SpiltString(const string &str, vector<string> &argv, char c) {
  argv.clear();
  if (str.empty()) return;
  string::size_type last_pos = 0, pos;
  do {
    pos = str.find(c, last_pos);
    if (pos > last_pos || c != ' ')
      argv.push_back(str.substr(last_pos, pos - last_pos));
    last_pos = pos + 1;
  } while (last_pos < str.length() && pos != string::npos);
  if (c != ' ' && str.back() == c)
    argv.push_back(string());
}

bool SpiltString(const string &str, pair<string, string> &p, char c) {
  string::size_type pos = str.find(c);
  if (pos == string::npos) return 0;
  p = std::make_pair(str.substr(0, pos), str.substr(pos + 1));
  return 1;
}