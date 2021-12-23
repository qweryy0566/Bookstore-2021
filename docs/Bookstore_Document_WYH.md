# Bookstore-2021 Document

+ Author >_ Jerx2y

+ Modified by qweryy

+ Version 3.0

> 1. 以下该格式的字体、删除线为实现者注。
> 2. 注解的含义是实现者对文档的理解和一些更为具体的实现方式。
> 3. 部分区块在基本不违背原始文件意思的前提下对原始文件进行了重写。

### 代码风格

> 新增。

使用 Google 风格。

### 要实现的类（结构体）

##### Book 类（储存书本信息）

> 修改为有一定封装的类，提供数值、修改接口。

```cpp
class Book {
  friend ostream &operator<<(ostream &, const Book &);  // 重载输出运算符方便输出。

 private:
  // 均有默认值。
  char isbn[kBookIsbn + 1] = "", keywords[kBookKeyword + 1] = "";
  char name[kBookName + 1] = "", author[kBookName + 1] = "";
  long long count = 0;  // 剩余书本数量。
  double price = 0;

 public:
  Book();
  explicit Book(const string &isbn_, const long long count_ = 0,
                const double price_ = 0, const string &keywords_ = "",
                const string &name_ = "", const string &author_ = "");
  // 访问数据的接口。
  const string Isbn() const;
  const string Name() const;
  const string Author() const;
  const string Keywords() const;
  const double &Price() const;
  const long long &Count() const;
  // 修改数据元素的接口。
  void ChangeIsbn(const string &);
  void ChangeName(const string &);
  void ChangeAuthor(const string &);
  void ChangeKeywords(const string &);
  void ChangePrice(const double &);
  void AddCount(const int &);
};
```



##### User 类（储存用户信息）

> 修改为有一定封装的类。

```cpp
enum Privilege { kGuest = 0, kCustomer = 1, kWorker = 3, kRoot = 7 };
class User {
 private:
  char id[kUserId + 1] = "", password[kUserId + 1] = "";
  char name[kUserName + 1] = "";

 public:
  Privilege privilege = kGuest;  // 存储用户的权限。
  User();
  User(const string &, const string &, const string &, const Privilege &);
  // 根据传入的信息（id,password,name,privilege）构造用户。
  // 以下三个成员函数为数据访问接口。
  const string Name() const;
  const string Id() const;
  const string Password() const;
  void ChangePassword(const string &);  // 提供修改密码的接口。
};
```



##### Finance 类（储存每一笔交易信息）

> 重载了加法、输出运算符。

```cpp
class Finance {
  friend ostream &operator<<(ostream &, const Finance &);

 private:
  double income = 0, expense = 0;  // 存储收入、支出（其中一个为 0）

 public:
  Finance();
  Finance(const double &);  // 传入的数值若为正，为收入；负的则为支出。
  Finance &operator+=(const Finance &);  // 支持加法方便计算总收入/支出。
};
```

##### Logging 类（储存每一条日志信息）

> 原始文档：

```cpp
int time; // 操作时间
char userid[MAXLEN]; // 操作的用户 id
bool result; // result 为 0/1 代表指令执行 失败/成功
char command[MAXLEN]; // 储存命令
```

> 目前未实现完毕，只存储了操作用户 id 和命令字符串，如下：
>
```c++
class Log {
  friend ostream &operator<<(ostream &, const Log &);

 private:
  char user_id[kUserId + 1] = "", command[1025] = "";

 public:
  Log();
  Log(const string &, const string &);
};
```

##### Exception 类（异常处理）

> 虽然存下来有助于调试，但对于本题，不需要存储具体错误内容，故存储错误内容暂时没有实现。

```cpp
class Exception {
private:
    string message;  // TODO.
public:
    Exception(const std::string& arg, int type); // 异常类构造函数，type 为异常类型（可以用 enum 类）
    string what() { return message; } // 返回具体异常信息
};
```



##### Storage 类（类的文件读写）

> 1. 原始文档中使用 `std::queue` 存储回收的空间欠妥当，故改为用链表方式连接所有已回收的空间。具体地，每个数据前都存一个 `int` 数值，在该空间被回收时用来记录下一个被回收的空间地址，并在类中记录最后被回收的空间的地址。
> 2. 增加开头存若干个 `int` 的功能，用来存储总数量等信息。

```cpp
template <class T, int info_len = 0>
class MemoryRiver {
 private:
  fstream file;
  string file_name;
  int del_head = 0, sizeofT = sizeof(T);
  // del_head 在最开头，即为被回收的空间链表头（最后被回收的空间的地址）

 public:
  void Init(const string &name);  // 根据传入的数据文件名初始化
  // 读取文件开头第 n 个信息
  void ReadInfo(int &t, const int &n = 1);
  // 写入文件开头第 n 个信息
  void WriteInfo(const int &t, const int &n = 1);
  // 在文件合适位置写入类对象 t，并返回写入的位置索引 index
  int Write(const T &t);
  // 用 t 的值更新位置索引 index 对应的对象
  void Update(const T &t, const int &index = 4);
  //读出位置索引 index 对应的 T 对象的值并赋值给 t
  void Read(T &t, const int &index = 4);
  //删除位置索引 index 对应的对象
  void Delete(const int &index);
};
```



> 在块链中，无需存储原始数据（用户、书）的所有信息，只需存排序关键字和原始数据地址，由此设计以下的 Node 类。

##### Node 类（块状数组中储存元素的节点）

> 除了代码风格的调整外没有较大调整。

```c++
const int kMaxLen = 61;
class Node {
 private:
  int offset = 0;  // offset 为这个节点保存信息在 dat 文件里的位置
  char key[kMaxLen] = "", value[kMaxLen] = "";  // 保存两个关键字，分别为排序关键字和能唯一代表该元素的关键字

 public:
  Node();
  explicit Node(const string &, const string &value_ = "", const int &);
  // 构造函数，将 string 转成 char[] 存储
  // 提供访问数据接口。
  const string Key() const;
  const string Value() const;
  const int &Offset() const;
  bool operator<(const Node &) const;  // 依次按照第一、第二关键字的顺序比较来判断大小
  bool operator==(const Node &) const;
  bool operator!=(const Node &) const;
};
```



##### Block 类（块状数组中的块）

> 1. 增加类似 STL 容器中的获取数组大小、首元素、尾元素（即原文档中的 `maxvar()`）的接口。
> 2. 增加参数为数组首地址和大小的构造函数，能够从数组构造块。
> 3. `add` 改为 `bool Add(const Node &)`，返回是否需要裂块。
> 3. 根据块链类中的 `Query` 函数相应增加块中的 `Query` 函数。
> 3. 增加 `Find(const Node &)` 接口查找块中是否有该元素。
>
> 修改后接口如下：

```cpp
class Block {
 private:
  int siz = 0;
  Node array[(kBlockLen << 1) + 5];

 public:
  Block();
  Block(const int &, const Node *);
  const int &Size() const;  // 返回块大小。
  const Node &Front() const;  // 返回首元素。
  const Node &Back() const;  // 返回尾元素。
  // 检查块链中是否有 obj，有的话返回在文件中的位置，没有则返回 0（没有数据存在开头）。
  int Find(const Node &) const;
  // 向该块添加元素。如果块过大（需要裂块），返回 false。
  bool Add(const Node &);
  // 删除该块的元素。
  void Del(const Node &);
  Block Split(const int &);  // 分裂这个块，并返回后半部分元素构成的块
  Block &Merge(const Block &);  // 合并两个块
  void Query(const string &, vector<int> &) const;
};
```

##### BlockIndex 类（储存块的每个块的信息）

> 原始文档：

```cpp
class BlockIndex {
private:
    int size;
    int offset[kSize]; // 储存每个块在文件中的位置
    Node maxvar[kSize]; // 储存每个块中的最大元素
public:
    BlockIndex();
    bool inrange(const int &pos); // 判断 pos 是否在 [0,size) 中
    int &getoffset(const int &pos); // 返回 pos 对应的 offset 值
    Node &getmaxvar(const int &pos) // 返回 pos 对应的 maxvar 值
    void find(const Node &var, int &pos); // 找到元素对应的块编号，存入 pos
    void extend(const Node &, const Node &, const int, const int &); // 裂块后更新
    void shrink(const Node &, const int &); // 并块后更新
    void query(const string &val, int &lpos, int &rpos); // 询问关键字 val 对应的块的范围
};
```

> 逻辑调整较大。经文档设计者同意后进行如下调整：
>
> 1. 将块链类声明为该索引类的友元。
> 2. 将绝大多数业务调整到块链类中，该类中仅保留查找函数（查找某个元素在哪一个块内）和修改函数（平移索引）。
> 
> 修改后如下：

```c++
class BlockIndex {
  friend class BlockList;

 private:
  int cnt = 1, offset[kBlockCnt] = {4};
  Node back[kBlockCnt];

 public:
  void Move(const int &i, const int &delta);  // 将第 i 个元素平移。
  int FindPosition(const Node &) const;  // 确定 obj 在第几个块中。
};
```

##### BlockArray 类 （文件数据化结构类）

> 1. 增加 `Find(const Node &)` 接口查找块链中是否有该元素。
> 2. 将 `Add` 和 `Del` 函数的参数类型改为 `Node`，有原始参数类型到 `Node` 类型的构造函数。
> 3. 新增私有成员函数 `DeleteBlock` 处理删块。
> 3. `Query` 函数返回值含义为节点的 `offset` 信息，即对应原始数据的存储地址。

```cpp
class BlockList {
 private:
  MemoryRiver<Block> blocks;
  MemoryRiver<BlockIndex> blocks_index;

  void DeleteBlock(BlockIndex &, const int &);

 public:
  void Init(const string &);  // 初始化（绑定文件名）
  bool Add(const Node &);  // 在块链中插入元素
  bool Del(const Node &);  // 在块链中删除元素
  // 检查块链中是否有 obj，有的话返回在文件中的位置，没有则返回 0（没有数据存在开头）。
  int Find(const Node &);
  bool Query(const string &, vector<int> &);  // 第二个是输出参数，返回 offset 数组。
};
```



### 文件结构

#### 代码文件

+ main.cpp 
  + 初始化
  + 指令读入，调用 bookstore 的函数
  + 异常处理：接收各个函数抛出的异常并进行处理
+ commands.cpp/hpp
  + 枚举指令并调用
  + ~~维护一个栈代表当前登录用户，每次操作前检查权限，若不够则抛出异常~~
    
    > 登录栈改到用户管理类中。
  
+ user.cpp/hpp
  + 实现 User 类
  
  以下函数接口均对应题面中给的操作指令，若非法则直接抛出异常：
  
  + void Login(string, string);
  + void Logout();
  + void Register(string, string, string);
  + void ChangePassword(string, string, string);
  + void AddUser(string, string, int, string);
  + void DeleteUser(string);
  > 这些函数统一封装到一个用户管理类中，实现如下：
  >
  > ```c++
  > class UserManager {
  > private:
  > struct LoginUser {
  > Privilege privilege;
  > int book_offset = 0;  // 该用户选中的图书在文件中的位置。
  > string id;
  > LoginUser(const User &);  // 从 User 类元素构造。
  > };
  > MemoryRiver<User> users;  // 开头存一个数表示用户数量（不含来宾）
  > BlockList list;  // 按 id 排序，用于快速查找用户。
  > vector<LoginUser> stack;  // 登录栈，元素类型为上面的内建结构体。
  > unordered_map<string, int> login_id;  // 记录每个 id 当前登录了几次，用于判断一个用户是否处于登录状态。
  > 
  > const LoginUser &CurrentUser() const;
  > 
  > public:
  > void Init(const string &);
  > void Login(const string &, const string &);
  > void Logout();
  > void Register(const string &, const string &, const string &);
  > void Passwd(const string &, const string &, const string &);
  > // 注意对外接口传入参数均为 string。
  > void AddUser(const string &, const string &, const string &, const string &);
  > void DeleteUser(const string &);
  > void SelectBook(const int &);
  > // 以下函数都是对外开放的返回当前登录的用户的对应信息的接口。
  > const int &GetBookOffset() const;
  > const Privilege &GetPrivilege() const;
  > const string &GetId() const;
  > };
  > ```
  >
  > 同时，在该文件中还实现了以下函数用于判断参数的合法性：
  >
  > ```c++
  > bool IsUserName(const string &);
  > bool IsUserId(const string &);
  > bool IsUserPassword(const string &);
  > bool IsUserPrivilege(const string &);
  > ```
  >
  > 
  
+ book.cpp/hpp

  + 实现 Book 类

  以下函数接口均对应题面中给的操作指令，若非法则直接抛出异常：

  + void Show(string);
  + void Buy(string, int);
  + void Select(string);
  + void Modify(string);
  + void Import(int, double);
  >同样统一封装到一个书本管理类中。细节上：
  >
  >1. `Show` 和  `Modify` 函数已改到 `commands.cpp` 中，这里细分为四种对应的函数。
  >2. 由于选中的书本索引信息在用户管理类中，调用 `Buy` 和  `ModifyXXXX` 等函数需要传入选中图书在文件中的地址。
  >
  >实现如下：
  >
  >```c++
  >class BookManager {
  > private:
  >  MemoryRiver<Book> books;
  >  BlockList isbn_list, name_list, author_list, keyword_list;
  >  // 分别是按照对应第一关键字排序的块链。
  >  void PrintIndex(const vector<int> &);  // 根据块链返回的数据地址信息按顺序输出书本信息。
  >
  > public:
  >  void Init(const string &);
  >  int Select(const string &);
  >  // 查找是否已经存在该 isbn.
  >  int Find(const string &);
  >  // Modify 函数
  >  void ModifyIsbn(const int &, const string &);
  >  void ModifyName(const int &, const string &);
  >  void ModifyAuthor(const int &, const string &);
  >  void ModifyKeywords(const int &, const string &);
  >  void ModifyPrice(const int &, const string &);
  >  double BuyBook(const int &, const int &);
  >  void AddBook(const int &, const int &);
  >  // Show 函数
  >  void ShowIsbn(const string &str = "");
  >  void ShowName(const string &);
  >  void ShowAuthor(const string &);
  >  void ShowKeyword(const string &);
  >  const Book GetBook(const int &);  // 根据书本在文件中的地址获取书本数据信息。
  >};
  >```
  >
  >同时，在该文件中还实现了以下函数用于判断参数的合法性：
  >
  >```c++
  >bool IsBookIsbn(const string &);
  >bool IsBookName(const string &);  // author 规则相同
  >bool IsBookAuthor(const string &);
  >bool IsBookKeyword(const string &);  // 单个 keyword
  >bool IsBookCount(const string &);
  >bool IsBookPrice(const string &);
  >```
  >
  >

+ log.cpp/hpp

  以下函数接口均对应题面中给的操作指令，若非法则直接抛出异常：

  + void ReportMyself(); // 枚举操作，找到此人的操作即可

  + void ShowFinance(int); // 直接从文件末尾逆序往前枚举

  + void ReportFinance();

  + void ReportEmployee(); 读出所有操作信息，并输出

  + void ReportLog();
  > 封装到日志管理类中，接口基本一致。

+ ull.cpp/hpp (文件数据结构的实现)

  + 实现 Node 类
  + 实现 Block 类
  + 实现 BlockIndex 类
  + 实现 BlockArray 类
  
+ storage.cpp/hpp

  + 实现 Storage 类

+ utils.cpp/hpp

  + ~~void CheckString(const string &str); // 判断字符串 str 是否合法，若不合法则抛出异常~~

    > 判断字符串是否合法的函数已在用户、书本类的实现文件中实现。

  + void GetInfo(const string &str, string &type, string &val); // str 为类似于 `-name="[book-name]"` 的字符串，将值存在 type 和 val 里

    > 具体地，输出参数改为 `std::pair<string, string>` 类型，实现中函数名改为 `SpiltString`。

  + 实现其他你认为有必要的工具函数，例如 ~~int StringToInt(const string&)~~ 等函数（如具体实现中要用到）

    > 大多使用库函数，例如 `isdigit(), isalnum(), isprint(), std::stoi(), std::stod(), std::to_string()` 等。

+ exception.hpp

  + 实现 Exception 类



#### 数据文件

+ book.dat

  储存书本信息：跟 size 个 Book 结构体储存具体书本信息

+ user.dat 

  储存用户信息：跟 size 个 User 结构体储存具体用户信息

+ finance.dat

  储存交易信息：文件开头是一个整数 size 代表交易数量，后跟 size 个 Finance 结构体储存具体交易信息

+ log.dat 

  储存日志信息：文件储存若干个 Logging 结构体代表具体日志信息

> 后缀名全部改为 `.bin`。

#### 索引文件

+ isbn.book.bin / isbn.book.idx.bin：以 ISBN 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位
+ name.book.bin / name.book.idx.bin：以 书名 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位
+ author.book.bin / author.book.idx.bin：以 作者 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位
+ keyword.book.bin / keyword.book.idx.bin：以 keyword 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位

+ id.user.bin / id.user.idx.bin：以 id 为关键字存放用户的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位

> 文件名有微调，但存储数据方式相同。

### 注意

1. 字符串操作一律用 std::string, 而存储到文件前转成 char[] 来存储；
2. 上文中提到的 `块状数组` 、`BlockArray` 为应该实现的文件数据结构，思想是用每个块维护具体信息，同时用一个数组维护每个块的索引以便快速定位至具体的块；
3. 在函数中，抛出异常应该发生在任何可能的更改之前。

> 实现者完全遵循了上述思想。

### 参考资料

[图书管理系统 - 开发文档](https://github.com/PaperL/BookStore_SiriusNEO/blob/master/Bookstore%20Document%20by%20lcf.md)
