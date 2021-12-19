# Bookstore-2021 Document

+ Author >_ Jerx2y
+ Version 2.0



### 要实现的类（结构体）

##### Book 结构体（储存书本信息）

```cpp
char isbn[MAXLEN], bookname[MAXLEN], author[MAXLEN];
vector<char*> keyword;
double price;
int quantity; // 剩余数量
```



##### User 结构体（储存用户信息）

```cpp
char id[MAXLEN], name[MAXLEN], password[MAXLEN];
int privilige; // 权限
```



##### Finance 结构体（储存每一笔交易信息）

```cpp
double income, expense; // 储存收入，支出（其中一个为 0 ）
```



##### Logging 结构体（储存每一条日志信息）

```cpp
int time; // 操作时间
char userid[MAXLEN]; // 操作的用户 id
bool result; // result 为 0/1 代表指令执行 失败/成功
char command[MAXLEN]; // 储存命令
```



##### Exception 类（异常处理）

```cpp
class Exception {
private:
    string message;
public:
    Exception(const std::string& arg, int type); // 异常类构造函数，type 为异常类型（可以用 enum 类）
    string what() { return message; } // 返回具体异常信息
};
```



##### Storage 类（类的文件读写）

```cpp
template <class T>
class Storage {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    std::queue<long> q; // 回收空间用
public:
    Storage() = default;

    void initialise(string FN); // 绑定文件名为FN的文件
    
    int write(T &t); //在文件合适位置写入类对象t，并返回写入的位置索引index
    
    void update(T &t, const int index); //用t的值更新位置索引index对应的对象
    
    void read(T &t, const int index = 0); //读出位置索引index对应的T对象的值并赋值给t
    
    void Delete(int index); //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)
};
```



##### Node 类（块状数组中储存元素的节点）

```c++
class Node {
private:
    int offset; // offset 为这个节点保存信息在 dat 文件里的位置
    char first[MAXLEN], second[MAXLEN]; // 保存两个关键字，分别为排序关键字和能唯一代表该元素的关键字
public:
    Node();
    Node(string, string, int); // 构造函数，将 string 转成 char[] 存储
    bool operator<(const Node &rhs) const; // 依次按照第一、第二关键字的顺序比较来判断大小
    bool operator==(const Node &rhs) const;
};
```



##### Block 类（块状数组中的块）

```cpp
class Block {
private:
    Node array[kSize];
    int size;
public:
    Block();
    bool empty() const; // 判断是否为空
    Node maxvar() const; // 返回最大元素
    void merge(Block&); // 合并两个块
    Block split(); // 分裂这个块，并返回后半部分元素构成的块
    Block add(const Node&); // 往这个块中添加一个节点，如果块大小大于阈值，返回分裂后后半部分元素构成的块；否则返回空块
    bool dec(const Node&); // 将这个节点从块中删除
};
```



##### BlockIndex 类（储存块的每个块的信息）

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



##### BlockArray 类 （文件数据化结构类）

```cpp
class BlockArray {
private:
    Storage<BlockIndex> blockindex_;
    Storage<Block> block_;
public:
    void initialize(const string&); // 初始化（绑定文件名）
    void insert(const string &first, const string &second, const int &offset); // 插入元素，其关键字为 first 和 second，在文件中的位置为 offset
    void erase(const string &first, const string &second, const int &offset); // 删除元素，同上
    void query(const string &first, vector<int> &res); // 询问第一关键字为 first 的节点的 offset 集合，存在 res 里
};
```



### 文件结构

#### 代码文件

+ main.cpp 
  + 初始化
  + 指令读入，调用 bookstore 的函数
  + 异常处理：接收各个函数抛出的异常并进行处理

+ bookstore.cpp/hpp
  + 枚举指令并调用
  + 维护一个栈代表当前登录用户，每次操作前检查权限，若不够则抛出异常
  
+ user.cpp/hpp
  + 实现 User 类
  
  以下函数接口均对应题面中给的操作指令，若非法则直接抛出异常：
  
  + void Login(string, string);
  + void Logout();
  + void Register(string, string, string);
  + void ChangePassword(string, string, string);
  + void AddUser(string, string, int, string);
  + void DeleteUser(string);
  
+ book.cpp/hpp

  + 实现 Book 类

  以下函数接口均对应题面中给的操作指令，若非法则直接抛出异常：

  + void Show(string);
  + void Buy(string, int);
  + void Select(string);
  + void Modify(string);
  + void Import(int, double);

+ log.cpp/hpp

  以下函数接口均对应题面中给的操作指令，若非法则直接抛出异常：

  + void ReportMyself(); // 枚举操作，找到此人的操作即可

  + void ShowFinance(int); // 直接从文件末尾逆序往前枚举

  + void ReportFinance();

  + void ReportEmployee(); 读出所有操作信息，并输出

  + void ReportLog();

+ ull.cpp/hpp (文件数据结构的实现)

  + 实现 Node 类
  + 实现 Block 类
  + 实现 BlockIndex 类
  + 实现 BlockArray 类
  
+ storage.cpp/hpp

  + 实现 Storage 类

+ utils.cpp/hpp

  + void CheckString(const string &str); // 判断字符串 str 是否合法，若不合法则抛出异常
  + void GetInfo(const string &str, string &type, string &val); // str 为类似于 `-name="[book-name]"` 的字符串，将值存在 type 和 val 里
  + 实现其他你认为有必要的工具函数，例如 int StringToInt(const string&) 等函数（如具体实现中要用到）

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



#### 索引文件

+ isbn.book.bin / isbn.book.idx.bin：以 ISBN 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位
+ name.book.bin / name.book.idx.bin：以 书名 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位
+ author.book.bin / author.book.idx.bin：以 作者 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位
+ keyword.book.bin / keyword.book.idx.bin：以 keyword 为关键字存放书本的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位

+ id.user.bin / id.user.idx.bin：以 id 为关键字存放用户的位置索引 / 分块后块的索引，以 Node / BlockIndex 类为基本单位



### 注意

1. 字符串操作一律用 std::string, 而存储到文件前转成 char[] 来存储；
2. 上文中提到的 `块状数组` 、`BlockArray` 为应该实现的文件数据结构，思想是用每个块维护具体信息，同时用一个数组维护每个块的索引以便快速定位至具体的块；
3. 在函数中，抛出异常应该发生在任何可能的更改之前。



### 参考资料

[图书管理系统 - 开发文档](https://github.com/PaperL/BookStore_SiriusNEO/blob/master/Bookstore%20Document%20by%20lcf.md)
