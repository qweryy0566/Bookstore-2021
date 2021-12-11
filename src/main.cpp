#include <iostream>

#include "storge.hpp"
#include "ull.h"
using std::cin;
using std::cout;

int main() {
  std::ios::sync_with_stdio(0);
  BlockList list("test");
  int Q, val;
  string opt, key;
  for (cin >> Q; Q--;) {
    cin >> opt >> key;
    if (opt == "insert") {
      cin >> val;
      list.Add(Node(key, val));
    } else if (opt == "delete") {
      cin >> val;
      list.Del(Node(key, val));
    } else if (opt == "find") {
      vector<int> ans;
      if (!list.Query(key, ans))
        cout << "null";
      else
        for (auto it : ans) cout << it << ' ';
      cout.put('\n');
    }
  }
  return 0;
}