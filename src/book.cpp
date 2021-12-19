#include "book.h"

bool IsBookISBN(const string &s) {
  if (s.length() > kBookISBN) return 0;
  for (auto c : s)
    if (!isprint(c)) return 0;
  return 1;
}
bool IsBookName(const string &s) {
  if (s.length() > kBookName) return 0;
  for (auto c : s)
    if (!isprint(c) || c == '\"') return 0;
  return 1;
}
bool IsBookCount(const string &s) {
  if (s.length() > 10) return 0;
  for (auto c : s)
    if (!isdigit(c)) return 0;
  return 1;
}
bool IsBookPrice(const string &s) {
  if (s.length() > 13) return 0;
  int cnt = 0;
  for (auto c : s)
    if (!isdigit(c)) {
      if (c != '.') return 0;
      ++cnt;
    }
  return cnt < 2;
}

// .......... class Book ..........

Book::Book() = default;
Book::Book(const string &isbn_, const int count_, const double price_,
           const string &keywords_, const string &name_, const string &author_)
    : Book() {
  // 进来之前必须先确定 keywords 的合法性。
  if (!IsBookISBN(isbn_) || !IsBookName(name_) || !IsBookName(author_))
    throw Exception();
  count = count_, price = price_;
  strcpy(isbn, isbn_.c_str());
  strcpy(keywords, keywords_.c_str());
  strcpy(name, name_.c_str());
  strcpy(author, author_.c_str());
}

// .......... class BookManager ..........

void BookManager::Init(const string &file) {
  books.Init(file + ".bin");
  isbn_list.Init(file + "_isbn_list");
  name_list.Init(file + "_name_list");
  author_list.Init(file + "_author_list");
  keyword_list.Init(file + "_keyword_list");
}

int BookManager::Select(const string &isbn) {
  Book tmp(isbn);
  int index = isbn_list.Find(Node(isbn));
  if (!index) {
    index = books.Write(tmp);
    isbn_list.Add(Node(isbn, 0, index));
  }
  return index;
}