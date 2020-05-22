# 走一遍os-tutorial
### 跟：https://github.com/cfenollosa/os-tutorial 把操作系统从源码走一遍
### 配合《深入理解计算机系统》食用
- 要想对计算机系统有一个全面的认识，有什么方法比得上自己写一遍更好呢？但是让我真的自己写，哈哈哈
- 好在早就有了好多开源代码，那就先跟着走一遍
- 在这篇文章中，我会记录下自己跑代码时遇到的问题，以及解决方法
- 另外，会配合CSAPP将涉及到的理论基础总结提炼下来
- 希望会对看到的同学有些许帮助  
-------------

原教程的的readme.md文件中大都只是讲述了程序的编译部署等操作。作者提供了一份文档：https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf文档配合着程序讲解了一些基础。 但是文档更贴近硬件一点，对系统的概念灌输的并不多。

另外，这个项目已经是三年前的了，会有一些bug，本文可以作为跑程序时的参考。仅仅将教程的程序跑一遍，想必对学习系统原理帮助不大。还是边跑教程边回顾下系统的知识点，即可以作为之前学习的总结，又可以帮助思考为什么这样实现。

还有，教程的中断部分之后都没有配套文档了，而且这个教程后续部分作者已经太监了3年多了。

### [第一部分：系统的启动](https://github.com/isyiming/live-up/blob/master/OS/OSpart1.md)
#### 内容：bios加载引导程序
macos catalina下使用qemu有bug，无法解决，只能使用妥协。

### [第二部分：从简单程序迈向系统内核](https://github.com/isyiming/live-up/blob/master/OS/OSpart2.md)
#### 最简单的内核代码：完成第一个外设的驱动（屏幕）

接下来的内容，从第18节到第24节那个教程就没有配套文档了，所以，如果你对这部分感兴趣，而直接看代码又感觉很没头绪的话，就看我写的笔记吧～
### [第三部分：逐步完善的系统内核](https://github.com/isyiming/live-up/blob/master/OS/OSpart3.md)
#### 添加了中断，并实现键盘输入和shell。

### [第四部分：终篇，实现内存管理](https://github.com/isyiming/live-up/blob/master/OS/OSpart4.md)
#### 添加了malloc，实现了简单的内存动态管理。实际上这个微型OS没有实现虚拟内存的功能，所以这个malloc非常简单，就是内核直接对主存进行操作的。
