## 第二部分：第一个外设：屏幕


#### 第十五节   使用VGA端口
#### 第十六节   视频显示驱动
#### 第十七节   滚动显示支持

5.14日伤心日，昨晚写的这里的竟然还没提交就没了。应该是我不小心复制文件的时候覆盖掉了。

以上三节都是想要实现一个看起来非常基础都功能，就是在屏幕上正确显示字符，而不是像之前那样仅仅在屏幕左上角显示一个字母。回顾一下系统的概念，系统是对硬件的抽象。它将为上层的应用提供统一的接口。这三节就是这个抽象的过程。我们是要对屏幕进行操作，我们要将内存中的数据发送给显示设备，这就是我们要实现的第一种I/O。

实际上系统要做的工作也没有太复杂，因为屏幕这个硬件的具体显示我们不需要管。只需要将要显示的数据不断的发送给设备控制器。但是CPU的运行速度远远快于显示设备的处理速度。如果我们想播放一段视频，cpu将一帧数据，cpu可以很快将这件事做完。但是这个时候显示设备还没处理完这些数据，那么cpu就需要等待。宝贵的CPU时间段都用来等待未免有些浪费了。所以就有了DMA，专门处理这种高速和低速设备之间的数据传输问题，把CPU解放出来。不过这个教程中没有使用DMA。

这部分的主要内容就是驱动屏幕外设，那么它是怎么样的？屏幕外设：

    屏幕外设包括显卡和显示器两个部分。
    屏幕负责显示显存的数据，具体他是怎么显示的和我们的系统还有cpu无关啦。
    我们只需要将想要显示的内容给显存就可以了。
    显卡有两种工作模式：
        一种是文本工作模式，比如我们现在使用的模式，或者Dos系统。
        还有就是图形模式，现在的大部分系统都是这种模式。

文本模式的屏幕驱动方式：

    文本模式下的显卡，在x86系统中是统一编址的，0xB8000~0xBFFFF这个地址段便是映射到文本模式的显存的。
    当访问这些地址的时候，实际上读写的是显存区域，而显卡会周期性的读取这里的数据，并且把它们按顺序显示在屏幕上。
    你可以看到屏幕的驱动程序screen.c,里面显示文本的函数如print_char，都是从0xB8000这个地址开始写入数据。

显卡的其他端口控制：

    我们要想实现完整的屏幕驱动工作，除了访问显存，还有一些与显示控制相关的寄存器需要访问。
    这些寄存器没有直接被统一编址，而是通过两个地址端口来控制。
    CPU访问0x3D4这个地址存入想要访问的显卡寄存器地址，再通过0x3D5端口来设置相应寄存器的值。
    之所以这样做，是因为显卡的寄存器非常多有300多个，而访问它的频率又没有那么频繁，相对于需要直接访问的主存来讲。所以给这部分寄存器独立端口编址是很合适的。
    这部分功能的实现就在ports.c和ports.h里。
    你可以看到设置光标位置的函数set_cursor_offset，实际上访问的是0x3D4的第14号和第15号寄存器。

当我们知道了以上两点后，就会发现原来驱动屏幕其实也很简单，顺理成章的事情。
比如我们想要在某个位置显示一个字符，那就计算好这个位置的显存地址（我的意思就是0xB8000~0xBFFFF）后，将字符存进去就好了。如此来看我们还可以用*或者-等特殊符号画出来一个简陋的像素风格的系统启动界面哈哈。

那么来看下drivers/screen.h吧
```C++
#define VIDEO_ADDRESS 0xb8000 //定义了显示器端口地址
#define MAX_ROWS 25 //行数
#define MAX_COLS 80//列数
#define WHITE_ON_BLACK 0x0f//默认颜色方案
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

/* Public kernel API */
void clear_screen();
void kprint_at(char *message, int col, int row);
void kprint(char *message);

```
三个函数分别是清屏、在特定的位置显示一个字符串、直接显示一个字符串。

这三个函数的基础是，如何在屏幕上显示一个字符print_char()。

    视频存储器是bios程序在系统启动之间，就将其地址写入到设备控制器中了。
    视频存储器的每个字符第一个字节是字符的ASCII码，第二个字符是我们的配色方案。
    我们想要做的就是不断的将想要显示的字符或图案数据发送给视频存储器。
    例如，想要在第3行第4列显示一个字符。
    那就需要将视频存储器的第488((3*80+4) * 2)个存储单元的，
    第一个字节填入想要显示的字符的ASCII码，第二个字节填入想要的配色方案。

那么清屏：

    就显示满屏幕的空白字符就好了。

屏幕滚动的实现：

    如果显示的时候已经到最后一行了，就需要滚动显示。
    就将视频存储器的某一行数据复制到上一行的地址空间就好了。
    这里作者写了一个函数memory_copy在util.c中。
    这个可能会在其他地方也使用到，所以这个函数就放在kernel文件夹下啦。
    clear_screen()就是调用的memory_copy。

我们再来看下项目的文件结构吧

    boot/
      这个文件夹下放了print、bootset、disk、gdt、等等。任何和引导相关的文件都放在这里，
    kernel/
      util.c util.h 提供了int_to_ascii()和memory_copy()
      kernel.c  main()
    drivers/
      ports和screen是屏幕的驱动
      在进入32位模式下工作后，bios就完全不能用了，而且本来bios支持的硬件也很少。
      所以要有新的驱动程序，在系统启动后能够正常驱动硬件。

现在看来我们的内核已经可以做些事情了，我们完成了一个最基本的I/O的抽象。现在我们的系统上任何新的程序想要在屏幕上显示字符的话，只需要kprint_at或者kprint就好啦。

显然这个系统还是太简陋了，而且我们知道，Linux系统的一个基本特征就是，设备将被抽象为文件I/O。磁盘，鼠标，键盘，屏幕等都可以用一个统一的接口来调用。

而且，我们的系统还不具备异常情况的处理，比如说如果突然显示器和主机断开连接了，我们没办法访问视频存储器了；有时候出现了除数为0的情况。现在我们的系统，可能出现一个错误，系统就挂掉了，这显然不是我们想要的。

另外，处理键盘的输入，现有的内核代码会怎样处理呢？我们只能在程序中写一个死循环，不断的检测键盘是否有按键按下，然后输出字符。这样显然效率太低了。cpu除了这件事什么都做不了了。
所以，要考虑到各种异常情况，在有可能的情况下，尽量保证系统出现异常时能给出解决方法，不要出点情况就崩溃。

---------

#### 1.既然出现啦kernel，我们就要说下系统是干嘛的了。
    在本节之前，我们在计算机只跑了一个程序。程序运行开始就代表了系统工作，程序结束，系统也就下班了。
    设想下我们现在有两个需求，一个是在网络上爬取历年各国的gdp数据保存到硬盘上。
    另一个是读取硬盘中的gdp数据分析下2020年那个国家的经济最先崩盘，按照可能性制成一张表，也保存在硬盘上。
    哎，访问网络、数据写入硬盘、读取硬盘数据，这些功能要是都一个一个的写程序，好麻烦。
    对，这是面向过程的编程，作为一个现代程序猿，我们应该面向对象了。
    那么，我们需要对硬件访问做一个封装，让它来为我们的上层应用提供通用的抽象的接口。
    哎呀，就这么轻易的将语言的知识和操作系统结合起来了，我真是一个善于总结的小能手。
    kernel，是一个操作系统的核心。
    它负责管理系统的进程、内存、设备驱动程序、文件和网络系统，后续它应该可以提供给我们中多系统调用方式。

#### 2.操作系统管理硬件
    操作系统的两个作用：
      防止硬件被失控应用程序滥用
      想应用程序提供简单一致的机制来运行硬件设施
    实现的方法就是：
      将硬件设施和接口抽象几个基本概念：
      进程：系统分配资源的最小单位，进程包括了处理器时间片段，虚拟内存，文件等系统资源
      虚拟内存：主存和文件的抽象（磁盘等一切IO设备都可以认为是文件）
      文件：将各种IO设备抽象为一个统一的概念

#### 3.设备控制器
#### 4.dma
