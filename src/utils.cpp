#include "utils.h"

void SpiltString(const string &str, vector<string> &argv, char c) {
  if (str.empty()) return;
  string::size_type last_pos = 0, pos;
  do {
    pos = str.find(c, last_pos);
    if (pos > last_pos)
      argv.push_back(str.substr(last_pos, pos - last_pos));
    last_pos = pos + 1;
  } while (last_pos < str.length() && pos != string::npos);
}
