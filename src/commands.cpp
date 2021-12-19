#include "commands.h"

void Arguments(const string &str, vector<string> &argv) {
  if (str.empty()) return;
  string::size_type last_pos = 0, pos;
  do {
    pos = str.find(' ', last_pos);
    if (pos > last_pos)
      argv.push_back(str.substr(last_pos, pos - last_pos));
    last_pos = pos + 1;
  } while (last_pos < str.length() && pos != string::npos);
}

// .......... class BookStore ..........

void BookStore::Init() {
  std::filesystem::create_directory("data");
  user_manager.Init("data/users");
}

void BookStore::Interprete(string &command) {
  vector<string> argv;
  Arguments(command, argv);
  if (argv.empty()) return;
  if (argv[0] == "su") {
    
  } else if (argv[0] == "logout") {

  } else if (argv[0] == "register") {

  } else if (argv[0] == "passwd") {

  } else if (argv[0] == "useradd") {

  } else if (argv[0] == "delete") {

  } else if (argv[0] == "show") {  // 包含了图书的 show 和日志的 show finance.

  } else if (argv[0] == "buy") {

  } else if (argv[0] == "select") {

  } else if (argv[0] == "modify") {

  } else if (argv[0] == "import") {

  } else if (argv[0] == "report") {

  } else if (argv[0] == "log") {

  } else {
    throw Exception();
  }
}