# 走一遍os-tutorial
### 跟：https://github.com/cfenollosa/os-tutorial 把操作系统从源码走一遍
### 配合《深入理解计算机系统》食用
-------------

### 开始前的啰嗦

要想对计算机系统有一个全面的认识，有什么方法比得上自己写一遍更好呢？但是让我真的自己从头写，实在是没那个水平呀。好在早就有了好多开源代码，那就先跟着走一遍。原教程的的readme.md文件中大都只是讲述了程序的编译部署等操作。作者提供了一份文档：https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf 文档配合着程序讲解了一些基础。但是文档更贴近硬件一点，对系统的概念灌输的并不多。还有，教程的中断部分开始就都没有配套文档了，这个教程后续部分作者已经太监了3年多了。哎，说实在的这个教程也没给出多少文档。

另外，这个项目已经是三年前的了，会有一些bug。在这篇文章中，我会记录下自己跑代码时遇到的问题，以及解决方法。本文可以作为跑程序时的参考。另外，仅仅将教程的程序跑一遍，想必对学习系统原理帮助不大。我会配合CSAPP将涉及到的理论基础总结提炼下来。希望会对看到的同学有些许帮助。


### [第一部分：系统的启动](https://github.com/isyiming/live-up/blob/master/OS/OSpart1.md)
#### bios加载引导程序，系统启动
##### macos catalina下使用qemu有bug，无法解决，只能使用妥协。

最重要的我认为就是32位虚拟地址保护模式下的寻址方式，这部分改了好几遍，网上好多博客的要么关键的地方点不到，要么有错误。现在总算是把思路描述的清晰了。了解了怎样寻址的，我们才可以最终实现虚拟内存。

### [第二部分：第一个外设：屏幕](https://github.com/isyiming/live-up/blob/master/OS/OSpart2.md)
#### 最简单的内核代码，完成第一个外设的驱动（屏幕）

### [第三部分：系统中断](https://github.com/isyiming/live-up/blob/master/OS/OSpart3.md)
#### 添加了中断，并实现键盘输入和shell。
接下来的内容，从第18节到第24节那个教程就没有配套文档了，所以，如果你对这部分感兴趣，而直接看代码又感觉很没头绪的话，就看我写的笔记吧～

终于梳理完了中断的执行过程。在CSAPP中并没有详细的解释，而且CSAPP中是在系统的层面将的，称之为异常。着重讲的是系统层面的异常控制。如果你想从头实现虚拟内存，或者这种必须要cpu被动触发的功能，那么一定要掌握中断的原理。

### [第四部分：终篇，物理地址的遍历Kmalloc函数](https://github.com/isyiming/live-up/blob/master/OS/OSpart4.md)
#### 添加了kmalloc，简单的内存动态管理。但是只实现了内存分配，没有释放功能。教程到此为止也就结束了。
##### 修改了程序中页对齐的错误。

实际上这个微型OS没有实现虚拟内存的功能，所以直接添加一个malloc就是内核直接对主存进行操作的。

打印小些字母a的时候显示的是':'，抽时间看下，应该是键盘的编码那里的问题。

## 好了，我们的os-tutorial之旅到此就结束了
#### 我们从系统的启动开始，学习了硬件如何寻址，如何通过程序驱动外设，通过中断实现了系统时钟和被动触发外设的驱动。
#### 最终实现了一个简单的shell，它可以对我们的键盘输入作出反馈。通过屏幕-键盘的交互，我们终于看到了一个简单的系统内核是如何开始工作的。
---------------

不知道你是不是和我一样，有点意犹未尽的感觉。系统的文件，虚拟内存，进程，我们还都没实现呐！这样可不是完整的操作系统～

但是这篇教程已经帮助我们很多了。至少系统启动之前和系统最底层一些对特定IO的操作，涉及到的汇编程序作者都帮我们实现了。所以让我们继续做下去吧！实现一个操作系统吧！

## 再回顾的时候，丰富一些插图。


### [ming_OS:新的起点，从虚拟内存开始](https://github.com/isyiming/ming_OS)
