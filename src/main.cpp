#include <iostream>
#include <string>

#include "commands.h"
#include "exception.hpp"
using std::cin;
using std::cout;

BookStore bookstore;

int main() {
  std::ios::sync_with_stdio(0);
  std::string input;
  while (std::getline(cin, input), input != "exit" && input != "quit")
    try {
      bookstore.Interprete(input);
    } catch (Exception) {
      cout << "Invalid\n";
    }
  return 0;
}