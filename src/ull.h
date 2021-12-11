#ifndef BOOKSTORE_ULL_H_
#define BOOKSTORE_ULL_H_

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#include "storge.hpp"
using std::string;
using std::vector;
const int kMaxLen = 65;
const int kBlockLen = 384, kBlockCnt = 272;

class Node {
 private:
  int offset = 0, value;
  char key[kMaxLen] = "";

 public:
  Node();
  string Key() const;
  const int &Value() const;
  Node(const string &, const int &, const int &offset_ = 0);
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
  int cnt = 1, offset[kBlockCnt] = {0};
  Node back[kBlockCnt];

 public:
  void Move(const int &, const int &);
  int FindPosition(const Node &) const;
};

class BlockList {
 private:
  MemoryRiver<Block> blocks;
  MemoryRiver<BlockIndex> blocks_index;

  void DeleteBlock(BlockIndex &, const int &);

 public:
  BlockList();
  explicit BlockList(const string &);
  bool Add(const Node &);
  bool Del(const Node &);
  bool Query(const string &, vector<int> &);
};

#endif  // BOOKSTORE_ULL_H_