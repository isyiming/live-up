# 在此总结阅读《stl源码剖析》的笔记

将每个容器的数据结构和内置算法用简练的描述总结下来，希望可以作为一个便捷的小手册使用

序列式容器

    array           C++自带
    vector
    heap            在stl中以算法形式呈现
    priority-queue  优先队列
    list            双向循环链表
    slist           单向链表
    deque           双向队列
    stack           栈
    queue           队列

    关联式容器
    RB-tree         非公开，红黑树是map等的实现原理
    set
    map
    multiset
    multimap

    hashtable
    hash_set
    hash_map
    hash_multiset
    hash_multimap

---
## vector
1.数据结构
```C++
    class vector{
        protected:
        iterator start;//表示当前使用空间的头
        iterator finish;/表示当前使用空间的尾
        iterator end_of_storage；//表示当前可用空间的尾
    }
```
2.线性连续空间

3.关于扩容：vector会申请一块大于用户申请大小的空间，以备将来可能的扩容。
    增加一个新元素时，如果超过备用容量，vector会直接申请一段2倍大小的新空间，再复制数据到新空间，再释放原空间。所以一旦vector容量改变了，那么原来的迭代器就失效了，因为vector已经转移到了新的地址了。

4.常用函数的实现策略
```c++
    pop_back()//直接将尾端标/Users/ming/本地文稿/GitHub/live-up/C++/C++base.md记finish往前移动一位
    erase(iterator first,iterator last);//将last后的元素都向前移动到first后，再将尾端标记finish往前移动到（last-first）
    clear();//用erase实现
    insert(position,n,x)；//在position前插入n个新节点，需要考虑两种情况，备用空间是否>插入节点个数
```
## list 双向链表
1.数据结构
```c++
struct _list_node{
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
}
```    
2.非连续空间：list是非连续空间，不可以使用指针++来移动迭代器。但是stl提供了Bidirectional Iterators，同样可以进行前移后移操作，而且不会因为插入等原因造成迭代器失效。list是一个双向链表，实际上它也是一个循环链表（头节点的next指向尾节点）。stl用了一个protected的成员变量node作为list结束标志。所以即使是一个空链表，在stl中它也是有一个节点的。

3.常用函数的实现策略
```C++
begin();//返回的是node.next
end();//返回的是node
empty();//return node->next==node;
push_front();//断链，插入到原来的头节点和node节点之间
push_back();//断链，插入到node节点和node prev节点之间
erase(iteration position);//
pop_front();
pop_back();//
clear()//
以上所有函数核心操作都是断链
remove(const T& value);//将和value相等的节点移除
unique();//移除连续而相同的元素

list提供了内置函数transfer实现一段节点迁移到新的位置。实际上还是断链，重连
但是transfer函数只提供给list内部其他函数调用。
splice();//将list的某一段节点移动到另一个list上去
merge();//将两个链表归并融合，当然前提是两个链表是递增排序的。但是即使不是递增排序的链表，也不会报error，只是得到的结果不是一个递增序列而已。
reserve();//翻转链表，实现方法就是不断的将当前节点插入到begin()之前。利用transfer实现
list不能使用stl的sort算法，但是list class内置了sort()

```
