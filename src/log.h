#ifndef BOOKSTORE_LOG_H_
#define BOOKSTORE_LOG_H_

#include "ull.h"
#include "utils.h"

bool IsTime(const string &);

class Finance {
 private:
  double income = 0, expense = 0;

 public:
  Finance();
  Finance(const double &);
  Finance &operator+=(const Finance &);
  friend ostream &operator<<(ostream &, const Finance &);
};

class LogManager {
 private:
  MemoryRiver<Finance, 1> spend;  // 记录数量。

 public:
  void Init(const string &);
  const int SpendCount();
  void AddSpend(const Finance &);
  void ShowFinance(const string &str = "");
};

#endif  // BOOKSTORE_LOG_H_