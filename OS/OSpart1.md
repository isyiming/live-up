# 走一遍os-tutorial

## 第一部分：系统的启动

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


那么我们来运行一下作者给我们的程序吧！

    首先cd到你的工程文件夹。
    比如我就是在文件夹：os-tutorial/01-bootsector-barebones

    编译汇编程序：nasm -f bin boot_sect_simple.asm -o boot_sect_simple.bin
    这样可以在01-bootsector-barebones/下看到一个二进制文件boot_sect_simple.bin
    接下来在qwmu上运行这个程序吧！
    这个段汇编代码将引导扇区填充了很多个0x00和一个0xAA55.
    随后就进入了死循环。

##### macos catalina用户的问题

    如果你是macos用户，并且系统更新到了catalina，恭喜你中奖了！
    https://stackoverflow.com/questions/58318920/qemu-not-responding-after-upgrading-to-macos-catalina
    brew为我们安装的qemu4.1版本的图形化显示方式和catalina不兼容。
    预计在即将推出的qemu4.2版本中会解决这个问题。
    上面那个连接里提到了一个解决方案，不要用brew安装的qemu，自己编译一遍。
    按照他的编译设置，qemu的界面依赖的是SDL，这个也许能解决吧。
    但是我选择关闭图形化显示～就加一个-nographic 的选项。
    所以执行指令是这样的：qemu-system-x86_64 -nographic boot_sect_simple.bin
    可以看到 "Booting from Hard Disk...".

    或者用-curses,就是一个很简陋的在终端上实现的图形化界面。

    实际上如果仅仅在qemu跑一个成熟的系统的化，我们也可以通过远程桌面的方式来显示，比如VNC和SPICE。但是这个教程中我们是从头开始，你可以理解为qemu目前的状态是一个没有系统的电脑主机，因为catalina的bug，我们连显示器都没法用了。现在是2020年5月12号，可能过两年qemu就会解决这个问题了。

退出qemu

    先control+a，然后释放这两个键，再按下x，就可以退出qemu了。

-------------
#### 第二节：在引导扇区打印hello
#### 第三节
    nasm -f bin boot_sect_memory_org.asm -o boot_sect_memory_org.bin
    nasm -f bin boot_sect_memory.asm -o boot_sect_memory.bin
    qemu-system-x86_64 -curses boot_sect_memory_org.bin
    qemu-system-x86_64 -curses boot_sect_memory.bin
    第四节
    nasm -f bin boot_sect_stack.asm -o boot_sect_stack.bin
    qemu-system-x86_64 -curses boot_sect_stack.bin
    第七节
    nasm -f bin boot_sect_main.asm -o boot_sect_main.bin
    qemu-system-x86_64 -curses boot_sect_main.bin

#### 第七节 让引导扇区从磁盘加载数据

#### 第八节
    nasm -f bin 32bit-print.asm -o 32bit-print.bin
    qemu-system-x86_64 -curses 32bit-print.bin
    第九节
    nasm -f bin 32bit-gdt.asm -o 32bit-gdt.bin
    qemu-system-x86_64 -curses 32bit-gdt.bin
    第十节
    nasm -f bin 32bit-main.asm -o 32bit-main.bin
    qemu-system-x86_64 -curses 32bit-main.bin

以上，我们利用BIOS加载引导程序，引导程序完成了从磁盘读取数据。
注意哦，这个教程里内核代码直接和引导程序放在一起了。

在屏幕上打印字母。但是我们还没有真正构建起我们的系统内核。如果给以上章节一个类比，我觉的现在就相当于我们的电脑还没完成开机。哈哈其实就是这样，BIOS加载了引导扇区的程序后，通常会加载系统程序。但是我们加载的是读取磁盘和打印字母的程序。接下来就开始真正构建我们的内核吧。


------

### 1.基础知识
  计算机中使用0和1二进制，这个原因没得说。我们使用十六进制方便表达和理解二进制。
  字长（word size），指明指针的范围，决定了系统可访问的虚拟内存的地址范围。
  32位系统地址空间是0～2^32-1，正好是4GB。
### 2.i386的16位模式和32位模式
  16位实模式

    8086的地址线有20根，可寻址范围是0～2^20-1，也就是1M字节。但是它的寄存器都是16位，所以只能直接访问2^16个字节，也就是64k。Intel采取了分段寻址的模式： 16位段基地址 :16位偏移 EA。其绝对地址计算方法为： 16位基地址左移 4位 +16位偏移 =20位地址。

  32位保护模式（虚拟地址保护模式）

    GDT全局描述符表：是一个段描述符数组，所有应用程序都可以访问。
    LDT局部描述符表：是一个局部段描述符数组，但是是属于局部的，某一个任务的。
    就和stl中的deque实现方法一样。对应deque的map和缓冲区。
  我们使用32位模式的时候，有了一些变化

    可以访问更大范围大内存地址了，最高可以到4G。
    地址保护模式将内存分割位一个个片段，我们就可以对特殊的内存地址进行保护了。
    比如内核代码所在的地址，我们不希望被用户代码访问。
    还有，GDT+LDT是不是很像虚拟内存的分页功能呢？没错，我们可以更高效的利用内存了。
    还有，因为bios的代码都是在16位实模式下写的，32位模式下内存访问方式变了，所以bios的代码都不能使用了。
    那么就需要我们自己实现对硬件的管理，也就是要重新实现硬件的驱动程序。

### 3.x86的内存地址
    首先，GPU访问一切外设都是根据物理地址（这句话是我说的，暂时感觉没毛病）。
    既然在32为虚拟保护模式下，可以访问4GB内存空间，除了分配给主存还有一些地址要分配给特定的外设。
    比如BIOS所在的ROM0xFFFFFFF0，显示器0xb8000，键盘等。
    但是x86没有直接采用统一端口编址，还有些外设是独立端口编址，要用IN/OUT指令访问。

### 4.处理器运行程序的过程
    尽管我们只是运行了一个非常简单的汇编程序。首先将它编译成二进制可执行文件。
    然后BIOS将它加载到内存中固定的代码段。
    处理器就会不断的执行程序计数器指向的指令，在更新程序计数器，指向下一条指令。
    这个过程中，处理器做的工作非常简单。只有几个操作：
      加载：从内存赋值一个字节到寄存器
      存储：从寄存器赋值一个字节到主存
      操作：把两个寄存器的内容复制到算数逻辑单元ALU进行计算，将结果放到一个寄存器中。
      跳转：从指令中抽取一个字，复制到程序计数器中，覆盖掉原来的值。
    一旦我们开机，有程序在运行，那么cup就会无休止的重复这几个操作。
    操作系统和其他硬件对他来讲都是另一个世界的人。
    它就像一头蒙上了眼睛的驴，无休止的拉磨盘，好可怜～
    看来cpu也是很简单的嘛。但是cpu背后有一套复杂的机制来实现高速运行。


### 5.计算机的启动过程
    主板电压稳定后，主板上控制芯片向CPU发送到reset信号会被撤销，CPU就开始工作了。
    CPU从开始工作，它的工作内容就是固定的：取指令-翻译指令-执行，循环往复。
    它的第一条指令的地址是0xFFFFFFF0也就是BIOS所在的地址。
    BIOS会执行一些硬件检测，最重要的，它要加载操作系统。
    如果你安装过系统，记不记得用优盘安装的时候会更改下启动方式，从优盘启动还是从磁盘启动。这就是选择BIOS加载的引导程序在哪里。
    引导扇区要引导加载内核程序，非常重要。因此，引导扇区必须放置在已知的标准位置。
    该位置是磁盘的第一个扇区（柱面0，磁头0，扇区0），占用512字节。
    放在这里的好处就是好找，就在磁盘开头嘛。但是磁盘不一定在引导扇区存储着引导程序。
    所以我们需要做下标记，告诉BIOS这个磁盘是有bootloader引导程序的。
    BIOS检查引导扇区的最后两个字节是不是0xAA55。如果是，那这个引导扇区就是有效的。随后引导程序被加载到内存的0x7C00地址处，并且跳转过去执行。
