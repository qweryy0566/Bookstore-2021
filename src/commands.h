#ifndef BOOKSTORE_COMMANDS_H_
#define BOOKSTORE_COMMANDS_H_

#include "book.h"
#include "log.h"
#include "user.h"
#include "utils.h"

const string kIsbnStr = "-ISBN", kNameStr = "-name", kAuthorStr = "-author",
             kKeywordStr = "-keyword", kPriceStr = "-price";

class BookStore {
 private:
  UserManager user_manager;
  BookManager book_manager;
  LogManager log_manager;

  void SelectBook(const string &);

 public:
  void Init();
  void Interprete(string &);

  void VisitSu(vector<string> &);
  void VisitLogout(vector<string> &);
  void VisitRegister(vector<string> &);
  void VisitPasswd(vector<string> &);
  void VisitUseradd(vector<string> &);
  void VisitDelete(vector<string> &);
  void VisitShow(vector<string> &);
  void VisitBuy(vector<string> &);
  void VisitSelect(vector<string> &);
  void VisitModify(vector<string> &);
  void VisitImport(vector<string> &);

  void VisitShowFinance(vector<string> &);
};

#endif  // BOOKSTORE_COMMANDS_H_