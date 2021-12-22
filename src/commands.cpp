#include "commands.h"

// .......... class BookStore ..........

void BookStore::Init() {
  if (!std::filesystem::exists("data"))
    std::filesystem::create_directory("data");
  user_manager.Init("data/users");
  book_manager.Init("data/books");
  log_manager.Init("data/");
}

// 返回是否需要退出系统。
bool BookStore::Interprete(string &command) {
  vector<string> argv;
  SpiltString(command, argv);
  if (argv.empty()) return 0;
  if (argv[0] == "quit" || argv[0] == "exit") {
    if (argv.size() != 1) throw Exception();
    return 1;
  } else if (argv[0] == "su") {
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
    // TODO : {3} report myself
    // TODO : {7} report finance
    // TODO : {7} report employee
  } else if (argv[0] == "log") {
    // TODO : {7} log
  } else {
    throw Exception();
  }
  return 0;  // 正常返回。
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
  // 禁止添加 0 权账户。
  user_manager.AddUser(argv[1], argv[2], argv[3], argv[4]);
}

// {7} delete [User-ID]
void BookStore::VisitDelete(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 7) throw Exception();
  if (argv.size() != 2) throw Exception();
  user_manager.DeleteUser(argv[1]);
}

// {1} show (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" |
// -keyword="[Keyword]")?
void BookStore::VisitShow(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 1) throw Exception();
  switch (argv.size()) {
    case 1:
      book_manager.ShowIsbn();
      break;
    case 2: {
      pair<string, string> param;
      string &str = param.second;
      if (!SpiltString(argv[1], param)) throw Exception();
      if (param.first == kIsbnStr) {
        if (!IsBookIsbn(str)) throw Exception();
        book_manager.ShowIsbn(str);
      } else if (param.first == kNameStr) {
        if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
          throw Exception();
        str = str.substr(1), str.pop_back();
        if (!IsBookName(str)) throw Exception();
        book_manager.ShowName(str);
      } else if (param.first == kAuthorStr) {
        if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
          throw Exception();
        str = str.substr(1), str.pop_back();
        if (!IsBookAuthor(str)) throw Exception();
        book_manager.ShowAuthor(str);
      } else if (param.first == kKeywordStr) {
        if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
          throw Exception();
        str = str.substr(1), str.pop_back();
        if (!IsBookKeyword(str)) throw Exception();
        book_manager.ShowKeyword(str);
      } else {
        throw Exception();
      }
      break;
    }
    default:
      throw Exception();
  }
}

// {1} buy [ISBN] [Quantity]
void BookStore::VisitBuy(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 1) throw Exception();
  if (argv.size() != 3) throw Exception();
  // 判断参数的合法性。
  if (!IsBookIsbn(argv[1]) || !IsBookCount(argv[2])) throw Exception();
  int index = book_manager.Find(argv[1]);
  if (!index) throw Exception();
  double income = book_manager.BuyBook(index, std::stoi(argv[2]));
  cout << std::fixed << std::setprecision(2) << income << '\n';
  // buy 对书店来说是收入。
  log_manager.AddSpend(+income);
}

// {3} select [ISBN]
void BookStore::VisitSelect(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  if (argv.size() != 2) throw Exception();
  SelectBook(argv[1]);
}
void BookStore::SelectBook(const string &isbn) {
  if (!IsBookIsbn(isbn)) throw Exception();  // 记得判断是否合法。
  user_manager.SelectBook(book_manager.Select(isbn));
}

// {3} modify (-ISBN=[ISBN] | -name="[Book-Name]" | -author="[Author]" |
// -keyword="[Keyword]" | -price=[Price])+
void BookStore::VisitModify(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 3) throw Exception();
  int index = user_manager.GetBookOffset();  // 未选中图书时 offset 为 0.
  if (argv.size() == 1 || !index) throw Exception();
  // 第一遍：检查是否有重复或非法的附加参数
  unordered_map<string, bool> vis;
  pair<string, string> param[argv.size()];
  vector<string> keywords;  // 存储分隔开来的 keyword.
  for (int i = 1; i < argv.size(); ++i) {
    if (!SpiltString(argv[i], param[i])) throw Exception();
    if (vis.find(param[i].first) != vis.end()) throw Exception();
    string &str = param[i].second;
    if (param[i].first == kIsbnStr) {  // 注意判断没有重复。
      if (!IsBookIsbn(str)) throw Exception();
      // 注意判断将 ISBN 改为自己的情况。
      Book tmp = book_manager.GetBook(user_manager.GetBookOffset());
      if (tmp.Isbn() != str && book_manager.Find(str)) throw Exception();
    } else if (param[i].first == kNameStr) {
      if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
        throw Exception();
      str = str.substr(1), str.pop_back();
      if (!IsBookName(str)) throw Exception();
    } else if (param[i].first == kAuthorStr) {
      if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
        throw Exception();
      str = str.substr(1), str.pop_back();
      if (!IsBookAuthor(str)) throw Exception();
    } else if (param[i].first == kKeywordStr) {
      if (str.length() <= 2 || str.front() != '\"' || str.back() != '\"')
        throw Exception();
      str = str.substr(1), str.pop_back();
      // 多个关键词的最大长度不能大于 60.
      if (str.length() > kBookKeyword) throw Exception();
      unordered_map<string, bool> vis_keyword;
      SpiltString(str, keywords, '|');
      for (auto it : keywords) {
        if (!IsBookKeyword(it) || vis_keyword.find(it) != vis_keyword.end())
          throw Exception();
        vis_keyword[it] = 1;
      }
    } else if (param[i].first == kPriceStr) {
      if (!IsBookPrice(str)) throw Exception();
    } else {
      throw Exception();
    }
    vis[param[i].first] = 1;
  }
  // 第二遍：确保指令合法的情况下依次修改。
  for (int i = 1; i < argv.size(); ++i)
    if (param[i].first == kIsbnStr) {
      book_manager.ModifyIsbn(index, param[i].second);
    } else if (param[i].first == kNameStr) {
      book_manager.ModifyName(index, param[i].second);
    } else if (param[i].first == kAuthorStr) {
      book_manager.ModifyAuthor(index, param[i].second);
    } else if (param[i].first == kKeywordStr) {
      book_manager.ModifyKeywords(index, param[i].second, keywords);
    } else if (param[i].first == kPriceStr) {
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
  // import 对书店来说是支出。
  log_manager.AddSpend(-std::stod(argv[2]));
}

// {7} show finance ([Time])?
void BookStore::VisitShowFinance(vector<string> &argv) {
  if (user_manager.GetPrivilege() < 7) throw Exception();
  switch (argv.size()) {
    case 2: log_manager.ShowFinance(); break;
    case 3: log_manager.ShowFinance(argv[2]); break;
    default: throw Exception();
  }
}