#ifndef BOOKSTORE_COMMANDS_H_
#define BOOKSTORE_COMMANDS_H_

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "exception.hpp"
#include "user.h"
using std::string;
using std::vector;

void Arguments(const string &, vector<string> &);

class BookStore {
 private:
  UserManager user_manager;

 public:
  void Init();
  void Interprete(string &);
};

#endif  // BOOKSTORE_COMMANDS_H_