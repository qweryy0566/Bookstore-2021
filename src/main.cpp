#include "commands.h"

BookStore bookstore;

int main() {
  std::ios::sync_with_stdio(0);
  // std::cin.tie(0), std::cout.tie(0);
  std::string input;
  bookstore.Init();
  while (std::getline(cin, input))
    try {
      if (bookstore.Interprete(input)) break;
    } catch (Exception) {
      cout << "Invalid\n";
    }
  return 0;
}