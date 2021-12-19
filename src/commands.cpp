#include "commands.h"

// .......... class BookStore ..........

void BookStore::Init() {
  if (!std::filesystem::exists("data"))
    std::filesystem::create_directory("data");
  user_manager.Init("data/users");
  book_manager.Init("data/books");
}

void BookStore::Interprete(string &command) {
  vector<string> argv;
  SpiltString(command, argv);
  if (argv.empty()) return;
  if (argv[0] == "su") {
    VisitSu(argv);
  } else if (argv[0] == "logout") {
    VisitLogout(argv);
  } else if (argv[0] == "register") {
    VisitRegister(argv);
  } else if (argv[0] == "passwd") {
    VisitPasswd(argv);
  } else if (argv[0] == "useradd") {
    VisitUseradd(argv);
  } else if (argv[0] == "delete") {
    VisitDelete(argv);
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

// su [User-ID] ([Password])?
void BookStore::VisitSu(vector<string> &argv) {
  switch (argv.size()) {
    case 2: user_manager.Login(argv[1], ""); break;
    case 3: user_manager.Login(argv[1], argv[2]); break;
    default: throw Exception();
  }
}

// logout
void BookStore::VisitLogout(vector<string> &argv) {
  if (argv.size() != 1) throw Exception();
  user_manager.Logout();
}

// register [User-ID] [Password] [User-Name]
void BookStore::VisitRegister(vector<string> &argv) {
  if (argv.size() != 4) throw Exception();
  user_manager.Register(argv[1], argv[2], argv[3]);
}

// passwd [User-ID] ([Old-Password])? [New-Password]
void BookStore::VisitPasswd(vector<string> &argv) {
  switch (argv.size()) {
    case 3: user_manager.Passwd(argv[1], "", argv[2]); break;
    case 4: user_manager.Passwd(argv[1], argv[2], argv[3]); break;
    default: throw Exception();
  }
}

// useradd [User-ID] [Password] [Priority] [User-Name]
void BookStore::VisitUseradd(vector<string> &argv) {
  if (argv.size() != 5) throw Exception();
  user_manager.AddUser(argv[1], argv[2], argv[3], argv[4]);
}

// delete [User-ID]
void BookStore::VisitDelete(vector<string> &argv) {
  if (argv.size() != 2) throw Exception();
  user_manager.DeleteUser(argv[1]);
}

// select [ISBN]
void BookStore::VisitSelect(vector<string> &argv) {
  if (argv.size() != 2) throw Exception();
  SelectBook(argv[1]);
}
void BookStore::SelectBook(const string &isbn) {
  user_manager.SelectBook(book_manager.Select(isbn));
}

