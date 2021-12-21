#ifndef BOOKSTORE_ULL_H_
#define BOOKSTORE_ULL_H_

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#include "exception.hpp"
#include "storage.hpp"
using std::string;
using std::vector;
const int kMaxLen = 61;
const int kBlockLen = 1500, kBlockCnt = 1500;

class Node {
 private:
  int offset = 0;  // 可以根据该地址找到原始数据。
  char key[kMaxLen] = "", value[kMaxLen] = "";

 public:
  Node();
  const string Key() const;
  const string Value() const;
  const int &Offset() const;
  explicit Node(const string &, const string &value_ = "",
                const int &offset_ = 0);
  bool operator<(const Node &) const;
  bool operator==(const Node &) const;
  bool operator!=(const Node &) const;
};

class Block {
 private:
  int siz = 0;
  Node array[(kBlockLen << 1) + 5];

 public:
  Block();
  Block(const int &, const Node *);
  const int &Size() const;
  const Node &Front() const;
  const Node &Back() const;
  // 检查块链中是否有 obj，有的话返回在文件中的位置，没有则返回 0
  // （没有数据存在开头）。
  int Find(const Node &) const;
  // 向该块添加元素。如果块过大，返回 false。
  bool Add(const Node &);
  // 删除该块的元素。如果没有该元素，返回 false。
  bool Del(const Node &);
  Block Split(const int &);
  Block &Merge(const Block &);
  void Query(const string &, vector<int> &) const;
};

class BlockIndex {
  friend class BlockList;

 private:
  int cnt = 1, offset[kBlockCnt] = {4};
  Node back[kBlockCnt];

 public:
  void Move(const int &, const int &);
  // 确定 obj 在第几个块中
  int FindPosition(const Node &) const;
};

class BlockList {
 private:
  MemoryRiver<Block> blocks;
  MemoryRiver<BlockIndex> blocks_index;

  void DeleteBlock(BlockIndex &, const int &);

 public:
  void Init(const string &);
  bool Add(const Node &);
  bool Del(const Node &);
  // 检查块链中是否有 obj，有的话返回在文件中的位置，没有则返回
  // 0（没有数据存在开头）。
  int Find(const Node &);
  bool Query(const string &, vector<int> &);
};

#endif  // BOOKSTORE_ULL_H_