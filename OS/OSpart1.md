# 走一遍os-tutorial

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
#### 先了解两个概念：assembler和BIOS

    assembler：汇编程序。
    BIOS：这个在电脑启动的时候有时候会遇到，那个蓝屏的菜单。
    人家的全名是"Basic Input Output System"，是计算机主板上的一段程序。
    主要负责在系统启动前：
      加电检查电脑硬件是否正常，初始化设备参数。
      从启动磁盘的开始扇区加载引导程序启动操作系统。
      为系统提供IO设备操作支持，硬件中断处理。


本节的目标就是：为系统启动创建引导程序

    当计算机启动时，BIOS不知道如何加载操作系统，它将该任务委托给引导扇区。
    因此，引导扇区必须放置在已知的标准位置。该位置是磁盘的第一个扇区（柱面0，磁头0，扇区0），占用512字节。
    放在这里的好处就是好找，就在磁盘开头嘛。但是磁盘不一定在引导扇区存储着引导程序。
    所以我们需要做下标记，告诉BIOS这个磁盘是有引导程序的。
    BIOS检查引导扇区的最后两个字节是不是0xAA55。
    如果是，那这个引导扇区就是有效的。加载它启动系统。

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
    但是我选择关闭图形化显示～就加一个-nographic的选项。
    所以执行指令是这样的：qemu-system-x86_64 -nographic boot_sect_simple.bin
    可以看到 "Booting from Hard Disk...".

    或者用-curses,就是一个很简陋的在终端上实现的图形化界面。

    实际上如果仅仅在qemu跑一个成熟的系统的化，我们也可以通过远程桌面的方式来显示，比如VNC和SPICE。但是这个教程中我们是从头开始，你可以理解为qemu目前的状态是一个没有系统的电脑主机，因为catalina的bug，我们连显示器都没法用了。现在是2020年5月12号，可能过两年qemu就会解决这个问题了。

退出qemu

    先control+a，然后释放这两个键，再按下x，就可以退出qemu了。

### 1.基础知识
    计算机中使用0和1二进制，这个原因没得说。我们使用十六进制方便表达和理解二进制。
    字长（word size），指明指针的范围，决定了系统可访问的虚拟内存的地址范围。
    32位系统地址空间是0～2^32-1，正好是4GB。
### 2.处理器运行程序的过程
    尽管我们只是运行了一个非常简单的汇编程序。首先将它编译成二进制可执行文件。
    然后BIOS将它加载到内存中固定的代码段。
    处理器就会不断的执行程序计数器指向的指令，在更新程序计数器，指向下一条指令。
    这个过程中，处理器做的工作非常简单。只有几个操作：
      加载：从内存赋值一个字节到寄存器
      存储：从寄存器赋值一个字节到主存
      操作：把两个寄存器的内容复制到算数逻辑单元ALU进行计算，将结果放到一个寄存器中。
      跳转：从指令中抽取一个字，复制到程序计数器中，覆盖掉原来的值。
    一旦我们开机，有程序在运行，那么cup就会无休止的重复这几个操作。看来cpu也是很简单的嘛。但是cpu背后有一套复杂的机制来实现高速运行。

-------------
#### 第二节：在引导扇区打印hello

### 程序就是bit流+上下文

    第三节
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
    第八节
    nasm -f bin 32bit-print.asm -o 32bit-print.bin
    qemu-system-x86_64 -curses 32bit-print.bin
    第九节
    nasm -f bin 32bit-gdt.asm -o 32bit-gdt.bin
    qemu-system-x86_64 -curses 32bit-gdt.bin
    第十节
    nasm -f bin 32bit-main.asm -o 32bit-main.bin
    qemu-system-x86_64 -curses 32bit-main.bin

以上，我们利用BIOS完成了从磁盘读取数据，在屏幕上打印字母。但是我们还没有真正构建起我们的系统内核。如果给以上章节一个类比，我觉的现在就相当于我们的电脑还没完成开机。哈哈其实就是这样，BIOS加载了引导扇区的程序后，通常会加载系统程序。但是我们加载的是读取磁盘和打印字母的程序。接下来就开始真正构建我们的内核吧。


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

#### 3.我们可以借此回忆一下程序的运行过程。

    源代码->
      预处理：处理全部包含文件和宏定义代码段，得到.i文件
      编译：词法分析，语义分析，代码优化，得到.s文件
      汇编：得到汇编文件.o
      链接：将源代码用到的库和各种依赖整合在一起，最终才能生成一个完整的可执行文件。

#### 4.链接器的作用

    在开发中非常重要，但是我们在应用层面上很少去了解它的工作。他的作用是将开发人员用到的代码和数据片段整合在一起，
    链接可以执行于编译时：
      静态链接库，我们的项目中用到的各种lib文件就是静态链接方式加入到项目中的。
      它的缺点就是，我们需要的库目标文件在项目中每出现一次，就会生成一个.o文件。
      这样加载到内存代码段的代码就变多了。但是静态链接方式的程序运行速度快。
    也可以执行于加载时：
    也可以执行于运行时：
      dll动态链接，项目中依赖的库函数，在运行时，才将库函数和开发代码（机器码）链接起来。
      这样运行速度慢了一些，可是项目的代码段小了很多。

#### 5.链接器的作用

#### 第十三节   创建一个简单的内核和一个能够引导它的引导程序

    i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o
    nasm kernel_entry.asm -f elf -o kernel_entry.o
    i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
    nasm bootsect.asm -f bin -o bootsect.bin
    cat bootsect.bin kernel.bin > os-image.bin
    qemu-system-i386 -curses -fda os-image.bin

本节咱们还要补充一些额外的知识。关于makefile文件的功能和作用

    如果你用过cmake，或者从源码安装过一些软件，肯定见到过makefile
    第一次用cmake已经是好几年前的事情啦，这个时候回忆往事真的感慨万千。好怀念本科的日子。
    那时候对cmake.. 和make这两个命令都不知道是干嘛的。
    就知道执行完就有了bin文件了，所以这两个命令打的飞快，感觉一秒钟就两条命令输入完成了哈哈。

#### makefile的作用

    上面我们在由源文件生成bin文件的时候，需要手动逐个文件编译。
    要是仅仅一个文件还好，要是在main函数中调用了好多个子函数呢。
    每一个源文件编译实在是太麻烦啦！
    所以项目管理工具make应运而生了，要是下载一个包的源代码，通常在文件夹里都会有一个makefile，make就是按照这个文件来编译链接源代码，生成可执行文件。然后我们再执行make install，就安装到了电脑到默认路径下。
    可见makefile就是make的药方，makefile告诉了make各个源文件之间的依赖关系。

#### camkelist

    你要是用过cmake，就一定记得camkelist，在cmakelist里配置项目依赖的库和生成文件路径什么的，就很简洁，虽然还是不如IDE方便。可是已经比makefile容易太多啦。
    cmake就是一个跨平台的项目管理工具，他把make的工作进一步抽象，cmake命令就是将cmakelist文件翻译成makefile的格式，所以camkelist相比makefile可读性也更高啦！
好了，我们cd到第十三节这个文件夹下，里面有一个makefile，是作者写好到，我们运行下make指令，是不是编译运行都一下全搞定了呢？

对了，要是你和我一样是是macos catalina，记得修改下makefile，加上-curses这个选项哈

还有，makefile中中的gcc和gdb路径记得更改，如果你是和我一样用homebrew安装的话。
