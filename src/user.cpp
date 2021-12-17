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
  if (s.length() > kUserPassword) return 0;
  for (auto c : s)
    if (!isalnum(c) && c != '_') return 0;
  return 1;
}
bool IsUserPrivilege(const string &s) {
  if (s.length() > 1 || !isdigit(s.front())) return 0;
  return s.front() == '0' || s.front() == '1' || s.front() == '3' ||
         s.front() == '7';
}

// .......... class User ..........

User::User() = default;
User::User(const string &id_, const string &password_, const string &name_,
           const Privilege &privilege_) {
  if (!IsUserId(id_) || IsUserPassword(password_) || !IsUserName(name_))
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
  strcpy(password, new_password.c_str());
}

// .......... class UserManager ..........

const int UserManager::Count() {
  int tmp;
  users.ReadInfo(tmp, 1);
  return tmp;
}
// 使得数量加上 delta
void UserManager::ChangeCount(const int &delta) {
  int tmp;
  users.ReadInfo(tmp, 1);
  tmp += delta;
  users.WriteInfo(tmp, 1);
}

UserManager::UserManager(const string &file)
    : users(file + ".bin"), list(file + "list") {
  if (!Count()) {
    User root("root", "sjtu", "", kRoot);
    int index = users.Write(root);
    list.Add(Node("root", 0, index));
    ChangeCount(1);
  }
  stack.push_back(User("", "", "", kGuest));
}

const User &UserManager::CurrentUser() const { return stack.back(); }

// {0}
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
// {1}
void UserManager::Logout() {
  if (CurrentUser().privilege < 1) throw Exception();
  stack.pop_back();
}
// {0}
void UserManager::Register(const string &id, const string &password,
                           const string &name) {
  User tmp(id, password, name);
  if (list.Find(Node(id))) throw Exception();
  int index = users.Write(tmp);
  list.Add(Node(id, 0, index));
}
// {1}
void UserManager::Passwd(const string &id, const string &old_password,
                         const string &new_password) {
  if (CurrentUser().privilege < 1) throw Exception();
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
  users.Update(tmp, index);
}
// {3}
void UserManager::AddUser(const string &id, const string &password,
                          const string &privilege, const string &name) {
  if (CurrentUser().privilege < 3) throw Exception();
  if (!IsUserPrivilege(privilege)) throw Exception();
  User tmp(id, password, name, (Privilege)std::stoi(privilege));
  if (tmp.privilege >= CurrentUser().privilege || list.Find(Node(id)))
    throw Exception();
  int index = users.Write(tmp);
  list.Add(Node(id, 0, index));
}
// {7}
void UserManager::DeleteUser(const string &id) {
  if (CurrentUser().privilege < 7) throw Exception();
  User tmp(id);
  int index = list.Find(Node(id));
  if (!index || login_id[id]) throw Exception();
  users.Delete(index);
  list.Del(Node(id));
  --login_id[id];
}