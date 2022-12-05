# list
  list.h是三种链表的实现文件，test_list.c是三种链表的测试文件  
## 三种链表的功能：  
  第一种链表LIST_HEAD：①记录前一个节点的next指针的地址，进行插入节点和直接删除当前节点 ② 使用宏定义实现c++中的模块功能  
  第二种链表TLIST_HEAD：①双向链表  ②使用宏定义实现c++模板功能 （功能比较齐全）  
  第三种链表KLIST_HEAD：①**linux内核使用的链表** 双向环形链表  ② Linux内核中使用的链表，根据**已知结构体中的链表字段地址**，获取**当前结构体的首地址**。  
  另外可以完善一下，nginx中的单链表可一取，取一串的链表。  
## 三种链表的结构图：
### 第一种链表LIST_HEAD串联结构图：  
![image](https://user-images.githubusercontent.com/35031390/205599130-d7d3fc3e-45fd-46df-b364-afd9f492045f.png)
### 第二种链表TLIST_HEAD串联结构图：
![image](https://user-images.githubusercontent.com/35031390/205600966-92ab17b9-6408-4441-bfc4-fd8a18120c66.png)
### 第三种链表KLIST_HEAD
  KLIST_HEAD在这里就不画图了，它是一个双向环形链表。  
  和上结构图不同的时：  
  （1）**这里是双向环形链表**。  
  （2）根据**已知结构体中的链表字段地址**，获取**当前结构体首地址**。  
#### KLIST_HEAD的链表结构
  struct klist_head 结构即是链表头也是链表节点。  
```
/***
 * 链表头和链表节点都是这个结构。
***/
struct klist_head {
    struct klist_head *next, *prev;
};
```
####  KLIST_HEAD结构初始化
```
/*使用函数，初始化链表节点 或 链表头*/
static inline void INIT_KLIST_HEAD(struct klist_head *list)
{
    list->next = list;
    list->prev = list;
}
```
#### 解析container_of() 和 offsetof() 的函数
  container_of()实现了根据一个**已知结构体内成员的指针变量**和**结构体内成员字段名**得出**宿主结构体的地址**的功能。  
  offsetof() 实现了：由 已知 **结构体类型** 和 **结构体内成员字段名** 得出 **结构体内成员字段在 结构体内的偏移量**。  
  两个函数的实现：  
```
/***
 * @brief: 根据一个已知结构体内成员的指针变量 和 结构体内成员字段名得出宿主结构体的地址。
 * @param1 [in] ptr :已知结构体内成员的指针变量（指向是结构体成员的地址）
 * @param2 [in] type: 结构体类型如 struct test_klist;
 * @param3 [in] member: 结构体内成员字段名
 * 返回宿主结构体的首地址
***/
#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


/***
 * @brief: 由结构体类型 和 结构体内成员字段名 得出 结构体内成员字段在 结构体内的偏移量
 * @param1 [in] TYPE : 结构体类型
 * @param2 [in] MEMBER: 结构体内成员字段名
 * 返回：结构体内成员字段 在 结构体内的偏移量
***/
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
```
  两个函数的解析：  
```
修饰符            变量类型                           变量名          右值
const     typeof( ((type*)0)->member )        *__mptr =       (ptr) ;
((type*)0)：它把0转换为一个type类型（也就是宿主结构体类型），为什么要这样做，且看后文
((type*)0)->member：这个0指针指向结构体中的member成员
typeof是gcc的c语言扩展保留字，用于获取变量的类型
typeof( ((type*)0)->member )    *：得出member的数据类型
所以，第2行的结果就是定义一个指向member的指针，并赋值为ptr

第3行，我们先看后半部分 offset(type, member)；
offsetof定义在/include/linux/stddef.h中

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

根据前面的讲解可以知道：&((TYPE *)0)->MEMBER获得MEMBER的地址，此时&stu=0，因为结构体的基地址为0，
所以MEMBER自然为其在结构体中的偏移地址。当然也可以把0换成1，这时&stu=1，要想得出功能成员相对于
宿主结构体的偏移量还得在后面减去1，所以用0的好处就是使得功能成员的地址是相对于宿主结构体的偏移量。
回到第2行，将0改为其他的任意正整数如“1”，并不影响最后的结果，因为在第二行中的目的仅仅是获得member的数据类型而已。

(char *)__mptr - offset(type, member)：用第2行获得的结构体成员地址减去其在结构体中的偏移值便可得到宿主结构体的地址
可能有些同学觉得疑问，为什么要把__mptr转换为char类型的指针呢，C语言中，
一个指向特定数据类型的指针减1，实际上减去的是它所指向数据类型的字节大小sizeof(data)，所以这里要把它转换成char类型，不然得不出正确结果
(type*)( (char *)__mptr - offset(type, member) ) 最后把地址转换成宿主结构体的类型type

由此，container_of()实现了根据一个已知结构体成员的指针和变量名得出宿主结构体的地址的功能
```




  
