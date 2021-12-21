#include "book.h"

bool IsBookIsbn(const string &s) {
  if (s.empty() || s.length() > kBookIsbn) return 0;
  for (auto c : s)
    if (!isprint(c)) return 0;
  return 1;
}
bool IsBookName(const string &s) {
  if (s.empty() || s.length() > kBookName) return 0;
  for (auto c : s)
    if (!isprint(c) || c == '\"') return 0;
  return 1;
}
bool IsBookAuthor(const string &s) {
  return IsBookName(s);
}
// 判断是否为单个关键词。
bool IsBookKeyword(const string &s) {
  if (s.empty() || s.length() > kBookKeyword) return 0;
  for (auto c : s)
    if (!isprint(c) || c == '\"' || c == '|') return 0;
  return 1;
}
bool IsBookCount(const string &s) {
  if (s.empty() || s.length() > 10) return 0;
  for (auto c : s)
    if (!isdigit(c)) return 0;
  return 1;
}
bool IsBookPrice(const string &s) {
  if (s.empty() || s.length() > 13) return 0;
  int cnt = 0;
  for (auto c : s)
    if (!isdigit(c)) {
      if (c != '.') return 0;
      ++cnt;
    }
  return cnt < 2;
}

// .......... class Book ..........

ostream &operator<<(ostream &lhs, const Book &rhs) {
  lhs << rhs.isbn << '\t' << rhs.name << '\t' << rhs.author << '\t'
      << rhs.keywords << '\t' << std::fixed << std::setprecision(2) << rhs.price
      << '\t' << rhs.count;
  return lhs;
}

Book::Book() = default;
Book::Book(const string &isbn_, const int count_, const double price_,
           const string &keywords_, const string &name_, const string &author_)
    : Book() {
  // 进来之前必须先确定参数的合法性。
  count = count_, price = price_;
  strcpy(isbn, isbn_.c_str());
  strcpy(keywords, keywords_.c_str());
  strcpy(name, name_.c_str());
  strcpy(author, author_.c_str());
}

const string Book::Isbn() const { return isbn; }
const string Book::Name() const { return name; }
const string Book::Author() const { return author; }
const string Book::Keywords() const { return keywords; }
const double &Book::Price() const { return price; }

void Book::ChangeIsbn(const string &str) {
  memset(isbn, 0, sizeof(isbn));
  strcpy(isbn, str.c_str());
}
void Book::ChangeName(const string &str) {
  memset(name, 0, sizeof(name));
  strcpy(name, str.c_str());
}
void Book::ChangeAuthor(const string &str) {
  memset(author, 0, sizeof(author));
  strcpy(author, str.c_str());
}
void Book::ChangeKeywords(const string &str) {
  memset(keywords, 0, sizeof(keywords));
  strcpy(keywords, str.c_str());
}
void Book::ChangePrice(const double &num) {
  price = num;
}
// 减掉 quantity 的量而不是直接赋值。
void Book::AddCount(const int &quantity) {
  if (count + quantity < 0) throw Exception();
  count += quantity;
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
    isbn_list.Add(Node(isbn, "", index));
    name_list.Add(Node("", isbn, index));
    author_list.Add(Node("", isbn, index));
    keyword_list.Add(Node("", isbn, index));
  }
  return index;
}

int BookManager::Find(const string &isbn) {
  return isbn_list.Find(Node(isbn));
}

// 注意：修改 ISBN 时会改变所有列表的信息。
void BookManager::ModifyIsbn(const int &index, const string &str) {
  Book tmp;
  books.Read(tmp, index);
  vector<string> keywords;
  SpiltString(tmp.Keywords(), keywords, '|');
  isbn_list.Del(Node(tmp.Isbn()));
  name_list.Del(Node(tmp.Name(), tmp.Isbn()));
  author_list.Del(Node(tmp.Author(), tmp.Isbn()));
  for (auto it : keywords)
    keyword_list.Del(Node(it, tmp.Isbn()));
  tmp.ChangeIsbn(str);
  isbn_list.Add(Node(tmp.Isbn(), "", index));
  name_list.Add(Node(tmp.Name(), tmp.Isbn(), index));
  author_list.Add(Node(tmp.Author(), tmp.Isbn(), index));
  for (auto it : keywords)
    keyword_list.Add(Node(it, tmp.Isbn(), index));
  books.Update(tmp, index);
}
void BookManager::ModifyName(const int &index, const string &str) {
  Book tmp;
  books.Read(tmp, index);
  name_list.Del(Node(tmp.Name(), tmp.Isbn()));
  tmp.ChangeName(str);
  name_list.Add(Node(tmp.Name(), tmp.Isbn(), index));
  books.Update(tmp, index);
}
void BookManager::ModifyAuthor(const int &index, const string &str) {
  Book tmp;
  books.Read(tmp, index);
  author_list.Del(Node(tmp.Author(), tmp.Isbn()));
  tmp.ChangeAuthor(str);
  author_list.Add(Node(tmp.Author(), tmp.Isbn(), index));
  books.Update(tmp, index);
}
void BookManager::ModifyKeywords(const int &index, const string &str,
                                 const vector<string> &new_keywords) {
  Book tmp;
  books.Read(tmp, index);
  vector<string> old_keywords;
  SpiltString(tmp.Keywords(), old_keywords, '|');
  for (auto it : old_keywords)
    keyword_list.Del(Node(it, tmp.Isbn()));
  tmp.ChangeKeywords(str);
  for (auto it : new_keywords)
    keyword_list.Add(Node(it, tmp.Isbn(), index));
  books.Update(tmp, index);
}
void BookManager::ModifyPrice(const int &index, const string &str) {
  Book tmp;
  books.Read(tmp, index);
  tmp.ChangePrice(std::stod(str));
  books.Update(tmp, index);
}
// 买 str 对应的 int 的量的书，返回价格。
double BookManager::BuyBook(const int &index, const int &quantity) {
  Book tmp;
  books.Read(tmp, index);
  tmp.AddCount(-quantity);  // 注意负号。
  books.Update(tmp, index);
  return tmp.Price() * quantity;
}
// 进货
void BookManager::AddBook(const int &index, const int &quantity) {
  Book tmp;
  books.Read(tmp, index);
  tmp.AddCount(+quantity);
  books.Update(tmp, index);
}

void BookManager::PrintIndex(const vector<int> &index) {
  if (index.empty())
    cout.put('\n');
  else
    for (auto it : index) {
      Book tmp;
      books.Read(tmp, it);
      cout << tmp << '\n';
    }
}
void BookManager::ShowIsbn(const string &str) {
  vector<int> index;
  isbn_list.Query(str, index);
  PrintIndex(index);
} 
void BookManager::ShowName(const string &str) {
  vector<int> index;
  name_list.Query(str, index);
  PrintIndex(index);
}
void BookManager::ShowAuthor(const string &str) {
  vector<int> index;
  author_list.Query(str, index);
  PrintIndex(index);
}
void BookManager::ShowKeyword(const string &str) {
  vector<int> index;
  keyword_list.Query(str, index);
  PrintIndex(index);
}