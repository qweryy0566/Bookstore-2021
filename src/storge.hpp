#ifndef BOOKSTORE_STORGE_HPP_
#define BOOKSTORE_STORGE_HPP_

#include <fstream>

using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

template <class T>
class MemoryRiver {
 private:
  fstream file;
  string file_name;
  int del_head = 0, sizeofT = sizeof(T);

 public:
  MemoryRiver() = default;

  explicit MemoryRiver(const string &file_name) : file_name(file_name) {
    file.open(file_name);
    if (!file) {
      file.open(file_name, std::ios::out);
      file.write(reinterpret_cast<char *>(&del_head), 4);
      Write(T());
    } else {
      file.read(reinterpret_cast<char *>(&del_head), 4);
    }
    file.close();
  }

  //在文件合适位置写入类对象 t，并返回写入的位置索引 index
  int Write(const T &t) {
    file.open(file_name);
    int pos, del_nxt = 0;
    if (del_head) {
      file.seekp(del_head), pos = file.tellp();
      file.read(reinterpret_cast<char *>(&del_nxt), 4);
      file.seekg(-4, std::ios::cur); // fstream 的输入和输出指针同步移动
      del_head = del_nxt, del_nxt = 0;
    } else {
      file.seekp(0, std::ios::end), pos = file.tellp();
    }
    file.write(reinterpret_cast<char *>(&del_nxt), 4);
    file.write(reinterpret_cast<const char *>(&t), sizeofT);
    file.close();
    return pos;
  }

  //用 t 的值更新位置索引 index 对应的对象
  void Update(const T &t, const int &index = 4) {
    file.open(file_name);
    file.seekp(index + 4);
    file.write(reinterpret_cast<const char *>(&t), sizeofT);
    file.close();
  }

  //读出位置索引 index 对应的 T 对象的值并赋值给 t
  void Read(T &t, const int &index = 4) {
    file.open(file_name);
    file.seekg(index + 4);
    file.read(reinterpret_cast<char *>(&t), sizeofT);
    file.close();
  }

  //删除位置索引 index 对应的对象
  void Delete(const int &index) {
    file.open(file_name);
    file.seekp(index);
    file.write(reinterpret_cast<char *>(&del_head), 4);
    // 更新 cur_nxt <- del_head
    del_head = index;
    // del_head <- cur
    file.close();
  }
};

#endif  // BOOKSTORE_STORGE_HPP_