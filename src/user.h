#ifndef BOOKSTORE_USER_H_
#define BOOKSTORE_USER_H_

#include <cctype>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "exception.hpp"
#include "ull.h"
using std::string;
using std::unordered_map;
using std::vector;
const int kMaxOperation = 1e5 + 9;
const int kUserId = 30, kUserPassword = 30, kUserName = 30;
enum Privilege { kGuest = 0, kCustomer = 1, kWorker = 3, kRoot = 7 };

bool IsUserName(const string &);
bool IsUserId(const string &);
bool IsUserPassword(const string &);
bool IsUserPrivilege(const string &);

class User {
 private:
  char id[kUserId + 1] = "", password[kUserPassword + 1] = "";
  char name[kUserName + 1] = "";

 public:
  Privilege privilege = kGuest;
  User();
  // 默认等级为 1
  User(const string &, const string &password_ = "", const string &name_ = "",
       const Privilege &privilege_ = kCustomer);
  const string Name() const;
  const string Id() const;
  const string Password() const;
  void ChangePassword(const string &);
};

class UserManager {
 private:
  MemoryRiver<User, 1> users;  // 开头存一个数表示用户数量（不含来宾）
  BlockList list;
  vector<User> stack;  // TODO : 存在 bug, 密码可能被更改，不过不影响本题。
  unordered_map<string, int> login_id;

  const int Count();
  void ChangeCount(const int &);
  const User &CurrentUser() const;

 public:
  void Init(const string &);
  void Login(const string &, const string &);
  void Logout();
  void Register(const string &, const string &, const string &);
  void Passwd(const string &, const string &, const string &);
  // 注意对外接口传入参数均为 string。
  void AddUser(const string &, const string &, const string &, const string &);
  void DeleteUser(const string &);
};

#endif  // BOOKSTORE_USER_H_