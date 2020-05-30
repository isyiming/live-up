/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */
/* 这是 JamesM 的教程中的链接文件，ming在此添加了记录内核代码段起始位置和内核占用内存结束位置的两个变量：kern_start 和kern_end*/
ENTRY(start)
SECTIONS
{
  . = 0x1000;
  PROVIDE( kern_start = . );
  .text  :
  {
    code = .; _code = .; __code = .;
    *(.text)
    . = ALIGN(4096);
  }

  .data :
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  PROVIDE( kern_end = . );

  end = .; _end = .; __end = .;
}
