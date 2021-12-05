# Bookstore-2021 Document

+ Author >_ wyh



### 实现的类

##### Book 类

```cpp
string isbn_, bookname_, author_;
vector<string> keyword_;
double price_;
int quantity_;
```



##### User 类

```cpp
string name_, password_;
int privilige_;
```



##### Node 类

```c++
int offset_;
string str_;
```



##### Block 类

```cpp
Node array_[SIZE];
Block *prev_, *next_;
void merge(Block&);
Block split();
```



##### BlockList 类 

```cpp
Block *head_, *tail_;
void insert();
void erase();
vector<int> query(string); // 返回匹配元素的索引值
```





### 文件结构

#### 代码文件

+ main.cpp 
  + 初始化
  + 指令读入
  + 异常处理

+ bookstore.cpp/hpp
  + 枚举指令并调用
+ user.cpp/hpp
  + 实现 User 类
  + Login(string, string)
  + Logout()
  + Register(string, string, string)
  + ChangePassword(string, string, string)
  + AddUser(string, string, int, string)
  + DeleteUser(string)

+ book.cpp/hpp

  + 实现 Book 类
  + Show(string)
  + Buy(string, int)
  + Select(string)
  + Modify(string)
  + Import(int, double)

+ log.cpp/hpp

  + ReportMyself()

  + ShowFinance(int)

  + ReportFinance()

  + ReportEmployee()

  + ReportLog()

    **实现：** 因为并不清楚有多少条，故暴力枚举即可。

+ blocklist.cpp/hpp

  （相信 dalao 并不需要用到这些）

  + 实现 Node 类

  + 实现 Block 类

  + 实现 BlockList 类

    **注意：** BlockList 块间按关键字 (ISBN, NAME...) 排序，块内按 ISBN 排序。

+ utils.cpp/hpp

  + 实现一些必要的工具函数



#### 数据文件

+ book.dat

  储存书本信息: int Book*

+ user.dat 

  储存用户信息: int User*

+ finance.dat 

  储存财政信息: int 若干操作

+ log.dat 

  储存日志信息: int 若干操作



#### 索引文件

+ bookisbn.bin
+ bookname.bin
+ bookauthor.bin
+ bookkeyword.bin

按照 4 个关键字索引对应书本的 offset

+ username.bin

按照 name 索引对应 user 的 offset



### 参考文档

[Bookstore Document by lcf.md](https://hub.fastgit.org/PaperL/BookStore_SiriusNEO/blob/master/Bookstore Document by lcf.md)

