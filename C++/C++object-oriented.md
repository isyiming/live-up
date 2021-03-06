
首先看的是博客：http://www.roading.org/develop/cpp/《深度探索c对象模型》笔记汇总.html

博主文采飞扬，可读性非常好。我总想通过笔记摘录下一些要点，供后续面试时回顾使用。本文就是读博客的时候随手想要总结出来的点。
C++对象面面观

C++相比C的额外成本：

* virtual function 机制，用来支持“执行期绑定”。
* virtual base class ——虚基类机制，以实现共享虚基类的 subobject。
* 除此之外C++没有太多理由比C迟缓。

如何面向对象？这个问题我从来没想过。书中给出了三种可以实现面向对象的方案。分别是：
* 简单对象模型
* 表格驱动对象模型，看到这里的时候感觉有点熟悉，原来就是虚函数的实现方式。
* C++对象模型：
    * 所有的非静态数据成员存储在对象本身中
    * 所有的静态数据成员、成员函数（包括静态与非静态）都置于对象之外
    * 用一张虚函数表（virtual table)存储所有指向虚函数的指针，并在表头附加上一个该类的type_info对象，在对象中则保存一个指向虚函数表的指针。
一个类的对象的内存大小包括：

* 所有非静态数据成员的大小。 因为静态数据成员和成员函数保存在类外。
* 由内存对齐而填补的内存大小。
* 为了支持virtual有内部产生的额外负担。
* 而派生类的大小是它的基类大小+它本身的非晶态数据成员的大小



深入C++构造函数

C++标准规定：如果类的设计者并未为类定义任何构造函数，那么会有一个默认
构造函数被暗中生成，而这个暗中生成的默认构造函数通常是不做什么事的(无
用的)，下面四种情况除外。
* 1.包含有带默认构造函数的对象成员的类
* 2.继承自带有默认构造函数的基类的类
* 3.带有虚函数的类
* 4.带有一个虚基类的类
拷贝构造函数的应用

当一个类对象以另一个同类实体作为初值时，大部分情况下会调用拷贝构造函数。
一般是这三种具体情况：
* 显式地以一个类对象作为另一个类对象的初值，形如X xx=x;
* 当类对象被作为参数交给函数时。
* 当函数返回一个类对象时。
* 后两种情形会产生一个临时对象。

如果定义的类没有拷贝构造函数，但是能用常规方法“成员逐一初始化(Default Memberwise Initialization)”来解决拷贝问题的时候。就不需要合成拷贝构造函数了。
成员逐一初始化(Default Memberwise Initialization)具体的实现方式则是位逐次拷贝（Bitwise copy semantics）。
如果数据成员是一个对象，则递归使用“成员逐一初始化(Default Memberwise Initialization)”的手法。

何时编译器会为类合成一个构造函数？
* 当类内含一个成员对象，而后者的类声明有一个拷贝构造函数时（不论是设
计者定义的还是编译器合成的）。
* 当类继承自一个声明有拷贝构造函数的类时（同样，不论这个拷贝构造函数
是被显示声明还是由编译器合成的）。
* 类中声明有虚函数。
* 当类的派生串链中包含有一个或多个虚基类。
有必要记录一下虚基类的概念
在棱形继承是，如果基类A派生出了B和C，而类D继承自B和C，这就是棱形继承。如果A中有一个成员变量int val; 那么在D中是不是会出现两个val？这就产生了歧义。解决的办法就是虚继承。
class B: virtual public A;
class C: virtual public A;
这时候这个被共享的基类就称为虚基类（Virtual Base Class），A 就是一个虚基类。在这种机制下，不论虚基类在继承体系中出现了多少次，在派生类中都只包含一份虚基类的成员。

命名返回值优化和成员初始化队列

Named return Value优化真是一个有意思的小技巧，但是这是编译器来帮我们完成的。博主测试后发现VS2010中，NRV优化是无论在什么情况下都默认开启的。
有一个函数：
X foo(){
    X xx;
    if(...)
        returnxx;
    else
        returnxx;
}
优化后的foo()以result取代xx：
void  foo(X &result){
    result.X::X();
    if(...){
        //直接处理result
        return;
    }
    else{
        //直接处理result
        return;
    }
}
NRV优化后的代码有两个优点：
NRV优化后的代码相较于原代码节省了一个临时对象的空间（省略了xx），直接取消了在函数内部声明XX这个变量，直接将函数的输入改为引用。
同时减少了两次函数调用：
* 减少xx对象的默认构造函数和析构函数（在foo()内部声明xx时）
* 以及一次拷贝构造函数的调用（return时，未优化的foo函数会将xx拷贝给临时变量）
* 增加了一次对a的默认构造函数的调用。（这里的a是啥？）
但是要注意：foo()这样的函数，它的每一个返回分支都返回相同的对象，这时候编译器才会对其进行NRV优化。
5.4日：突然发现这个博客可能好多问题是比较深度的，面试过程中可能不会问。先看网络和系统吧
