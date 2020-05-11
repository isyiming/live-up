-------------------------------------------------------------------------------------------------------------------
跟：https://github.com/cfenollosa/os-tutorial 把操作系统从源码走一遍

操作系统知识涉及面好多，从基础到复杂，从复杂到复杂。想用诙谐幽默的总结，走完一遍，希望总结完还可以笑得出来。

-------------------------------------------------------------------------------------------------------------------
第零节
安装qemu和nasm： brew install qemu nasm
    这两个东东是啥子腻？
    qemu就是一个虚拟机
    nasm是一个为可移植性与模块化而设计的一个80x86的汇编器
    作者贴心的提示了别用xcode里的nasm，因为它在我们的这个教程中大部分情况下都不能正常使用，哎它不顶用啊。
    那我就要提示下了，你用homebrew安装的nasm肯定是在/usr/local/Cellar文件夹里，这是homebrew的包安装路径/usr/local/bin里的nasm就是Cellar里的nasm的快捷方式。
    而xcode里的nasm就是系统安装的软件了，它肯定是在usr/bin里，当然这个bin里好多也都是快捷方式，看它的原身就可以知道一般都在Frameworks这个系统文件夹里。
    好的，当我写完这两句唠叨的时候，我的brew install qemu nasm指令还没执行完，那么我就好好看看
-------------------------------------------------------------------------------------------------------------------
第一节
先了解两个概念：
    assembler 汇编程序。
    BIOS 这个在电脑启动的时候有时候会遇到，那个蓝屏的菜单。人家的全名是"Basic Input Output System"，是计算机主板上的一段程序。
    主要负责在系统启动前：加电检查电脑硬件是否正常，初始化设备参数，从启动磁盘的开始扇区加载引导程序启动操作系统，为系统提供IO设备操作支持，硬件中断处理。
    本节的目标就是：为系统启动创建引导程序
    当计算机启动时，BIOS不知道如何加载操作系统，因此它将该任务委托给启动扇区。因此，引导扇区必须放置在已知的标准位置。该位置是磁盘的第一个扇区（柱面0，磁头0，扇区0），占用512字节。
    为了确保“磁盘是可引导的”，BIOS检查所称引导扇区的511个字节的0h，并且在第512字节写入0xAA55。
那么我们来运行一下作者给我们的程序吧。
首先cd到你的工程文件夹，比如我就是在文件夹：os-tutorial/01-bootsector-barebones
编译汇编程序：nasm -f bin boot_sect_simple.asm -o boot_sect_simple.bin
这样可以在01-bootsector-barebones/下看到一个二进制文件boot_sect_simple.bin
接下来在qwmu上运行这个程序吧！
如果你是macos用户，并且系统更新到了catalina，恭喜你中奖了！
https://stackoverflow.com/questions/58318920/qemu-not-responding-after-upgrading-to-macos-catalina
brew为我们安装的qemu4.1版本的图形化显示方式和catalina不兼容，预计在即将推出的qemu4.2版本中会解决这个问题。上面那个连接里提到了一个解决方案，不要用brew安装的qemu，自己编译一遍，按照他的编译设置，qemu的界面依赖的是SDL，这个也许能解决吧。
但是我选择关闭图形化显示～就加一个-nographic的选项。
所以执行指令是这样的：qemu-system-x86_64 -nographic boot_sect_simple.bin
You will see a window open which says "Booting from Hard Disk..." and nothing else. 
When was the last time you were so excited to see an infinite loop? ;-)
对的，就是这样的，那么问题来了，咋退出呀。。。
先control+a，然后释放这两个键，再按下x，就可以退出qemu了。
-------------------------------------------------------------------------------------------------------------------
第二节
要在引导扇区打印hello

-------------------------------------------------------------------------------------------------------------------
第三节

nasm -f bin boot_sect_memory_org.asm -o boot_sect_memory_org.bin
nasm -f bin boot_sect_memory.asm -o boot_sect_memory.bin

qemu-system-x86_64 -nographic boot_sect_memory_org.bin
qemu-system-x86_64 -nographic boot_sect_memory.bin

