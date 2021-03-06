# 在此总结阅读《stl源码剖析》的笔记

将每个容器的数据结构和内置算法用简练的描述总结下来，希望可以作为一个便捷的小手册使用

---
# 第一部分 容器概览
---

### 序列式容器

name      | 名称       |
-|-|
array     |  数组        |
vector    |  动态数组     |
list      |  双向循环链表 |
slist      | 单向链表     |
deque      | 双向队列     |
stack      | 栈          |
queue      | 队列         |
heap       | 在stl中以算法形式呈现|
priority-queue | 优先队列 |

### 关联式容器

name      | 名称       |
-|-|
set     |集合          |
multiset    |  可重复集合    |
map      | 键值-实值索引 |
multimap      |  可重复键值-实值索引|
hash_set      |     |
hash_map      |     |
hash_multiset      |     
hash_multimap       |     
RB-tree | |
hashtable | |

见过的面试题目
两个stack实现一个queue，其实stl里这两个都是用deque来实现的。。。所以他们两个的相互转化。。。


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
    pop_back()//直接将尾端标记finish往前移动一位
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
  node=(link_type)((* node).next);
  return * his;
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

    它以堆来实现，可以使用pair<优先级,value>做为优先队列的元素。
----

---
# 第三部分 关联式容器
---

## 树导论
容器内部以RB-tree或者hash-tree来实现。关联式容器每一个元素都有一个键值和一个对应的实值。按照键值大小，以对应规则将值放在适当位置。关联容器内部是树，所以没有所谓从头到尾，只有最大元素和最小元素。

如果关联容器是一个平衡二叉树，那么它的操作效率是最高的。但是平衡二叉树的维护代价也很高。所以有了RB-tree。

树的概念：

    节点nodes和边edges
    根节点root
    父节点
    子节点
    叶子节点
    兄弟节点
    根节点到任何节点的路径长度，即边数，为节点深度
    某节点到其最深叶子节点的边数为节点的高度

### 二叉搜索树

    任何节点的键值一定大于左子树中的每一个节点的键值，而且小于右子树中的每一个节点的键值。
    二叉搜索树的最左叶子就是树中的最小值
    二叉搜索树的最右叶子就是树中的最大值
    二叉搜索树的插入和搜索的时间复杂度o(log(n))
### 平衡二叉树

    如果一个二叉树极端不平衡，每一节点都只有左子节点。这时候二叉树就退化为一个单向链表了。
    实际上搜索树的查找就是二分嘛，如果退化成了一个链表，那就没有二分了，而是从头到尾的遍历了。
    所以一个有效可靠的搜索树，应该是平衡的，这就是平衡二叉树。平衡二叉树的效率高，是和相同节点个数的不平衡二叉树相比的。书中的图5-7的举例很容易有歧义。

### AVL tree

    要求任何节点的左右子树的高度相差最多是1
    插入节点后破坏了树的平衡性，只需要调整插入点到跟节点路径上平衡状态被破坏最深的节点X就可以了。
    有四种情况：
      1.插入节点在X左子树的左节点（外侧）
      2.插入节点在X左子树的右节点（内侧）
      3.插入节点在X右子树的左节点（内侧）
      4.插入节点在X右子树的右节点（外侧）
      外侧插入用单旋转解决，内侧插入用双旋转解决

我一直对单旋转和双旋转这两个称谓不解，我没体会到那里旋转了。我是把单旋转想象成拎葡萄串，原本是手拎着节点X，单旋转就相当于把X较深的子节点拎起来，然后再把～～～～我描述不出来了，反正我自己懂哈哈哈

双旋转可以由两次单旋转操作完成。


### RB tree

1.RB tree是stl实现关联容器的底层机制
    每个节点非红即黑
    根节点是黑色
    如果某节点是红色，那它的两个子节点一定是黑色
    任意节点到它的每一个叶子节点的黑色节点数都要相同。
    新增节点一定是红色，新增节点的父节点一定是黑色

    RB tree之所以作出黑红节点的位置和数量限制，就是为了维持一种弱平衡状态。相比于ALV tree，RB tree出现左右子数高度差为2的情况很正常，看起来RB tree更为不平衡一点。
    插入节点如果树不平衡了，ALV tree 和RB tree 都是两次旋转即可恢复平衡
    删除节点如果树不平衡了，ALV tree 需要维护路径上全部节点，和RB tree 需要最多三促旋转。
    RB tree的查找比ALV tree有时候会多一次比较，因为层差可以大一层。
2.节点设计

    节点比普通的二叉树多了一个指向父节点的指针。还有节点除了val外还有颜色属性。
3.迭代器

    RB-tree的迭代器其实就是基于搜索树的遍历规则。
    迭代器的++和--操作只要按照搜索树的规则，我们就都可以想到具体的实现。
    例如++操作，就是要找比当前节点大的第一个节点嘛，如果当前节点有右子树，那就找到右子树的最左叶子节点。
    要是没有右子树，那就找到父节点，恰好我们有指向父节点的指针。
    这时候右分两种情况，如果当前节点是父节点的右子节点，
    那就上溯，直到找到当前节点的一个右祖先（其实没有这个词，我的意思就是当前节点在这个祖先的左子树上）
    也有特殊情况，比如想要找根节点的右节点的时候，这时候如果根节点没有右节点，stl使用了一个header指向了跟节点。总之这是编程时的一个小flag。并没有什么深奥的技法。

### set 集合

1.概述

    set就是将元素按照搜素树的顺序填入rb-tree，所以stl中的set就是一个有序的树，利用stl提供的迭代器我们才可以顺序访问它。
    既然set的元素值就是键值，就是代表它的排列顺序的值，所以set没有更改元素的操作。这一点在看完rb-tree真的是明明白白的事情。
    想要改这个值，其实也可以，那就是把当前节点删除，然后在树的最后插入一个新节点。
    所以set索性就不提供更改操作了，只能删除和插入。
    底层源码决定了set拥有的特性，
### map

1.概述

    map的元素都是根据键值大小排序的，键值决定了对应的实值的位置。
    map不能有两个不同元素拥有相同的键值，确实如此，你想插入一个同名键值的元素的时候，一定会将原来的元素覆盖掉。
    map可以利用迭代器遍历和修改实值。但是键值不能更改，只能删除元素，再插入。
    因为map的底层实现是树，所以和list这种非连续空间的容器一样，插入删除操作之前的迭代器都是有效的。当然被删除的那个元素的迭代器肯定失效了。
### multiset

    允许键值重复的set，也就是允许重复元素的集合。
    它的插入采用的是rb-tree的insert_equal而非insert_unique。

### multimap

    允许键值重复的map，也就是允许重复元素的字典。
    它的插入采用的是rb-tree的insert_equal而非insert_unique。



### hashtable

    以rb-tree为基础的容器实现的索引具有对数时间复杂度。即使数据量很大的时候，我们的搜索复杂度增长的也很慢。
    而hashtalbe可以实现常数时间复杂度。如果实现真正的立即搜索，最快的办法就是直接直到待搜索元素的地址。
    这样不具备实现的基础。
    hashtable的实现方法就是想办法将元素的值和存储位置做一个映射（其实就是取余数）。但是给每一个元素赋予一个映射值显然代价太大了。而少量的映射值会带来碰撞问题（相同的元素对应了同一个索引）。
    一个良好的hashtable就是要解决碰撞问题。
    stl用的是开链法，我们就看下开链法好了。

用vector保存若干个桶bucket，将待存入元素与vector长度取模的结果就代表元素的位置。每个bucket内都保存着取模结果相同的元素，这些元素保存在一个单向链表里，链表的尾部指向下一个bucket，若需要扩容，就直接vector扩容就好了。

这里要注意的一个问题就是，bucket的vector的长度总是选取一个质数。
假如我们设定vector长度是8。我们就从二进制取余数来看吧。
如果是一个二进制数对0b1000取模，右数第4位肯定是没用的对吧。
那么这个hash就固定有一位失效了，没有对元素映射起到作用。
但是质数的余数只有它的倍数会出现这种情况

hashtable的内存释放，已经知道hashtable内部是由vector和list构成的。那么它的clear操作，除了遍历bucket释放list节点，就没什么别的方法了。

既然hashtable是以取模运算为基础的，那么很自然它不能处理浮点型和string这种没办法直接取模的输入类型。

### hashset

    hashset与set的用法完全相同，注意hashset和set的一个非常明显的区分，hashset内部元素是无序的。

### hashmap

    hashmap的用法和map完全相同，但是hashmap是无序的


### hash_multimap

    hash_multimap的用法和map完全相同，但是hash_multimap是无序的

### hash_multimap

    hash_multimap的用法和map完全相同，但是hash_multimap是无序的


## 质变算法和非质变算法

    质变指的是进行拷贝插入删除等会更改区间内迭代器所指元素内容的算法
for(map<int,char>::reverse_iterator rit=m.rbegin();rit!=m.rend();rit++)
