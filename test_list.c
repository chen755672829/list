#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

#define  LEN    30
#define TEST_LIST_HEAD   1
#define TEST_TLIST_HEAD  1
#define TEST_KLIST_HEAD  1

#if TEST_LIST_HEAD
struct test_list {
    LIST_ENTRY(test_list) list;  // 链表节点
    int val;
};
LIST_HEAD(list, test_list) hlist;  //定义 链表头
#endif




#if TEST_TLIST_HEAD
// 双向链表
struct test_Tlist {
    int val;
    TLIST_ENTRY(test_Tlist) list;  // 链表节点
};
TLIST_HEAD(TLIST, test_Tlist) Tlist, *Tlist_ptr; // 定义 链表头
#endif




#if TEST_KLIST_HEAD
// 内核使用的链表
struct test_klist {
    int val;
    struct klist_head klist;  // 链表节点
};
kLIST_HEAD(g_klist);       //定义并初始化链表头
#endif 



int main()
{
    int i;

#if TEST_LIST_HEAD
    printf("********************start_test_LIST_HEAD**********************\n");
    struct test_list * tmp = NULL, *pp;
    for (i = 0; i < LEN; i++) {
        tmp = malloc(sizeof(struct test_list));
        tmp->val = i;
        LIST_ADD_HEAD(hlist, tmp, list);
    }

    list_for_each(hlist, tmp, list) {
        printf("tmp->val==%d\n", tmp->val);
    }

    //list_for_each(hlist, tmp, list) 
    list_for_each_safe(hlist, tmp, pp, list)
    {
         LIST_REMOVE_ENTRY(hlist, tmp, list);
         free(tmp);
         tmp = NULL;
         if (LIST_FRIST(hlist))
            printf("head->val==%d\n", LIST_FRIST(hlist)->val);
    }

#endif 



#if TEST_TLIST_HEAD
    printf("********************start_test_TLIST_HEAD**********************\n");
    Tlist_ptr = &Tlist;
    // 初始化链表头结构
    TLIST_HEAD_INITIALIZER(Tlist_ptr);
    struct test_Tlist *kk = NULL, *n;
    for (i = 0; i < LEN; i++) {
        kk = malloc(sizeof(struct test_Tlist));
        kk->val = i;
        if(i % 2 == 0)
            TLIST_ADD_HEAD(Tlist_ptr, kk, list);  // 头插入法
        else TLIST_ADD_END(Tlist_ptr, kk, list);  // 尾插入法
    }
    printf("Tlist_size==%d first->key==%d end->key==%d\n", 
            TLIST_SIZE(Tlist_ptr), TLIST_FIRST(Tlist_ptr)->val, TLIST_END(Tlist_ptr)->val);

    // 正向遍历 链表 节点全部类型条目
    Tlist_for_each(Tlist_ptr, kk, list) 
    {
        printf("kk->val==%d\n", kk->val);
    }
    
    printf("--------------------------\n");
    // 安全正向遍历 链表 节点全部类型条目
    Tlist_for_each_reverse(Tlist_ptr, kk, list) 
    {
        printf("kk->val==%d\n", kk->val);
    }
    printf("--------------------------\n");

#define T_REMOVE 0
#if T_REMOVE
    Tlist_for_each_reverse_safe(Tlist_ptr, kk, n, list) {
        TLIST_REMOVE_ENTRY(Tlist_ptr, kk, list);  //从链表中删除节点kk
        free(kk);
        kk = NULL;
        if (!LIST_EMPYT(Tlist_ptr)) {
            printf("Tlist_size==%d first->key==%d end->key==%d\n", 
                            TLIST_SIZE(Tlist_ptr), TLIST_FIRST(Tlist_ptr)->val, TLIST_END(Tlist_ptr)->val);
        }
    }
#else 

    while(!LIST_EMPYT(Tlist_ptr)) {
        TLIST_REMOVE_HEAD(Tlist_ptr, kk, list);  //从链表头删除
        free(kk);
        kk = NULL;
        if(!LIST_EMPYT(Tlist_ptr))
            printf("Tlist_size==%d first->key==%d end->key==%d\n", 
                            TLIST_SIZE(Tlist_ptr), TLIST_FIRST(Tlist_ptr)->val, TLIST_END(Tlist_ptr)->val);
    }

#endif

#endif 



#if TEST_KLIST_HEAD
    printf("********************start_test_KLIST_HEAD**********************\n");
    struct test_klist * ptr;
    for(i = 0;i < LEN; i++) {
        ptr = malloc(sizeof(struct test_klist));
        if(ptr == NULL) {
            break;
        }
        ptr->val = i;
        INIT_KLIST_HEAD(&ptr->klist);
        klist_add(&ptr->klist, &g_klist);
    }
#if 0
    // 遍历
    struct klist_head *head = &g_klist, *pos;
    klist_for_each(pos, head) {
        ptr = klist_entry(pos, struct test_klist , klist);
        printf("ptr->val==%d\n", ptr->val);
    }

#elif 0
    // 正向遍历 并删除节点
    struct klist_head *head = &g_klist;
    struct test_klist *pos, *next;
    klist_for_each_entry(pos, head, klist) {
        printf("----pos->val==%d\n",pos->val);
    }
    klist_for_each_entry_safe(pos, next, head, klist) {
        printf("pos->val==%d\n",pos->val);
        klist_del(&pos->klist);
        free(pos);
    }
#else
    // 反向遍历 并删除节点
    struct klist_head *head = &g_klist;
    struct test_klist *pos, *next;
    klist_for_each_entry_reverse(pos, head, klist) {
        printf("----pos->val==%d\n",pos->val);
    }
    klist_for_each_entry_safe_reverse(pos, next, head, klist) {
        printf("pos->val==%d\n",pos->val);
        klist_del(&pos->klist);
        free(pos);
    }

#endif
#endif


    return 0;
}


