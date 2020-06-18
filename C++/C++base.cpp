# 在此总结C++的基础

1. 输入参数是指针的，一定要判断是否为nullptr，否则函数体中的对指针进行取值取地址操作直接造成程序崩溃。

2. 类成员的初始化是按照成员变量的声明顺序初始化的，而不是构造函数的初始化列表。

3. 在main之前声明的类的构造函数会先于main执行。

4. new原理：会调用operator new这个全局函数，operator new会调用malloc分配内存空间。

5. malloc/free和new/delete的区别
  malloc和free是函数，new和delete是操作符
  malloc申请的空间不会初始化，new可以初始化
  malloc申请空间时，需要手动计算空间大小并传递，new只需在其后跟上空间的类型即可
  申请自定义类型对象时，malloc/free只会开辟与销毁空间，不会调用构造函数与析构函数，而new在申请空间后会调用构造函数完成对象的初始化，delete在释放空间前会调用析构函数完成空间中资源的清理








1.typedef
        给复杂变量类型创造别名
        typedef void* void_pointer;

2.虚函数

3.智能指针

4.内联函数

5. 线程同步：临界区，互斥量，信号量
  临界区内的变量，一个线程访问后，另一个线程就会阻塞等待。可以配合信号量使用，信号量为1的时候临界区可读可写，信号量为0的时候临界区只可以读。

6. 单例模式
  在类中添加static类型的实例instance，只有instance==null时，构造函数才会instance=new Singleton;
  在构造函数中：首先判断instance==null，然后判断同步锁状态，再次判断instance==null，三个条件都满足才会创建instance；

7. 请设计一个类，该类只能在堆上创建对象
  构造函数中
      return new HeapOnly;
  如果 return StackOnly();，没有用new，由于是函数中返回的临时变量，那么就是在栈上的。





C++ 11新特性

5. 右值引用 &&
  int &&rr=i   //错误，不能将一个右值引用绑定到一个左值上
  int &&r2=i*42; //正确，将rr2绑定到乘法结果上




http://c.biancheng.net/view/199.html

#### 内联函数和宏
