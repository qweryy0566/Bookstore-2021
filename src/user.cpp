#include "user.h"

bool IsUserName(const string &s) {
  if (s.length() > kUserName) return 0;
  for (auto c : s)
    if (!isprint(c)) return 0;
  return 1;
}
bool IsUserId(const string &s) {
  if (s.length() > kUserId) return 0;
  for (auto c : s)
    if (!isalnum(c) && c != '_') return 0;
  return 1;
}
bool IsUserPassword(const string &s) {
  return IsUserId(s);
}
bool IsUserPrivilege(const string &s) {
  return s.length() == 1 &&
         (s[0] == '0' || s[0] == '1' || s[0] == '3' || s[0] == '7');
}

// .......... class User ..........

User::User() = default;
User::User(const string &id_, const string &password_, const string &name_,
           const Privilege &privilege_)
    : User() {
  if (!IsUserId(id_) || !IsUserPassword(password_) || !IsUserName(name_))
    throw Exception();
  privilege = privilege_;
  strcpy(name, name_.c_str());
  strcpy(id, id_.c_str());
  strcpy(password, password_.c_str());
}

const string User::Name() const { return name; }
const string User::Id() const { return id; }
const string User::Password() const { return password; }

void User::ChangePassword(const string &new_password) {
  if (!IsUserPassword(new_password)) throw Exception();
  memset(password, 0, sizeof(password));
  strcpy(password, new_password.c_str());
}

// .......... class UserManager ..........

UserManager::LoginUser::LoginUser() = default;
UserManager::LoginUser::LoginUser(const User &obj) : LoginUser() {
  privilege = obj.privilege, id = obj.Id();
}

const UserManager::LoginUser &UserManager::CurrentUser() const {
  return stack.back();
}

void UserManager::Init(const string &file) {
  users.Init(file + ".bin");
  list.Init(file + "_list");
  if (!list.Find(Node("root"))) {  // 注意到 root 一旦创建就不可能被删除
    User root("root", "sjtu", "", kRoot);
    int index = users.Write(root);
    list.Add(Node("root", "", index));
  }
  stack.push_back(User("", "", "", kGuest));
}

void UserManager::Login(const string &id, const string &password) {
  User tmp(id, password);
  int index = list.Find(Node(id));
  if (!index) throw Exception();
  users.Read(tmp, index);
  if (!password.empty()) {
    if (password != tmp.Password()) throw Exception();
  } else if (CurrentUser().privilege <= tmp.privilege) {
    throw Exception();
  }
  stack.push_back(tmp);
  ++login_id[id];
}

// 没有特别判断是否有登录账户，因为栈中有一个 0 权的虚账户，不能 logout.
void UserManager::Logout() {
  --login_id[CurrentUser().id];
  stack.pop_back();
}

void UserManager::Register(const string &id, const string &password,
                           const string &name) {
  User tmp(id, password, name);
  if (list.Find(Node(id))) throw Exception();
  int index = users.Write(tmp);
  list.Add(Node(id, "", index));
}

void UserManager::Passwd(const string &id, const string &old_password,
                         const string &new_password) {
  User tmp(id, old_password);
  int index = list.Find(Node(id));
  if (!index) throw Exception();
  users.Read(tmp, index);
  if (!old_password.empty()) {
    if (old_password != tmp.Password()) throw Exception();
  } else if (CurrentUser().privilege != kRoot) {
    throw Exception();
  }
  tmp.ChangePassword(new_password);
  // 该函数内判断了 new_password 的合法性。
  // 由于 id 没变，不需要修改块链内信息。
  users.Update(tmp, index);
}

void UserManager::AddUser(const string &id, const string &password,
                          const string &privilege, const string &name) {
  if (!IsUserPrivilege(privilege)) throw Exception();
  User tmp(id, password, name, (Privilege)std::stoi(privilege));
  if (tmp.privilege >= CurrentUser().privilege || list.Find(Node(id)))
    throw Exception();  // 已判断是否重名。
  int index = users.Write(tmp);
  list.Add(Node(id, "", index));
}

void UserManager::DeleteUser(const string &id) {
  User tmp(id);
  int index = list.Find(Node(id));
  if (!index || login_id[id]) throw Exception();
  users.Delete(index);
  list.Del(Node(id));
}

const int &UserManager::GetBookOffset() const {
  return CurrentUser().book_offset;
}
void UserManager::SelectBook(const int &new_book_offset) {
  stack.back().book_offset = new_book_offset;
}
const Privilege &UserManager::GetPrivilege() const {
  return CurrentUser().privilege;
}