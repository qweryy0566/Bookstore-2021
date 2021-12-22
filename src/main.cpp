#include "commands.h"

BookStore bookstore;

int main() {
  std::ios::sync_with_stdio(0);
  // std::cin.tie(0), std::cout.tie(0);
  std::string input;
  bookstore.Init();
  while (std::getline(cin, input))
    try {
      // 单个指令最大长度为 1024 字节。
      if (input.length() > 1024) throw Exception();
      for (auto it : input)
        if (!isascii(it)) throw Exception();
      if (bookstore.Interprete(input)) break;
    } catch (Exception) {
      cout << "Invalid\n";
    }
  return 0;
}