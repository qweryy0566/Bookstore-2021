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
    VisitSelect(argv);
  } else if (argv[0] == "modify") {
    VisitModify(argv);
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

// modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" |
// -keyword="[Keyword]" | -price=[Price])+
void BookStore::VisitModify(vector<string> &argv) {
  int index = user_manager.GetBookOffset();
  if (argv.size() == 1 || !index) throw Exception();
  // 第一遍：检查是否有重复或非法的附加参数
  unordered_map<string, bool> vis;
  pair<string, string> param[argv.size()];
  vector<string> keywords;  // 存储分隔开来的 keyword.
  for (int i = 1; i < argv.size(); ++i) {
    int pos = argv[i].find('=');
    param[i] = std::make_pair(argv[i].substr(0, pos), argv[i].substr(pos + 1));
    if (vis.find(param[i].first) != vis.end()) throw Exception();
    string &str = param[i].second;
    if (param[i].first == "-ISBN") {  // 注意判断没有重复。
      if (!IsBookIsbn(str) || book_manager.Find(str)) throw Exception();
    } else if (param[i].first == "-name") {
      if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
        throw Exception();
      str.substr(1), str.pop_back();
      if (!IsBookName(str)) throw Exception();
    } else if (param[i].first == "-author") {
      if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
        throw Exception();
      str.substr(1), str.pop_back();
      if (!IsBookAuthor(str)) throw Exception();
    } else if (param[i].first == "-keyword") {
      if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
        throw Exception();
      str.substr(1), str.pop_back();
      SpiltString(str, keywords, '|');
      for (auto it : keywords)
        if (!IsBookKeyword(it)) throw Exception();
    } else if (param[i].first == "-price") {
      if (!IsBookPrice(str)) throw Exception();
    } else {
      throw Exception();
    }
    vis[param[i].first] = 1;
  }
  // 第二遍：确保指令合法的情况下依次修改。
  for (int i = 1; i < argv.size(); ++i)
    if (param[i].first == "-ISBN") {
      book_manager.ModifyIsbn(index, param[i].second);
    } else if (param[i].first == "-name") {
      book_manager.ModifyName(index, param[i].second);
    } else if (param[i].first == "-author") {
      book_manager.ModifyAuthor(index, param[i].second);
    } else if (param[i].first == "-keyword") {
      book_manager.ModifyKeywords(index, param[i].second, keywords);
    } else if (param[i].first == "-price") {
      book_manager.ModifyPrice(index, param[i].second);
    }
}