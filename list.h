#ifndef __list_h__
#define __list_h__

#include <assert.h>

#define __LIST_HEAD__     // ①记录前一个节点的next指针的地址，进行插入节点和直接删除当前节点 ② 使用宏定义实现c++中的模块功能
#define __TLIST_HEAD__    // ① 双向链表 ②使用宏定义实现c++模板功能
#define __KLIST_HEAD__    // ① linux内核中使用的链表，根据结构体中的链表字段，进行获取当前结构体首地址。
/*** 
 * 特点：__LIST_HEAD__ 记录前一个节点的next指针的地址，进行实现直接插入节点的功能和直接删除当前节点。
         __TLIST_HEAD__是双向链表。
         __KLIST_HEAD__是环形链表，也是双向链表
***/

#ifdef __LIST_HEAD__
/*
    第一种list
*/

/***
 * list 定义
 * @param1 [in] name  为list 结构体名。
 * @param2 [in] type  为 list节点的 类型type。
***/
#define LIST_HEAD(name, type)   \
    struct name {               \
        struct type *frist;   \
    }

/***
 * berif: list 的节点定义
 * entry : 名词：条目（也就是每一个节点的意思）； 动词：入口
 * @param1 [in] next  指向下一个节点内存的指针
 * @param2 [in] provious  指向 上一个节点的l_next指针的地址 的指针
 * 
***/
#define LIST_ENTRY(type)        \
    struct {                    \
        struct type *next;      \
        struct type **previous;   \
    }


#define LIST_FRIST(head)    (head).frist
#define LIST_NEXT_ENTRY(elem, field)     (elem)->field.next

/***
 * @brief: 添加节点 头插入法
 * @param1 [in] head  链表头变量名
 * @param2 [in] elem  将要插入的节点全部的变量名
 * @param3 [in] field 表示 elem 里面的 链表 变量名
***/
#define LIST_ADD_HEAD(head, elem, field)                                \
do {                                                                    \
    if ((LIST_NEXT_ENTRY(elem, field) = LIST_FRIST(head)) != NULL) {   \
        LIST_FRIST(head)->field.previous = &LIST_NEXT_ENTRY(elem, field);   \
    }                                                                   \
    LIST_FRIST(head) = elem;                                            \
    elem->field.previous = &LIST_FRIST(head);                         \
}while(0)

/***
 *  brief: 在x节点后面增加 节点y;
 *  @param1 [in] x  在x节点后，插入节点
 *  @param2 [in] y  将要插入的节点全部的变量名
 *  @param3 [in] field 表示 节点全部类型 里面的 链表 变量名
***/
#define LIST_ADD(x, y, field)                                                   \
do {                                                                            \
    assert(x != NULL && y != NULL);                                             \
    if ((LIST_NEXT_ENTRY(y, field) = LIST_NEXT_ENTRY(x, field)) != NULL) {     \
        LIST_NEXT_ENTRY(x, field)->field.previous =                           \
                &LIST_NEXT_ENTRY(y, field);                                     \
    }                                                                           \
    y->field.previous = &LIST_NEXT_ENTRY(x, field);                           \
    LIST_NEXT_ENTRY(x, field) = y;                                              \
}while(0)


/***
 * @brief: 从链表中删除节点elem，需要维护头节点
 * @param1 head: 链表头变量名
 * @param2 elem  表示节点全部类型指针变量
 * @param3 field 表示节点全部类型 中的 链表节点。
 *
***/
#define LIST_REMOVE_ENTRY(head, elem, field)                          \
do {                                                            \
    *(elem->field.previous) = LIST_NEXT_ENTRY(elem, field);   \
    if (LIST_NEXT_ENTRY(elem, field)) {                         \
        LIST_NEXT_ENTRY(elem, field)->field.previous          \
                = elem->field.previous;                       \
    }                                                           \
    if (LIST_FRIST(head) ==  elem) {                            \
        LIST_FRIST(head) = LIST_NEXT_ENTRY(elem, field);        \
    }                                                           \
    LIST_NEXT_ENTRY(elem, field) = NULL;                        \
    elem->field.previous = NULL;                              \
}while(0)

/***
 * @brief: 遍历链表
 * @param1 head: 链表头变量名
 * @param2 pos 为 节点全部类型的指针，作用为了遍历链表，作为下标使用，和for中的i作用一样。
 * @param3 field 表示节点全部类型 中的 链表节点。
***/
#define list_for_each(head, pos, field)         \
for (pos = LIST_FRIST(head);                     \
    pos;                                        \
    pos = LIST_NEXT_ENTRY(pos, field))

#define LIST_NULL_NEXT_ENTRY(elem, field)           \
        (elem) ? LIST_NEXT_ENTRY(elem, field) : NULL

/***
 * @brief: 安全的遍历链表，作用为了避免用户在for循环内，对当前节点进行操作带来的影响。
 * @param1 head: 链表头变量名
 * @param2 pos 为节点全部类型的指针，作用为了遍历链表，作为下标使用，和for中的i作用一样。
 * @param3 n  为节点全部类型的指针，作用：记录进入for循环前的pos的下一个节点指针。
 * @param4 field 表示节点全部类型 中的 链表节点。
***/
#define list_for_each_safe(head, pos, n, field)                     \
for (pos = LIST_FRIST(head), n = LIST_NULL_NEXT_ENTRY(pos, field);  \
     pos;                                                           \
     pos = n, n = LIST_NULL_NEXT_ENTRY(pos, field))


#endif


#ifdef __TLIST_HEAD__

/*链表头节点*/
#define TLIST_HEAD(name, type)      \
    struct name {                   \
        struct type *T_first;       \
        struct type *T_end;         \
        int _size;                  \
    }

/* 初始化链表头节点 */
#define TLIST_HEAD_INITIALIZER(list)   do {     \
    list->T_first = list->T_end = NULL;         \
    list->_size = 0;                            \
} while(0)

/* 链表节点 */
#define  TLIST_ENTRY(type)          \
    struct {                        \
        struct type *T_next;        \
        struct type *T_prev;        \
    }


#define TLIST_FIRST(head)   (head)->T_first
#define TLIST_END(head)     (head)->T_end
#define TLIST_SIZE(head)    (head)->_size
#define LIST_EMPYT(head)    !(head)->_size
#define TLIST_NEXT_ENTRY(elem, field) (elem)->field.T_next
#define TLIST_PREV_ENTRY(elem, field) (elem)->field.T_prev


/***
 *  链表头插入元素, 需要维护头节点，和 尾结点 
 *  @param1 [in] head 表示链表头指针
 *  @param2 [in] elem 要插入的节点全部类型指针
 *  @param3 [in] field 表示节点里面 的 链表字段名
***/
#define TLIST_ADD_HEAD(head, elem, field)                               \
do {                                                                    \
    TLIST_PREV_ENTRY(elem, field) = NULL;                               \
    TLIST_NEXT_ENTRY(elem, field) = NULL;                               \
    if ((TLIST_NEXT_ENTRY(elem, field) = TLIST_FIRST(head)) != NULL) { \
        TLIST_PREV_ENTRY(TLIST_FIRST(head), field) = elem;              \
    }                                                                   \
    TLIST_FIRST(head) = elem;                                           \
    if (TLIST_END(head) == NULL) TLIST_END(head) = elem;                \
    ++(TLIST_SIZE(head));                                               \
} while(0)


/***
 * berif: 表示在x后添加elem元素，需要维护链表的尾结点
 * @param1 [in] head 为链表头指针
 * @param2 [in] x 为链表中的节点（节点全部类型指针）
 * @param3 [in] elem 要添加的节点（节点全部类型指针）
 * @param4 [in] field 为 节点全部类型中的链表字段变量名
***/
#define TLIST_ADD(head, x, elem, field)                                               \
do {                                                                                \
    assert(x != NULL);                                                              \
   if ((TLIST_NEXT_ENTRY(elem, field) = TLIST_NEXT_ENTRY(x, field)) != NULL) {     \
        TLIST_PREV_ENTRY(TLIST_NEXT_ENTRY(x, field), field) = elem;                 \
   }                                                                                \
   TLIST_PREV_ENTRY(elem, field) = x;                                               \
   TLIST_NEXT_ENTRY(x, field) = elem;                                               \
   if(TLIST_END(head) == x) TLIST_END(head) = elem;                                 \
   ++(TLIST_SIZE(head));                                                            \
} while(0)


/***
 * berif: 在链表末尾添加元素
 * @param1 [in] head 表示链表变量
 * @param2 [in] elem 要插入的 节点全部类型指针
 * @param3 [in] field 表示节点里面的链表字段名
***/
#define TLIST_ADD_END(head, elem, field)                    \
do {                                                        \
    if(TLIST_END(head) == NULL) {                           \
        TLIST_ADD_HEAD(head, elem, field);                  \
    }                                                       \
    else {                                                  \
        TLIST_ADD(head, TLIST_END(head), elem, field);      \
    }                                                       \
} while(0)


/***
 * 从链表中去除元素
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] elem 表示链表中需要去除的 节点全部类型的指针
 * @param3 [in] field 表示 节点全部类型 中的链表字段名
***/
#define TLIST_REMOVE_ENTRY(head, elem, field)                                       \
do {                                                                                \
    assert(elem != NULL);                                                           \
    if (TLIST_PREV_ENTRY(elem, field) != NULL) {                                    \
        TLIST_NEXT_ENTRY(TLIST_PREV_ENTRY(elem, field), field) =                    \
                TLIST_NEXT_ENTRY(elem, field);                                      \
    }                                                                               \
    if (TLIST_NEXT_ENTRY(elem, field) != NULL) {                                    \
        TLIST_PREV_ENTRY(TLIST_NEXT_ENTRY(elem, field), field) =                    \
                TLIST_PREV_ENTRY(elem, field);                                      \
    }                                                                               \
    if (TLIST_FIRST(head) == elem) {                                                \
        TLIST_FIRST(head) = TLIST_NEXT_ENTRY(elem, field);                          \
    }                                                                               \
    if (TLIST_END(head) == elem) {                                                  \
        TLIST_END(head) = TLIST_PREV_ENTRY(elem, field);                            \
    }                                                                               \
    TLIST_NEXT_ENTRY(elem, field) = NULL;                                           \
    TLIST_PREV_ENTRY(elem, field)  = NULL;                                          \
    --(TLIST_SIZE(head));                                                           \
} while(0)


/***
 * 从链表头去除元素
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] pos 表示 表示 链表节点全部 类型指针，作用只是用来记录赋值使用
 * @param3 [in] field 表示 节点全部类型 中的 链表字段名
***/
#define TLIST_REMOVE_HEAD(head, pos, field)         \
do {                                                \
    pos = TLIST_FIRST(head);                        \
    TLIST_REMOVE_ENTRY(head, pos, field);           \
} while(0)


/***
 * 从链表尾部去除元素
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] pos 表示 链表节点全部 类型指针，作用只是用来记录赋值使用
 * @param3 [in] field 表示 节点全部类型 中的 链表字段名
***/
#define TLIST_REMOVE_END(head, pos, field)          \
do {                                                \
    pos =  TLIST_END(head);                         \
    TLIST_REMOVE_ENTRY(head, pos, field);           \
} while (0)

/***
 * 正向遍历链表 节点全部类型条目
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] pos 表示 链表节点全部类型指针，作用如同遍历时的 变量i
 * @param3 [in] field 表示 节点全部类型 中的 链表字段名
 *
***/
#define Tlist_for_each(head, pos, field)                    \
    for (pos = TLIST_FIRST(head); pos; pos = TLIST_NEXT_ENTRY(pos, field))

#define TLIST_NULL_NEXT_ENTRY(pos, field)  pos ? TLIST_NEXT_ENTRY(pos, field) : NULL

/***
 * 安全的正向遍历链表 节点全部类型条目。作用：为了避免用户在for循环内，对当前节点进行操作带来的影响。
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] pos 表示 链表节点全部类型指针 作用如同遍，历时的 变量i
 * @param3 [in] n: 表示 链表节点全部类型指针, 作用：在没有进入for前，记录pos节点的下一个节点。
 * @param4 [in] field 表示 节点全部类型 中的 链表字段名
 *
***/
#define Tlist_for_each_safe(head, pos, n, field)                            \
    for (pos = TLIST_FIRST(head), n = TLIST_NULL_NEXT_ENTRY(pos, field);    \
        pos;                                                                \
        pos =n, n = TLIST_NULL_NEXT_ENTRY(pos, field))


#define TLIST_NULL_PREV_ENTRY(pos, field)  pos ? TLIST_PREV_ENTRY(pos, field) : NULL

/***
 * 反向遍历链表 节点全部类型条目
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] pos 表示 作用如同遍历时的 变量i
 * @param3 [in] field 表示 节点全部类型 中的 链表字段名
 *
***/

#define Tlist_for_each_reverse(head, pos, field)                        \
    for (pos = TLIST_END(head); pos; pos = TLIST_PREV_ENTRY(pos, field))

/***
 * 安全的反向遍历链表 节点全部类型条目。作用：为了避免用户在for循环内，对当前节点进行操作带来的影响。
 * @param1 [in] head 表示 链表头变量
 * @param2 [in] pos  表示 链表节点全部类型指针，作用如同遍历时的 变量i
 * @param3 [in] n: 表示 链表节点全部类型指针, 作用：在没有进入for前，记录pos节点的下一个节点。
 * @param4 [in] field 表示 节点全部类型 中的 链表字段名
 *
***/
#define Tlist_for_each_reverse_safe(head, pos, n, field)                    \
    for (pos = TLIST_END(head), n = TLIST_NULL_PREV_ENTRY(pos, field);      \
        pos;                                                                \
        pos = n, n = TLIST_NULL_PREV_ENTRY(pos, field))

#endif



#ifdef __KLIST_HEAD__
/* Stripped down implementation of linked list taken
 * from the Linux Kernel.
 */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

/***
 * linux 内核使用的就是这个链表
***/

/***
 * 链表头和链表节点都是这个结构。
***/
struct klist_head {
    struct klist_head *next, *prev;
};

#define kLIST_HEAD_INIT(name) { &(name), &(name) }

/***
 * 定义并初始化一个链表头
 * 若多个链表，不是用这个宏定义。
 * 一般先定义一个链表头数组，然后使用INIT_KLIST_HEAD函数，进行每一个链表头初始化。
***/
#define kLIST_HEAD(name) \
    struct klist_head name = kLIST_HEAD_INIT(name)

/*使用函数，初始化链表节点 或 链表头*/
static inline void INIT_KLIST_HEAD(struct klist_head *list)
{
    list->next = list;
    list->prev = list;
}

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __klist_add(struct klist_head *new,
                  struct klist_head *prev,
                  struct klist_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */

/***
 * @brief: 添加head后添加一个链表节点。
 * @param1 [in] new： 需要插入的链表节点，结构体指针struct klist_head *
 * @param2 [in] head：在head后插入新节点
***/
static inline void klist_add(struct klist_head *new, struct klist_head *head)
{
    __klist_add(new, head, head->next);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __klist_del(struct klist_head * prev, struct klist_head * next)
{
    next->prev = prev;
    prev->next = next;
}

/* 而这个地址正好是出错信息提示的地址0x00100100 */
/*
 0x00200200 这样的低地址是内核空间申请内存不会出现的地址
 0x00100100和0x00200200 和NULL的作用基本一样，
 兄弟，我觉得把entry->prev 设置为 LIST_POISON2，这样做有一个好处，就是：当你一旦不小心错误的引用了 entry->prev 这个指针的时候，就会打印如下的出错信息：
 Unable to handle kernel paging request at virtual address 00200200
*/
#define POISON_POINTER_DELTA 0
#define LIST_POISON1  ((void *) 0x00100100 + POISON_POINTER_DELTA)
#define LIST_POISON2  ((void *) 0x00200200 + POISON_POINTER_DELTA)

/**
 * list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static inline void __klist_del_entry(struct klist_head *entry)
{
    __klist_del(entry->prev, entry->next);
}

/***
 * @brief: 添加删除一个节点
 * @param1 [in] entry 需要链表去除的链表节点struct klist_head。
 * 
***/

static inline void klist_del(struct klist_head *entry)
{
    __klist_del(entry->prev, entry->next);
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
}

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define klist_entry(ptr, type, member) \
    kcontainer_of(ptr, type, member)

/**
 * list_for_each	-	iterate over a list
 * @pos:	the &struct list_head to use as a loop cursor.
 * @head:	the head for your list.
 */

/***
 * @brief: 只遍历链表节点 struct klist_head
 * @param1 [in] pos 为链表节点指针：struct klist_head *，作用如for中的i一样
 * @param2 [in] head 为链表头节点指针。
 *
***/
#define klist_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the &struct list_head to use as a loop cursor.
 * @n:		another &struct list_head to use as temporary storage
 * @head:	the head for your list.
 */
 /***
 * @brief: 安全的遍历链表节点 struct klist_head，避免进入for内，用户对当前节点指针操作带来的影响。
 * @param1 [in] pos 为链表节点指针：struct klist_head *，作用如for中的i一样
 * @param2 [in] n：为链表节点指针：struct klist_head *。记录没有进入for循环内时，pos的下一个节点指针。
 * @param2 [in] head 为链表头节点指针：struct klist_head *。
***/
#define klist_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)


/***
 * @brief: 由结构体类型 和 结构体内成员字段名 得出 结构体内成员字段在 结构体内的偏移量
 * @param1 [in] TYPE : 结构体类型
 * @param2 [in] MEMBER: 结构体内成员字段名
 * 返回：结构体内成员字段 在 结构体内的偏移量
***/
#define koffsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
/***
 * @brief: 根据一个已知结构体内成员的指针变量 和 结构体内成员字段名得出宿主结构体的地址。
 * @param1 [in] ptr :已知结构体内成员的指针变量（指向是结构体成员的地址）
 * @param2 [in] type: 结构体类型如 struct test_klist;
 * @param3 [in] member: 结构体内成员字段名
 * 返回宿主结构体的首地址
***/
#define kcontainer_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - koffsetof(type,member) );})


/***
 * @brief: 遍历链表对应的条目结构
 * @param1 [out] pos ：条目结构指针。
 * @param2 [in]  head: 链表头指针 ：struct klist_head *
 * @param3 [in]  member 条目结构中的链表字段名。
 * 遍历每一个条目结构，以指针的方式 存入pos中。
***/
#define klist_for_each_entry(pos,  head, member)     \
    for (pos = klist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head);    \
         pos = klist_entry(pos->member.next, typeof(*pos), member))


/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:	the type * to use as a loop cursor.
 * @n:		another type * to use as temporary storage
 * @head:	the head for your list.
 * @member:	the name of the list_head within the struct.
 */
 /***
 * @brief: 安全的遍历链表的条目结构 -> 避免用户在for内对当前指针操作，造成无法遍历的影响。
 * @param1 [out] pos ：条目结构指针。
 * @param2 [in]  n： 条目结构指针，为了在进入for内前，记录pos的下一个条目结构。
 * @param3 [in]  head: 链表头指针 ：struct klist_head *
 * @param4 [in]  member 条目结构中的链表字段名。
 * 遍历每一个条目结构，以指针的方式 存入pos中。
***/
#define klist_for_each_entry_safe(pos, n, head, member)     \
    for (pos = klist_entry((head)->next, typeof(*pos), member), \
        n = klist_entry(pos->member.next, typeof(*pos), member);    \
         &pos->member != (head); 					\
         pos = n, n = klist_entry(n->member.next, typeof(*n), member))


/***
 * @brief: 反向遍历链表对应的条目结构
 * @param1 [out] pos ：条目结构指针。
 * @param2 [in]  head: 链表头指针 ：struct klist_head *
 * @param3 [in]  member 条目结构中的链表字段名。
 * 遍历每一个条目结构，以指针的方式 存入pos中。
***/
#define klist_for_each_entry_reverse(pos,  head, member)        \
            for (pos = klist_entry((head)->prev, typeof(*pos), member);  \
                 &pos->member != (head);                        \
                 pos = klist_entry(pos->member.prev, typeof(*pos), member))


/**
 * list_for_each_entry_safe_reverse
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Iterate backwards over list of given type, safe against removal
 * of list entry.
 */
/***
 * @brief: 安全的反向遍历链表的条目结构 -> 避免用户在for内对当前指针操作，造成无法遍历的影响。
 * @param1 [out] pos ：条目结构指针。
 * @param2 [in]  n： 条目结构指针，为了在进入for内前，记录pos的下一个条目结构。
 * @param3 [in]  head: 链表头指针 ：struct klist_head *
 * @param4 [in]  member 条目结构中的链表字段名。
 * 遍历每一个条目结构，以指针的方式 存入pos中。
***/

#define klist_for_each_entry_safe_reverse(pos, n, head, member)     \
        for (pos = klist_entry((head)->prev, typeof(*pos), member),  \
            n = klist_entry(pos->member.prev, typeof(*pos), member); \
             &pos->member != (head);                        \
             pos = n, n = klist_entry(n->member.prev, typeof(*n), member))


#endif


#endif














