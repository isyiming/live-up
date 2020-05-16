# 在此总结阅读《stl源码剖析》的笔记

将每个容器的数据结构和内置算法用简练的描述总结下来，希望可以作为一个便捷的小手册使用

---
# 第一部分 容器概览
---

序列式容器

name      | 名称       |  插入  |删除 |查找|
-|-|-|-|-
array     |数组          |o(n)|o(n)|o(n)|
vector    |  动态数组    |o(n)|o(n)|o(n)|
list      | 双向循环链表 |o(1)|o(1)|o(n)|
slist      |     单向链表|o(1)|o(1)|o(n)|
deque      |     双向队列|~|~|o(n)|
stack      |     栈|~|~|o(n)|
queue      |     队列|~|~|o(n)|
heap       |     在stl中以算法形式呈现|
priority-queue  优先队列|o(log(n))|~|o(log(n))|


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
# 第二部分 序列式容器
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
```c++
class lists{
protected:
  typedef _list_node<T> lsit_node;
public:
  typedef _list_node* link_type;
protected:
  link_type node;
}
```    
2.非连续空间：list是非连续空间，不可以使用指针++来移动迭代器。但是stl提供了Bidirectional Iterators，同样可以进行前移后移操作，而且不会因为插入等原因造成迭代器失效。list是一个双向链表，实际上它也是一个循环链表（头节点的next指向尾节点）。stl用了一个protected的成员变量node作为list结束标志。所以即使是一个空链表，在stl中它也是有一个节点的。

3.迭代器
list既然是非连续空间，那么它的迭代器就不是一个简单的指针。实际上list的迭代器还是比较简单的，因为每个节点都有两个分别指向pre和next的指针。但是在使用的时候还是可以对迭代器进行++和--等操作。这是因为list的迭代器设计里，通过函数重载符为我们实现了这一点。例如：
```c++
reference operator*()
const
{
  return (*node).data;
}
self& operator++(){
  node=(link_type)((*node).next);
  return *this;
}
`````

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
但是transfer函数只提供给list内部其他函数调用
splice();//将list的某一段节点移动到另一个list上去
merge();//将两个链表归并融合，当然前提是两个链表是递增排序的。但是即使不是递增排序的链表，也不会报error，只是得到的结果不是一个递增序列而已。
reserve();//翻转链表，实现方法就是不断的将当前节点插入到begin()之前。利用transfer实现
list不能使用stl的sort算法，但是list class内置了sort()，是快排
```

## slist 单向链表



## deque 双向队列
1.数据结构
```c++
class deque{
public:
  typedef T value_type;
  typedef value_type* pointer;
  ...
protected:
  typedef pointer* map_pointer;//元素的指针的指针
  map_pointer map;
  size_type map_size;
}
```
2.间断连续空间。deque的实现比较复杂，首先要有一个连续空间map（deque的中控器），map中保存着若干个指针，指向若干个连续空间（缓冲区）。缓冲区就是真正保存数据的地方。这个map我更愿意称之为表。就类似虚函数表的功能，保存着真正的数据区域的地址。
如果deque需要扩容，真正的扩容在map已满的时候。那么就可vector一样，要重新开辟一块空间，把map的内容全部复制过去。

3.迭代器
这本书的作者老是强调deque为我们提供了分段连续空间，如果没有看deque的源码，确实很难想到这一点，因为我们确实可以直接通过索引访问deque的任意一个有效元素。

deque的迭代器为这种遍历的操作创造了条件。如果可以直接通过索引遍历deque的任意一个元素的话，那么迭代器应该能够知道自己是否在一个缓冲区的边缘。如果下一步操作是访问另一个缓冲区的话，它应该可以读取map中的下一个缓冲区的指针。所以迭代器要掌握的有：1）map，2）缓冲区的头和尾，3）和自己在缓冲区的位置。
和list一样，deque的迭代器指针操作都利用了函数重载符来实现，所以我们看到的迭代器指针+ - 等操作都不是真正的指针运算。

## stack 栈

1.概述

    先进后出，只能从栈的顶端访问栈。stl是以deque双向队列来实现栈的。
    那么stack的pop()就是deque的pop_back()
    那么stack的push()就是deque的push_back()
  由于stack以deque为基础来实现的，所以stack一般不被称之为容器，而是container adapter 容器适配器。

  同样的list也可以用来实现stack，我想我们可以比较下用list和deque实现的差异：

      top() 没差
      pop() 弹出 list 时间复杂度o(1) deque 时间复杂度 o(1)
      push()压入 list 时间复杂度o(1) deque 时间复杂度 o(k) k为缓冲区长度
但是如果压入引起了deque扩容的话，用deque来实现就更快了。
那么为啥stl没有用list来实现stack呢？
我感觉是因为迭代器的考虑吧。如果在同一个缓冲区内，deque的迭代器指向的是这个元素的地址是连续的。而list的迭代器指向的地址是非连续的。

2.stack没有迭代器


## queue 队列
1.概述

    先进先出，只能从顶端取出元素，和尾部压入元素。stl也是以deque来实现queue的。
    那么queue 的pop()就是deque的pop_front()
    那么queue 的push()就是deque的push_back()
    同样，list也可以用来实现queue。

## heap 堆
1.概述

    stl中没有把堆作为一个公开的容器。但是它是优先队列的底层实现机制。
    堆是一个完全二叉树。最后一个层之前的每一个层都是满的
    如果一个完全二叉树有n个元素,k层。
    那么它的第i层节点个数就是:i^2
    最后一层的节点个数:n-(2^(k-1)-1)
    而每一个节点m的父节点就是int(m/2)

    正是由于这个二叉树是满的，所以我们没必要真的构建一个真正的树，只需要用一个array就可以表示堆了。
    程序中元素的序号是从0开始的，那这第0个位置stl中就用一个不可能取到的值占据。
    当然，我们也可以在计算父节点位置的时候始终-1.可是这样就太麻烦了。
    stl中提供的heap是max-heap。

2.push_heap

    简单来讲，将要压入的元素插在堆的最后。然后上溯，不断的将新节点和父节点比较。
    newnode>fathernode?swap(newnode,fathernode):break;
    这种上溯操作显然可以用递归来实现，但是递归的代价肯定要大很多。stl中是是用循环来实现的。push_heap()函数接收两个迭代器分别指向heap的头和尾。
3.pop_heap

    pop_heap()的实现和push正好相反。先将最大值（堆根节点，array的第1个元素）取出，然后将堆尾（array的最后一个元素，我们叫它endnode吧）置于堆顶。
    下溯。不断交换endnode和它的子节点，直到它没有子节点为止（只需要判断array长度和2*m的关系就可以了，m代表当前endnode的位置）。
4.sort_heap

    不断地对heap进行pop操作，直到当前堆的长度为1.将弹出的元素存入一个新的array，就可以得到一个有序递减的序列。
5.make_heap

    将现有的一段数据转化为一个heap。例如将一个长度为L的vector生成一个heap
    实现：对vector进行上溯，完成后对vector的前L-1个元素上溯，再对vector的前L-2个元素上溯...

6.heap没有迭代器，实际上heap并没有对我们开放。因为heap仅仅是作为stl的一种底层机制，遍历这种操作在各种容器内部已经有实现（如果可以遍历的话）。既然pop和push都不需要遍历，那也就没有实现的必要了。  

## priority_queue 优先队列

1.概述

    默认以vector实现，和stack一样，它以底层容器vector来实现，所以priority_queue也是一种容器适配器，而不是容器。
    可以使用pair<优先级,value>做为优先队列的元素。
----

---
# 第三部分 关联式容器
---


for(map<int,char>::reverse_iterator rit=m.rbegin();rit!=m.rend();rit++)
