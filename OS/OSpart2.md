# 走一遍os-tutorial

## 第二部分：从简单程序迈向系统



#### 第十四节   创建一个简单的内核和一个能够引导它的引导程序

在第十三节中，我们就已经在虚拟器上运行了一段代码了。这一步相当于什么呢，感觉就像在单片机上运行了一段程序，就好比点亮小灯。这是我们直接运行于硬件（虽然我们是在虚拟机上）上的代码。尽管它很简单，只能在屏幕上打印一个x出来。但是我们已经可以看到内核雏形了，不对，应该是看到了未来组成内核的核糖核苷酸吧。我们再努力下，让他早日变成受精卵。

安装调试工具gdb

    作者提供的方法又是由源码安装。这个项目毕竟是三年前的了，如今brew里已经有gdb了。所以直接用brew安装gdb就ojbk～

跑下本节代码

    首先make，得到os-image.bin
    然后运行下：qemu-system-i386 -curses -fda os-image.bin
    但是打开makefile看下代码你就会发现，哎，make run 和 make debug这个两个命令也有哦
    make run就是直接编译链接后运行bin文件了，
    make debug 就是以debug模式运行，这要多亏我gdb～

我们再来看下项目的文件结构吧

    了一个boot文件夹：
      这个在win系统下就有哦，win系统里有一些字体，还有efi那是引导文件。
      作者在这个文件夹下放了print、bootset、disk、gdt、等等。
    一个kernel文件夹：
      这个就是我们系统的内核啦，但是它还没有发育起来。

#### 既然出现啦kernel，我们就要说下系统是干嘛的了。
    在本节之前，我们在计算机只跑了一个程序。程序运行开始就代表了系统工作，程序结束，系统也就下班了。
    设想下我们现在有两个需求，一个是在网络上爬取历年各国的gdp数据保存到硬盘上。
    另一个是读取硬盘中的gdp数据分析下2020年那个国家的经济最先崩盘，按照可能性制成一张表，也保存在硬盘上。
    哎，访问网络、数据写入硬盘、读取硬盘数据，这些功能要是都一个一个的写程序，好麻烦。
    对，这是面向过程的编程，作为一个现代程序猿，我们应该面向对象了。
    那么，我们需要对硬件访问做一个封装，让它来为我们的上层应用提供通用的抽象的接口。
    哎呀，就这么轻易的将语言的知识和操作系统结合起来了，我真是一个善于总结的小能手。
    kernel，是一个操作系统的核心。
    它负责管理系统的进程、内存、设备驱动程序、文件和网络系统，后续它应该可以提供给我们中多系统调用方式。
#### 操作系统管理硬件
    操作系统的两个作用：
      防止硬件被失控应用程序滥用
      想应用程序提供简单一致的机制来运行硬件设施
    实现的方法就是：
      将硬件设施和接口抽象几个基本概念：
      进程：系统分配资源的最小单位，进程包括了处理器时间片段，虚拟内存，文件等系统资源
      虚拟内存：主存和文件的抽象（磁盘等一切IO设备都可以认为是文件）
      文件：将各种IO设备抽象为一个统一的概念





#### 第十五节   使用VGA端口
#### 第十六节   视频显示驱动
#### 第十七节   滚动显示支持
以上三节都是想要实现一个看起来非常基础都功能，就是在屏幕上正确显示字符，而不是像之前那样仅仅在屏幕左上角显示一个字母。

这三节，等我总结下代码，再说它的原理。哈哈哈，暂时偷懒

我们

#### 第十八节   滚动显示支持



## 第二部分：在系统上运行程序
-------------


## 第三部分：程序间的交流和通讯
-------------


~~删除线~~ <s>删除线（开启识别HTML标签时）</s>
*斜体字*      _斜体字_
**粗体**  __粗体__
***粗斜体*** ___粗斜体___

上标：X<sub>2</sub>，下标：O<sup>2</sup>

**缩写(同HTML的abbr标签)**

> 即更长的单词或短语的缩写形式，前提是开启识别HTML标签时，已默认开启

The <abbr title="Hyper Text Markup Language">HTML</abbr> specification is maintained by the <abbr title="World Wide Web Consortium">W3C</abbr>.


### 引用 Blockquotes

> 引用文本 Blockquotes

引用的行内混合 Blockquotes

> 引用：如果想要插入空白换行`即<br />标签`，在插入处先键入两个以上的空格然后回车即可，[普通链接](http://localhost/)。

### 锚点与链接 Links

[普通链接](http://localhost/)

[普通链接带标题](http://localhost/ "普通链接带标题")

直接链接：<https://github.com>

[锚点链接][anchor-id]

[anchor-id]: http://www.this-anchor-link.com/

GFM a-tail link @pandao

> @pandao

### 多语言代码高亮 Codes

#### 行内代码 Inline code

执行命令：`npm install marked`

#### 缩进风格



#### c++代码　

```C++
function test(){
}
```


### 图片 Images

Image:

![](htts://pndao.gitub.io/edior.md/examples/images/4.j)


----

### 列表 Lists

#### 无序列表（减号）Unordered Lists (-)

- 列表一


#### 无序列表（星号）Unordered Lists (*)

* 列表一


#### 无序列表（加号和嵌套）Unordered Lists (+)

+ 列表二
    + 列表二-1
    + 列表二-2
    + 列表二-3


#### 有序列表 Ordered Lists (-)

1. 第一行
2. 第二行
3. 第三行

#### GFM task list

- [x] GFM task list 1
- [ ] GFM task list 3
    - [ ] GFM task list 3-1
    - [ ] GFM task list 3-2
    - [ ] GFM task list 3-3


----

### 绘制表格 Tables

| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |



:smiley:

:star:
ctrl + shift+m
