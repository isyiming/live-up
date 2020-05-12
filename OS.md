# 走一遍os-tutorial
### 跟：https://github.com/cfenollosa/os-tutorial 把操作系统从源码走一遍
### 配合《深入理解计算机系统》食用
- 要想对计算机系统有一个全面的认识，有什么方法比得上自己写一遍更好呢？但是让我真的自己写，哈哈哈
- 好在早就有了好多开源代码，那就先跟着走一遍
- 在这篇文章中，我会记录下自己跑代码时遇到的问题，以及解决方法
- 另外，会配合CSAPP将涉及到的理论基础总结提炼下来
- 希望会对看到的同学有些许帮助  
-------------

## 第一部分：程序结构和执行

#### 第零节：安装虚拟机环境 qemu和nasm
    macos下的安装方法，在终端中执行指令：brew install qemu nasm
    qemu就是一个虚拟机，nasm是一个为可移植性与模块化而设计的一个80x86的汇编器。
    作者贴心的提示了别用xcode里的nasm，因为它在我们的这个教程中大部分情况下都不能正常使用，哎它不顶用啊。

    那我就要提示下了，你用homebrew安装的nasm肯定是在/usr/local/Cellar文件夹里，
    这是homebrew的包安装路径/usr/local/bin里的nasm就是Cellar里的nasm的快捷方式。
    而xcode里的nasm就是系统安装的软件了，它肯定是在usr/bin里。
    当然这个bin里好多也都是快捷方式，看它的原身就可以知道一般都在Frameworks这个系统文件夹里。
    好的，当我写完这两句唠叨的时候，我的qemu和nasm也安装完了。

#### 第一节：为系统启动创建引导程序
先了解两个概念：assembler和BIOS

    assembler：汇编程序。
    BIOS：这个在电脑启动的时候有时候会遇到，那个蓝屏的菜单。
    人家的全名是"Basic Input Output System"，是计算机主板上的一段程序。
    主要负责在系统启动前：
      加电检查电脑硬件是否正常，初始化设备参数。
      从启动磁盘的开始扇区加载引导程序启动操作系统。
      为系统提供IO设备操作支持，硬件中断处理。

本节的目标就是：为系统启动创建引导程序

    当计算机启动时，BIOS不知道如何加载操作系统，因此它将该任务委托给启动扇区。
    因此，引导扇区必须放置在已知的标准位置。
    该位置是磁盘的第一个扇区（柱面0，磁头0，扇区0），占用512字节。
    为了确保“磁盘是可引导的”，BIOS检查所称引导扇区的511个字节的0h，并且在第512字节写入0xAA55。

那么我们来运行一下作者给我们的程序吧！

    首先cd到你的工程文件夹。
    比如我就是在文件夹：os-tutorial/01-bootsector-barebones

    编译汇编程序：nasm -f bin boot_sect_simple.asm -o boot_sect_simple.bin
    这样可以在01-bootsector-barebones/下看到一个二进制文件boot_sect_simple.bin
    接下来在qwmu上运行这个程序吧！

macos catalina用户的问题

    如果你是macos用户，并且系统更新到了catalina，恭喜你中奖了！
    https://stackoverflow.com/questions/58318920/qemu-not-responding-after-upgrading-to-macos-catalina
    brew为我们安装的qemu4.1版本的图形化显示方式和catalina不兼容。
    预计在即将推出的qemu4.2版本中会解决这个问题。
    上面那个连接里提到了一个解决方案，不要用brew安装的qemu，自己编译一遍。
    按照他的编译设置，qemu的界面依赖的是SDL，这个也许能解决吧。
    但是我选择关闭图形化显示～就加一个-nographic的选项。
    所以执行指令是这样的：qemu-system-x86_64 -nographic boot_sect_simple.bin
    可以看到 "Booting from Hard Disk...".

    或者用-curses,就是一个很简陋的在终端上实现的图形化界面。

    实际上如果仅仅在qemu跑一个成熟的系统的化，我们也可以通过远程桌面的方式来显示，比如VNC和SPICE。但是这个教程中我们是从头开始，你可以理解为qemu目前的状态是一个没有系统的电脑主机，因为catalina的bug，我们连显示器都没法用了。现在是2020年5月12号，可能过两年qemu就会解决这个问题了。

退出qemu

    先control+a，然后释放这两个键，再按下x，就可以退出qemu了。
-------------
#### 第二节：在引导扇区打印hello

程序就是bit流
上下文
#### 第三节
    nasm -f bin boot_sect_memory_org.asm -o boot_sect_memory_org.bin
    nasm -f bin boot_sect_memory.asm -o boot_sect_memory.bin

    qemu-system-x86_64 -curses boot_sect_memory_org.bin
    qemu-system-x86_64 -curses boot_sect_memory.bin

#### 第四节
    nasm -f bin boot_sect_stack.asm -o boot_sect_stack.bin

    qemu-system-x86_64 -curses boot_sect_stack.bin


#### 第七节
    nasm -f bin boot_sect_main.asm -o boot_sect_main.bin
    qemu-system-x86_64 -curses boot_sect_main.bin

#### 第八节
    nasm -f bin 32bit-print.asm -o 32bit-print.bin
    qemu-system-x86_64 -curses 32bit-print.bin

#### 第九节
    nasm -f bin 32bit-gdt.asm -o 32bit-gdt.bin
    qemu-system-x86_64 -curses 32bit-gdt.bin

#### 第十节
    nasm -f bin 32bit-main.asm -o 32bit-main.bin
    qemu-system-x86_64 -curses 32bit-main.bin

运行后的结果是：

    Booting from Hard Disk...
    0xDADA
    0xFACE


#### 第十一节  安装C语言编译器
    从C语言开始，那么首先安装一个C语言编译器吧，就用gcc,作者提供的方式是下载源码自己编译源文件.
    https://github.com/cfenollosa/os-tutorial/tree/master/11-kernel-crosscompiler

    其实可以用brew安装的
    brew tap nativeos/i386-elf-toolchain
    brew install i386-elf-binutils i386-elf-gcc
    好了我们把gcc编译器和配套工具集binutils安装完毕。
#### 第十二节   编译运行下C代码
    开始编写内核，从这部分开始，我们来到了CSAPP的第二部分，要通过
    这一节作者提供了几个非常简单的小栗子，我们编译一下试试看
    编译：
      i386-elf-gcc -ffreestanding -c function.c -o function.o
      这条指令的意思是，指定gcc编译器，将function.c这段c代码，转化成了可重定位目标文件function.o。但是这段代码没有main函数，所以要以ffreestanding形式编译。也就是说这段代码最后其实是没办法执行的，因为没有函数入口。
      这其中还包含三个过程：
        -首先C语言预处理器将源代码中的#命令（包含文件和宏定义）全部替换为对应的代码段。
        -然后编译器将生成源文件function.c的汇编代码function.s
        -接下来汇编器将汇编代码转换为function.o
        -可以利用反汇编器查看汇编代码：i386-elf-objdump -d function.o
    链接：
      i386-elf-ld -o function.bin -Ttext 0x0 --oformat binary function.o
      这样就得到了可执行的二进制文件function.bin

1.我们可以借此回忆一下程序的运行过程。

    源代码->
      预处理：处理全部包含文件和宏定义代码段，得到.i文件
      编译：词法分析，语义分析，代码优化，得到.s文件
      汇编：得到汇编文件.o
      链接：将源代码用到的库和各种依赖整合在一起，最终才能生成一个完整的可执行文件。

2.链接器的作用

    在开发中非常重要，但是我们在应用层面上很少去了解它的工作。他的作用是将开发人员用到的代码和数据片段整合在一起，
    链接可以执行于编译时：
      静态链接库，我们的项目中用到的各种lib文件就是静态链接方式加入到项目中的。
      它的缺点就是，我们需要的库目标文件在项目中每出现一次，就会生成一个.o文件。
      这样加载到内存代码段的代码就变多了。但是静态链接方式的程序运行速度快。
    也可以执行于加载时：
    也可以执行于运行时：
      dll动态链接，项目中依赖的库函数，在运行时，才将库函数和开发代码（机器码）链接起来。
      这样运行速度慢了一些，可是项目的代码段小了很多。


#### 第十三节   创建一个简单的内核和一个能够引导它的引导程序

i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o

nasm kernel_entry.asm -f elf -o kernel_entry.o

i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

nasm bootsect.asm -f bin -o bootsect.bin
Â
cat bootsect.bin kernel.bin > os-image.bin

qemu-system-i386 -curses -fda os-image.bin

本节咱们还要补充一些额外的知识。关于makefile文件的功能和作用

    如果你用过cmake，或者从源码安装过一些软件，肯定见到过makefile
    第一次用cmake已经是好几年前的事情啦，这个时候回忆往事真的感慨万千。好怀念本科的日子。
    那时候对cmake.. 和make这两个命令都不知道是干嘛的。
    就知道执行完就有了bin文件了，所以这两个命令打的飞快，感觉一秒钟就两条命令输入完成了哈哈。

makefile的作用

    上面我们在由源文件生成bin文件的时候，需要手动逐个文件编译。
    要是仅仅一个文件还好，要是在main函数中调用了好多个子函数呢。
    每一个源文件编译实在是太麻烦啦！
    所以项目管理工具make应运而生了，要是下载一个包的源代码，通常在文件夹里都会有一个makefile，make就是按照这个文件来编译链接源代码，生成可执行文件。然后我们再执行make install，就安装到了电脑到默认路径下。
    可见makefile就是make的药方，makefile告诉了make各个源文件之间的依赖关系。

camkelist

    你要是用过cmake，就一定记得camkelist，在cmakelist里配置项目依赖的库和生成文件路径什么的，就很简洁，虽然还是不如IDE方便。可是已经比makefile容易太多啦。
    cmake就是一个跨平台的项目管理工具，他把make的工作进一步抽象，cmake命令就是将cmakelist文件翻译成makefile的格式，所以camkelist相比makefile可读性也更高啦！
好了，我们cd到第十三节这个文件夹下，里面有一个makefile，是作者写好到，我们运行下make指令，是不是编译运行都一下全搞定了呢？
对了，要是你和我一样是是macos catalina，记得修改下makefile，加上-curses这个选项哈

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

既然出现啦kernel，我们就要说下系统是干嘛的了。

在本节之前，我们在计算机只跑了一个程序。程序运行开始就代表了系统工作，程序结束，系统也就下班了。设想下我们现在有两个需求，一个是在网络上爬取历年各国的gdp数据保存到硬盘上。另一个是读取硬盘中的gdp数据分析下2020年那个国家的经济最先崩盘，按照可能性制成一张表，也保存在硬盘上。

哎，访问网络、数据写入硬盘、读取硬盘数据，这些功能要是都一个一个的写程序，好麻烦。对，这是面向过程的编程，作为一个现代程序猿，我们应该面向对象了。那么，我们需要对硬件访问做一个封装，让它来为我们的上层应用提供通用的抽象的接口。哎呀，就这么轻易的将语言的知识和操作系统结合起来了，我真是一个善于总结的小能手。

kernel，是一个操作系统的核心。它负责管理系统的进程、内存、设备驱动程序、文件和网络系统，后续它应该可以提供给我们中多系统调用方式。 


#### 第十五节   创建一个简单的内核和一个能够引导它的引导程序



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


预格式化文本：

    | First Header  | Second Header |
    | ------------- | ------------- |
    | Content Cell  | Content Cell  |
    | Content Cell  | Content Cell  |

#### c++代码　

```C++
function test(){
}
```


### 图片 Images

Image:

![](https://pandao.github.io/editor.md/examples/images/4.jpg)


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
