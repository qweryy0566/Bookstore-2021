#include "log.h"

bool IsTime(const string &s) {
  if (s.length() > 10) return 0;
  for (auto c : s)
    if (!isdigit(c)) return 0;
  return 1;
}

// .......... class Finance ..........

Finance::Finance() = default;
// 传入的若为正，为收入；负的为支出。
Finance::Finance(const double &delta) {
  delta > 0 ? (income = delta) : (expense = -delta);
}

Finance &Finance::operator+=(const Finance &rhs) {
  income += rhs.income, expense += rhs.expense;
  return *this;
}
ostream &operator<<(ostream &lhs, const Finance &rhs) {
  lhs << std::fixed << std::setprecision(2)
      << "+ " << rhs.income << " - " << rhs.expense;
  return lhs;
}

// .......... class LogManager ..........

void LogManager::Init(const string &dir) {
  spend.Init(dir + "finance.bin");
}

const int LogManager::SpendCount() {
  int count;
  spend.ReadInfo(count);
  return count;
}

void LogManager::AddSpend(const Finance &record) {
  spend.Write(record);
  int count;
  spend.ReadInfo(count);
  spend.WriteInfo(count + 1);
}

void LogManager::ShowFinance(const string &str) {
  if (!IsTime(str)) throw Exception();
  int total = SpendCount(), need_cnt = str.empty() ? total : std::stoi(str);
  if (need_cnt > total) throw Exception();
  Finance ans, tmp;
  if (need_cnt) {
    int d = sizeof(Finance) + 4;
    for (int i = 1; i <= need_cnt; ++i) {
      spend.Read(tmp, 8 + (total - i) * d);  // 存储地址可计算，需验证。
      ans += tmp;
    }
    cout << ans;
  }
  cout.put('\n');
}