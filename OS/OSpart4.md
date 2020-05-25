## 第四部分：实现伪动态内存分配malloc
-------------


#### 第二十二节 malloc

我们首先分析下工程的文件吧

    boot/
      这个一般不会变了
    kernel/
      kernel.c  main()
      util.c util.h
          int_to_ascii()和memory_copy()
          memory_set()功能是将指定内存空间填入指定的值。
          reverse() 翻转数组
          strlen() 返回数组长度
    cpu/
        types.h 定义了一些类似于 unsigned int之类的变量类型
        isr.c 触发中断后，isr_handler将消息和中断号输出到屏幕上
        isr.h中定义了中断向量表，一共有32个中断向量
        interrupt.asm是真正执行触发中断后的中断服务函数的
        idt.h中定义了中断向量的数据结构
        timer.h init_timer cpu计时器初始化函数，会调用中断服务函数register_interrupt_handler，定时中断触发后就执行timer_callback打印当前的系统时钟。
    drivers/
      ports和screen是屏幕的驱动
      keyboard 键盘的驱动 实际上这个键盘的驱动还是有点问题，shift+输出的特殊字符都没办法正常显示，比如一个%，会显示成?5。但是我还是想先不管他了。
    libc/
      mem.c 和 mem.h 终于迎来了内存管理

memory allocation，动态内存分配。申请一块连续的指定大小的内存块区域。从功能描述上来看，malloc应该可以对碎片化的内存块进行整理。多个进程工作的时候，如果之前开辟的进程注销了，那就产生了内存碎片。当剩下的内存空间中没有一块足够大的连续空间容纳新申请的内存块时，就需要碎片整理了。

但是...

这个教程的meory仅仅是给你返回一个以4kb为步长递增的地址...

说实话着实鸡肋。但是还是要说下源码，因为以后要用到。

这个函数唯一值得说的就是对齐操作了。对齐操作，这也是为什么我们平时写代码的时候，声明的变量并不是可以从任意地址开始的，某些bit你永远也不会取到。如果我们的程序中有些是依赖高低字节的，那么对齐就非常有必要了。而且更重要的是，CPU读取内存的时候是粒度读取的，不可以随意的读取某一bit。假如读取主存的第1024～1027这三个bit。就好麻烦了。具体的等我再回顾这里的知识的时候再总结吧。

```c++
uint32_t free_mem_addr = 0x10000;//这个就是作者设定的起始地址，实际上应该是内核代码段的结束位置。我现在不确定0x10000是不是内核代码结束的位置。但是肯定是一个没有被使用的地址。

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    //if (align == 1 && (free_mem_addr & 0xFFFFF000)) {//作者的代码，有错误
      if (align == 1 && (free_mem_addr & 0x00000FFF)) {//这里对齐应该是0x00000FFF
        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr){
        *phys_addr = free_mem_addr;
    }

    uint32_t ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}

```

## 好了，我们的os-tutorial之旅到此就结束了
#### 我们从系统的启动开始，学习了硬件如何寻址，如何通过程序驱动外设，通过中断实现了系统时钟和被动触发外设的驱动。
#### 最终实现了一个简单的shell，它可以对我们的键盘输入作出反馈。通过屏幕-键盘的交互，我们终于看到了一个简单的系统内核是如何开始工作的。
