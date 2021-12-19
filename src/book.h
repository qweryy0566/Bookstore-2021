#ifndef BOOKSTORE_BOOK_H_
#define BOOKSTORE_BOOK_H_

#include "ull.h"
#include "user.h"
#include "utils.h"
using std::string;
using std::unordered_map;
using std::vector;

const int kBookISBN = 20, kBookKeyword = 60, kBookName = 60;
// BookName 和 BookAuthor 共用判断函数和最大长度。

bool IsBookISBN(const string &);
bool IsBookName(const string &);  // author 和单个 keyword 规则相同
bool IsBookCount(const string &);
bool IsBookPrice(const string &);

class Book {
 private:
  char isbn[kBookISBN + 1] = "", keywords[kBookKeyword + 1] = "";
  char name[kBookName + 1] = "", author[kBookName + 1] = "";
  int count = 0;
  double price = 0;

 public:
  Book();
  explicit Book(const string &isbn_, const int count_ = 0,
                const double price_ = 0, const string &keywords_ = "",
                const string &name_ = "", const string &author_ = "");
};

class BookManager {
 private:
  MemoryRiver<Book> books;
  BlockList isbn_list, name_list, author_list, keyword_list;

 public:
  void Init(const string &);
  int Select(const string &);
};

#endif  // BOOKSTORE_BOOK_H_