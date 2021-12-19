#include "commands.h"

BookStore bookstore;

int main() {
  std::ios::sync_with_stdio(0);
  std::string input;
  bookstore.Init();
  while (std::getline(cin, input),
         !cin.eof() && input != "exit" && input != "quit")
    try {
      bookstore.Interprete(input);
    } catch (Exception) {
      cout << "Invalid\n";
    }
  return 0;
}