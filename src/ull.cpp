#include "ull.h"

// .......... class Node ..........

Node::Node() = default;
Node::Node(const string &key_, const string &value_, const int &offset_)
    : Node() {
  offset = offset_;
  strcpy(key, key_.c_str());
  strcpy(value, value_.c_str());
}

const string Node::Key() const { return key; }
const string Node::Value() const { return value; }
const int &Node::Offset() const { return offset; }
bool Node::operator<(const Node &rhs) const {
  if (!strcmp(key, rhs.key)) return strcmp(value, rhs.value) < 0;
  return strcmp(key, rhs.key) < 0;
}
bool Node::operator==(const Node &rhs) const {
  return !strcmp(key, rhs.key) && !strcmp(value, rhs.value);
}
bool Node::operator!=(const Node &rhs) const {
  return !(*this == rhs);
}

// .......... class Block ..........

Block::Block() = default;
Block::Block(const int &siz_, const Node *array_) : siz(siz_) {
  memcpy(array, array_, siz_ * sizeof(Node));
}

const int &Block::Size() const { return siz; }
const Node &Block::Front() const { return array[0]; }
const Node &Block::Back() const { return array[siz - 1]; }

int Block::Find(const Node &obj) const {
  int pos = std::lower_bound(array, array + siz, obj) - array;
  return array[pos] == obj ? array[pos].Offset() : 0;
}

// 向该块添加元素。如果块过大，返回 false。
bool Block::Add(const Node &obj) {
  int pos = std::lower_bound(array, array + siz, obj) - array;
  ++siz;
  for (int i = siz - 1; i > pos; --i) array[i] = array[i - 1];
  array[pos] = obj;
  return siz < kBlockLen << 1;
}

// 删除该块的元素。如果没有该元素，返回 false。
void Block::Del(const Node &obj) {
  int pos = std::lower_bound(array, array + siz, obj) - array;
  for (--siz; pos < siz; ++pos) array[pos] = array[pos + 1];
  array[siz] = Node();
}

Block Block::Split(const int &new_block_siz = kBlockLen) {
  siz -= new_block_siz;
  return Block(new_block_siz, array + siz);

}
Block &Block::Merge(const Block &obj) {
  memcpy(array + siz, obj.array, obj.siz * sizeof(Node));
  siz += obj.siz;
  return *this;
}
void Block::Query(const string &key, vector<int> &ans) const {
  for (int i = 0; i < siz; ++i)
    if (key.empty() || key == array[i].Key())  // 空串匹配所有
      ans.push_back(array[i].Offset());
  // 返回 offset.
}

// .......... class BlockIndex ..........

void BlockIndex::Move(const int &i, const int &delta) {
  offset[i] = offset[i + delta], back[i] = back[i + delta];
}
int BlockIndex::FindPosition(const Node &obj) const {
  int ans = std::lower_bound(back, back + cnt, obj) - back;
  return ans - (ans == cnt);
}

// .......... class BlockList ..........

void BlockList::Init(const string &file_name) {
  blocks.Init(file_name + ".bin");
  blocks_index.Init(file_name + "_index.bin");
}

bool BlockList::Add(const Node &obj) {
  BlockIndex index;
  blocks_index.Read(index);
  Block tmp;
  int pos = index.FindPosition(obj);
  blocks.Read(tmp, index.offset[pos]);
  if (!tmp.Add(obj)) {
    ++index.cnt;
    for (int i = index.cnt - 1; i > pos + 1; --i) index.Move(i, -1);
    Block new_block = tmp.Split();
    index.offset[pos + 1] = blocks.Write(new_block);
    index.back[pos + 1] = new_block.Back();
  }
  index.back[pos] = tmp.Back();
  blocks.Update(tmp, index.offset[pos]);
  blocks_index.Update(index);
  return 1;
}

void BlockList::DeleteBlock(BlockIndex &index, const int &pos) {
  if (index.cnt > 1) {
    blocks.Delete(index.offset[pos]), --index.cnt;
    for (int i = pos; i < index.cnt; ++i) index.Move(i, 1);
  } else {
    blocks.Update(Block(), index.offset[0]);
  }
}

// TODO : 对空的块类删除元素会出错。
bool BlockList::Del(const Node &obj) {
  BlockIndex index;
  blocks_index.Read(index);
  Block tmp;
  int pos = index.FindPosition(obj);
  blocks.Read(tmp, index.offset[pos]);
  tmp.Del(obj);
  if (pos + 1 < index.cnt) {
    Block nxt_block;
    blocks.Read(nxt_block, index.offset[pos + 1]);
    if (tmp.Size() + nxt_block.Size() <= kBlockLen) {
      DeleteBlock(index, pos + 1);
      tmp.Merge(nxt_block);
    }
  }
  if (!tmp.Size())
    DeleteBlock(index, pos);
  else {
    index.back[pos] = tmp.Back();
    blocks.Update(tmp, index.offset[pos]);
  }
  blocks_index.Update(index);
  return 1;
}

int BlockList::Find(const Node &obj) {
  BlockIndex index;
  blocks_index.Read(index);
  Block tmp;
  int pos = index.FindPosition(obj);
  blocks.Read(tmp, index.offset[pos]);
  return tmp.Find(obj);
}

// 查询 key 的节点，返回 offset 数组。特别地，空串匹配所有元素。
bool BlockList::Query(const string &key, vector<int> &ans) {
  BlockIndex index;
  blocks_index.Read(index);
  Block tmp;
  for (int i = 0; i < index.cnt; ++i)
    if (key <= index.back[i].Key()) {
      blocks.Read(tmp, index.offset[i]);
      tmp.Query(key, ans);
      if (!key.empty() && key < index.back[i].Key()) break;
    }
  return ans.size();
}