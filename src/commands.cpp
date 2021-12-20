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
    argv.size() > 1 && argv[1] == "finance" ? VisitShowFinance(argv)
                                            : VisitShow(argv);
  } else if (argv[0] == "buy") {
    VisitBuy(argv);
  } else if (argv[0] == "select") {
    VisitSelect(argv);
  } else if (argv[0] == "modify") {
    VisitModify(argv);
  } else if (argv[0] == "import") {
    VisitImport(argv);
  } else if (argv[0] == "report") {

  } else if (argv[0] == "log") {

  } else {
    throw Exception();
  }
}

// {0} su [User-ID] ([Password])?
void BookStore::VisitSu(vector<string> &argv) {
  switch (argv.size()) {
    case 2: user_manager.Login(argv[1], ""); break;
    case 3: user_manager.Login(argv[1], argv[2]); break;
    default: throw Exception();
  }
}

// {1} logout
void BookStore::VisitLogout(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 1) throw Exception();
  if (argv.size() != 1) throw Exception();
  user_manager.Logout();
}

// {0} register [User-ID] [Password] [User-Name]
void BookStore::VisitRegister(vector<string> &argv) {
  if (argv.size() != 4) throw Exception();
  user_manager.Register(argv[1], argv[2], argv[3]);
}

// {1} passwd [User-ID] ([Old-Password])? [New-Password]
void BookStore::VisitPasswd(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 1) throw Exception();
  switch (argv.size()) {
    case 3: user_manager.Passwd(argv[1], "", argv[2]); break;
    case 4: user_manager.Passwd(argv[1], argv[2], argv[3]); break;
    default: throw Exception();
  }
}

// {3} useradd [User-ID] [Password] [Priority] [User-Name]
void BookStore::VisitUseradd(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  if (argv.size() != 5) throw Exception();
  user_manager.AddUser(argv[1], argv[2], argv[3], argv[4]);
}

// {7} delete [User-ID]
void BookStore::VisitDelete(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 7) throw Exception();
  if (argv.size() != 2) throw Exception();
  user_manager.DeleteUser(argv[1]);
}

// {3} show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" |
// -keyword="[Keyword]")?
void BookStore::VisitShow(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  switch (argv.size()) {
    case 1:
      book_manager.ShowIsbn();
      break;
    case 2: {
      pair<string, string> param;
      if (!SpiltString(argv[1], param)) throw Exception();
      if (param.first == "=ISBN") {
        if (!IsBookIsbn(param.second)) throw Exception();
        book_manager.ShowIsbn(param.second);
      } else if (param.first == "=name") {
        if (!IsBookName(param.second)) throw Exception();
        book_manager.ShowName(param.second);
      } else if (param.first == "=author") {
        if (!IsBookAuthor(param.second)) throw Exception();
        book_manager.ShowAuthor(param.second);
      } else if (param.first == "=keyword") {
        if (!IsBookKeyword(param.second)) throw Exception();
        book_manager.ShowKeyword(param.second);
      } else {
        throw Exception();
      }
      break;
    }
    default:
      throw Exception();
  }
  cout.put('\n');
}

// {1} buy [ISBN] [Quantity]
void BookStore::VisitBuy(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 1) throw Exception();
  if (argv.size() != 3) throw Exception();
  // 判断参数的合法性。
  if (!IsBookIsbn(argv[1]) || !IsBookCount(argv[2])) throw Exception();
  int index = book_manager.Find(argv[1]);
  if (!index) throw Exception();
  cout << std::fixed << std::setprecision(2)
       << book_manager.BuyBook(index, std::stoi(argv[2])) << '\n';
  
}

// {3} select [ISBN]
void BookStore::VisitSelect(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  if (argv.size() != 2) throw Exception();
  SelectBook(argv[1]);
}
void BookStore::SelectBook(const string &isbn) {
  user_manager.SelectBook(book_manager.Select(isbn));
}

// {3} modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" |
// -keyword="[Keyword]" | -price=[Price])+
void BookStore::VisitModify(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  int index = user_manager.GetBookOffset();
  if (argv.size() == 1 || !index) throw Exception();
  // 第一遍：检查是否有重复或非法的附加参数
  unordered_map<string, bool> vis;
  pair<string, string> param[argv.size()];
  vector<string> keywords;  // 存储分隔开来的 keyword.
  for (int i = 1; i < argv.size(); ++i) {
    if (!SpiltString(argv[i], param[i])) throw Exception();
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

// {3} import [Quantity] [Total-Cost]
void BookStore::VisitImport(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  if (argv.size() != 3) throw Exception();
  if (!IsBookCount(argv[1]) || !IsBookPrice(argv[2])) throw Exception();
  int index = user_manager.GetBookOffset();
  if (!index) throw Exception();
  book_manager.AddBook(index, std::stoi(argv[1]));

}

// {7} show finance ([Time])?
void BookStore::VisitShowFinance(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 7) throw Exception();

}