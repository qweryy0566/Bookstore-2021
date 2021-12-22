#ifndef BOOKSTORE_USER_H_
#define BOOKSTORE_USER_H_

#include "exception.hpp"
#include "ull.h"
#include "utils.h"
const int kUserId = 30, kUserName = 30;
enum Privilege { kGuest = 0, kCustomer = 1, kWorker = 3, kRoot = 7 };
// password 和 id 规则相同

bool IsUserName(const string &);
bool IsUserId(const string &);
bool IsUserPassword(const string &);
bool IsUserPrivilege(const string &);

class User {
 private:
  char id[kUserId + 1] = "", password[kUserId + 1] = "";
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
  struct LoginUser {
    Privilege privilege;
    int book_offset = 0;  // 该用户选中的图书在文件中的位置。
    string id;
    LoginUser();
    LoginUser(const User &);
  };
  MemoryRiver<User> users;  // 开头存一个数表示用户数量（不含来宾）
  BlockList list;
  vector<LoginUser> stack;  // TODO : 存在 bug, 密码可能被更改，不过不影响本题。
  unordered_map<string, int> login_id;

  const LoginUser &CurrentUser() const;

 public:
  void Init(const string &);
  void Login(const string &, const string &);
  void Logout();
  void Register(const string &, const string &, const string &);
  void Passwd(const string &, const string &, const string &);
  // 注意对外接口传入参数均为 string。
  void AddUser(const string &, const string &, const string &, const string &);
  void DeleteUser(const string &);
  void SelectBook(const int &);
  // 以下函数都是返回当前登录的用户的对应信息。
  const int &GetBookOffset() const;
  const Privilege &GetPrivilege() const;
};

#endif  // BOOKSTORE_USER_H_