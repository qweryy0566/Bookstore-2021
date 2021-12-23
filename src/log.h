#ifndef BOOKSTORE_LOG_H_
#define BOOKSTORE_LOG_H_

#include "ull.h"
#include "user.h"
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

class Log {
 private:
  char user_id[kUserId + 1] = "", command[1025] = "";

 public:
  Log();
  Log(const string &, const string &);
  friend ostream &operator<<(ostream &, const Log &);
};

class LogManager {
 private:
  MemoryRiver<Finance, 1> spend;  // 记录数量。
  MemoryRiver<Log, 1> records;

 public:
  void Init(const string &);
  const int SpendCount();
  void AddSpend(const Finance &);
  void ShowFinance(const string &str = "");
  void ReportFinance();
  void AddLog(const Log &);
};

#endif  // BOOKSTORE_LOG_H_