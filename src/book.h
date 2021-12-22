#ifndef BOOKSTORE_BOOK_H_
#define BOOKSTORE_BOOK_H_

#include "ull.h"
#include "user.h"
#include "utils.h"

const int kBookIsbn = 20, kBookKeyword = 60, kBookName = 60;
// BookName 和 BookAuthor 共用判断函数和最大长度。

bool IsBookIsbn(const string &);
bool IsBookName(const string &);  // author 规则相同
bool IsBookAuthor(const string &);
bool IsBookKeyword(const string &);  // 单个 keyword
bool IsBookCount(const string &);
bool IsBookPrice(const string &);

class Book {
  friend ostream &operator<<(ostream &, const Book &);

 private:
  char isbn[kBookIsbn + 1] = "", keywords[kBookKeyword + 1] = "";
  char name[kBookName + 1] = "", author[kBookName + 1] = "";
  long long count = 0;  // 可能有多个 2^31-1 相加。
  double price = 0;

 public:
  Book();
  explicit Book(const string &isbn_, const long long count_ = 0,
                const double price_ = 0, const string &keywords_ = "",
                const string &name_ = "", const string &author_ = "");
  const string Isbn() const;
  const string Name() const;
  const string Author() const;
  const string Keywords() const;
  const double &Price() const;
  const long long &Count() const;
  void ChangeIsbn(const string &);
  void ChangeName(const string &);
  void ChangeAuthor(const string &);
  void ChangeKeywords(const string &);
  void ChangePrice(const double &);
  void AddCount(const int &);
};

class BookManager {
 private:
  MemoryRiver<Book> books;
  BlockList isbn_list, name_list, author_list, keyword_list;

  void PrintIndex(const vector<int> &);

 public:
  void Init(const string &);
  int Select(const string &);
  // 查找是否已经存在该 isbn.
  int Find(const string &);
  void ModifyIsbn(const int &, const string &);
  void ModifyName(const int &, const string &);
  void ModifyAuthor(const int &, const string &);
  void ModifyKeywords(const int &, const string &, const vector<string> &);
  void ModifyPrice(const int &, const string &);
  double BuyBook(const int &, const int &);
  void AddBook(const int &, const int &);
  void ShowIsbn(const string &str = "");
  void ShowName(const string &);
  void ShowAuthor(const string &);
  void ShowKeyword(const string &);
  const Book GetBook(const int &);
};

#endif  // BOOKSTORE_BOOK_H_