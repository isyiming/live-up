
作为一个C++开发者，还是要掌握makefile呀。实际上我是拜读了这篇博客：
https://blog.csdn.net/haoel/article/details/2886

https://seisman.github.io/how-to-write-makefile/overview.html

要感谢 陈皓 大神精彩的文章。这篇笔记完全是提炼以上博客中的关键点，没有任何原创内容。

### makefile的两个作用：

1. 编译

  源代码.c编译为目标文件.o
  检查语法是否正确，函数和变量的声明是否正确

2. 链接

  将目标文件.o拼合成可执行文件
  链接函数和全局变量
  当中间目标文件.o太多时，链接极不方便，所以打包成.a(win下的.lib)

3. 从实例来学习makefile语法

```Makefile
    edit : main.o kbd.o command.o display.o /
               insert.o search.o files.o utils.o
                cc -o edit main.o kbd.o command.o display.o /
                           insert.o search.o files.o utils.o

    main.o : main.c defs.h
            cc -c main.c
    kbd.o : kbd.c defs.h command.h
            cc -c kbd.c
    command.o : command.c defs.h command.h
            cc -c command.c
    display.o : display.c defs.h buffer.h
            cc -c display.c
    insert.o : insert.c defs.h buffer.h
            cc -c insert.c
    search.o : search.c defs.h buffer.h
            cc -c search.c
    files.o : files.c defs.h buffer.h command.h
            cc -c files.c
    utils.o : utils.c defs.h
            cc -c utils.c
    clean :
            rm edit main.o kbd.o command.o display.o /
               insert.o search.o files.o utils.o
```
以上这个实例中，
目标文件（target）包含：执行文件edit和中间目标文件（* .o），
依赖文件（prerequisites）就是冒号后面的那些 .c 文件和 .h文件。
每一个 .o 文件都有一组依赖文件，而这些 .o 文件又是执行文件 edit 的依赖文件。
依赖关系的实质上就是说明了目标文件是由哪些文件生成的，换言之，目标文件是哪些文件更新的。

4. 可以用变量来代替需要重复添加的文件名
```Makefile
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
edit : $(objects)
    cc -o edit $(objects)
main.o : main.c defs.h
    cc -c main.c
kbd.o : kbd.c defs.h command.h
    cc -c kbd.c
command.o : command.c defs.h command.h
    cc -c command.c
display.o : display.c defs.h buffer.h
    cc -c display.c
insert.o : insert.c defs.h buffer.h
    cc -c insert.c
search.o : search.c defs.h buffer.h
    cc -c search.c
files.o : files.c defs.h buffer.h command.h
    cc -c files.c
utils.o : utils.c defs.h
    cc -c utils.c
clean :
    rm edit $(objects)
```
我们定义了一个变量objects，这样就不用繁琐地每次都再edit那里手动添加两次.o文件名了。

5. make的自动推导使得makefile文件更加简介
```Makefile
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
edit : $(objects)
    cc -o edit $(objects)
main.o : defs.h
kbd.o : defs.h command.h
command.o : defs.h command.h
display.o : defs.h buffer.h
insert.o : defs.h buffer.h
search.o : defs.h buffer.h
files.o : defs.h buffer.h command.h
utils.o : defs.h
.PHONY : clean
clean :
    rm edit $(objects)
```
由于make的自动推导，只要make看到一个.o文件，就会自动把对应的.c文件添加到依赖中。于是我们只需要在每一个.o文件后添加依赖的.h文件就可以了。

6. make自动推导的进一步精简
```Makefile
objects = main.o kbd.o command.o display.o \
    insert.o search.o files.o utils.o
edit : $(objects)
    cc -o edit $(objects)
$(objects) : defs.h
kbd.o command.o files.o : command.h
display.o insert.o search.o files.o : buffer.h

.PHONY : clean
clean :
    rm edit $(objects)
```


makefile $@, $^, $<, $?
$@  表示目标文件
$^  表示所有的依赖文件
$<  表示第一个依赖文件
$?  表示比目标还要新的依赖文件列表
