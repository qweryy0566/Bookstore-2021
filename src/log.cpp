#include "log.h"

bool IsTime(const string &s) {
  if (s.length() > 10) return 0;
  for (auto c : s)
    if (!isdigit(c)) return 0;
  return s.empty() || std::stoll(s) < 1ll << 31;
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
  lhs << std::fixed << std::setprecision(2) << "+ " << rhs.income << " - "
      << rhs.expense;
  return lhs;
}

// .......... class Log ..........

Log::Log() = default;
Log::Log(const string &id_, const string &cmd_) : Log() {
  strcpy(user_id, id_.c_str());
  strcpy(command, cmd_.c_str());
}

ostream &operator<<(ostream &lhs, const Log &rhs) {
  lhs << std::setw(kUserId) << std::setfill(' ') << rhs.user_id << "  "
      << rhs.command;
  return lhs;
}

// .......... class LogManager ..........

void LogManager::Init(const string &dir) {
  spend.Init(dir + "finance.bin");
  records.Init(dir + "records.bin");
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
  int total = SpendCount(), need_cnt;
  if (str.empty()) need_cnt = total;
  else {
    need_cnt = std::stoi(str);
    if (need_cnt > total) throw Exception();
    if (!need_cnt) {  // 注意和 "show finance" 且 total = 0 情况区分。 
      cout.put('\n'); return;
    }
  }
  Finance ans, tmp;
  int d = sizeof(Finance) + 4;
  for (int i = 1; i <= need_cnt; ++i) {
    spend.Read(tmp, 8 + (total - i) * d);  // 存储地址可计算，需验证。
    ans += tmp;
  }
  cout << ans << '\n';
}

void LogManager::ReportFinance() {
  Finance ans, tmp;
  int total = SpendCount(), d = sizeof(Finance) + 4, wide = 0;
  for (int i = total; i; i /= 10, ++wide);
  for (int i = 1; i <= total; ++i) {
    spend.Read(tmp, 8 + (total - i) * d);
    cout << std::setw(wide) << std::setfill(' ') << i << ": " << tmp << '\n';
  }
}

void LogManager::AddLog(const Log &record) {
  records.Write(record);
  int count;
  records.ReadInfo(count);
  records.WriteInfo(count + 1);
}

void LogManager::ReportEmployee(const string &id) {

}

void LogManager::ReportLog() {
  
}