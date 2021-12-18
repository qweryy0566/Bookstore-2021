#ifndef BOOKSTORE_COMMANDS_H_
#define BOOKSTORE_COMMANDS_H_

#include <iostream>
#include <string>
#include <vector>

#include "exception.hpp"
#include "user.h"
using std::string;
using std::vector;

class BookStore {
 private:
  UserManager user_manager;

 public:
  BookStore();

  void Interprete(string &);
};

#endif  // BOOKSTORE_COMMANDS_H_