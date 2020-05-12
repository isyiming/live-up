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

### 第零节：安装虚拟机环境 qemu和nasm
    macos下的安装方法，在终端中执行指令：brew install qemu nasm
    qemu就是一个虚拟机，nasm是一个为可移植性与模块化而设计的一个80x86的汇编器。
    作者贴心的提示了别用xcode里的nasm，因为它在我们的这个教程中大部分情况下都不能正常使用，哎它不顶用啊。
    那我就要提示下了，你用homebrew安装的nasm肯定是在/usr/local/Cellar文件夹里，
    这是homebrew的包安装路径/usr/local/bin里的nasm就是Cellar里的nasm的快捷方式。
    而xcode里的nasm就是系统安装的软件了，它肯定是在usr/bin里。
    当然这个bin里好多也都是快捷方式，看它的原身就可以知道一般都在Frameworks这个系统文件夹里。
    好的，当我写完这两句唠叨的时候，我的qemu和nasm也安装完了。

### 第一节
    先了解两个概念：
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

    那么我们来运行一下作者给我们的程序吧。首先cd到你的工程文件夹。
    比如我就是在文件夹：os-tutorial/01-bootsector-barebones

    编译汇编程序：nasm -f bin boot_sect_simple.asm -o boot_sect_simple.bin
    这样可以在01-bootsector-barebones/下看到一个二进制文件boot_sect_simple.bin

    接下来在qwmu上运行这个程序吧！如果你是macos用户，并且系统更新到了catalina，恭喜你中奖了！
    https://stackoverflow.com/questions/58318920/qemu-not-responding-after-upgrading-to-macos-catalina
    brew为我们安装的qemu4.1版本的图形化显示方式和catalina不兼容。
    预计在即将推出的qemu4.2版本中会解决这个问题。
    上面那个连接里提到了一个解决方案，不要用brew安装的qemu，自己编译一遍，按照他的编译设置，qemu的界面依赖的是SDL，这个也许能解决吧。
    但是我选择关闭图形化显示～就加一个-nographic的选项。
    所以执行指令是这样的：qemu-system-x86_64 -nographic boot_sect_simple.bin
    You will see a window open which says "Booting from Hard Disk..." and nothing else.
    When was the last time you were so excited to see an infinite loop? ;-)
    对的，就是这样的，那么问题来了，咋退出呀。。。
    先control+a，然后释放这两个键，再按下x，就可以退出qemu了。



    -------------------------------------------------------------------------------------------------------------------

    -------------------------------------------------------------------------------------------------------------------
    第二节
    要在引导扇区打印hello

    -------------------------------------------------------------------------------------------------------------------
    第三节

    nasm -f bin boot_sect_memory_org.asm -o boot_sect_memory_org.bin
    nasm -f bin boot_sect_memory.asm -o boot_sect_memory.bin

    qemu-system-x86_64 -nographic boot_sect_memory_org.bin
    qemu-system-x86_64 -nographic boot_sect_memory.bin

    -------------------------------------------------------------------------------------------------------------------
    第四节
    nasm -f bin boot_sect_stack.asm -o boot_sect_stack.bin

    qemu-system-x86_64 -nographic boot_sect_stack.bin
    -------------------------------------------------------------------------------------------------------------------
    第十一节
    开始编写内核
    从C语言开始，那么首先安装一个C语言编译器吧，就用gcc

    https://github.com/cfenollosa/os-tutorial/tree/master/11-kernel-crosscompiler

    作者提供的方式是下载源码自己编译源文件
    其实可以用brew安装的

    brew tap nativeos/i386-elf-toolchain
    brew install i386-elf-binutils i386-elf-gcc

    好了我们把gcc编译器和配套工具集binutils安装完毕。

    -------------------------------------------------------------------------------------------------------------------
    第十二节
    开始编写内核，

    这一节作者提供了几个非常简单的小栗子，我们编译一下试试看

    i386-elf-gcc -ffreestanding -c function.c -o function.o
    将function.c编译链接后生成了二进制可执行文件function.o
第二部分：在系统上运行程序
-------------

第三部分：程序间的交流和通讯
-------------

### 字符效果和横线等

----

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

即缩进四个空格，也做为实现类似`<pre>`预格式化文本(Preformatted Text)的功能。

    <?php
        echo "Hello world!";
    ?>

预格式化文本：

    | First Header  | Second Header |
    | ------------- | ------------- |
    | Content Cell  | Content Cell  |
    | Content Cell  | Content Cell  |

#### JS代码　

```javascript
function test(){
	console.log("Hello world!");
}

(function(){
    var box = function(){
        return box.fn.init();
    };

    box.prototype = box.fn = {
        init : function(){
            console.log('box.init()');

			return this;
        },

		add : function(str){
			alert("add", str);

			return this;
		},

		remove : function(str){
			alert("remove", str);

			return this;
		}
    };

    box.fn.init.prototype = box.fn;

    window.box =box;
})();

var testBox = box();
testBox.add("jQuery").remove("jQuery");
```

#### HTML代码 HTML codes

```html
<!DOCTYPE html>
<html>
    <head>
        <mate charest="utf-8" />
        <title>Hello world!</title>
    </head>
    <body>
        <h1>Hello world!</h1>
    </body>
</html>
```

### 图片 Images

Image:

![](https://pandao.github.io/editor.md/examples/images/4.jpg)

> Follow your heart.

![](https://pandao.github.io/editor.md/examples/images/8.jpg)

> 图为：厦门白城沙滩

图片加链接 (Image + Link)：

[![](https://pandao.github.io/editor.md/examples/images/7.jpg)](https://pandao.github.io/editor.md/examples/images/7.jpg "李健首张专辑《似水流年》封面")

> 图为：李健首张专辑《似水流年》封面

----

### 列表 Lists

#### 无序列表（减号）Unordered Lists (-)

- 列表一
- 列表二
- 列表三

#### 无序列表（星号）Unordered Lists (*)

* 列表一
* 列表二
* 列表三

#### 无序列表（加号和嵌套）Unordered Lists (+)

+ 列表一
+ 列表二
    + 列表二-1
    + 列表二-2
    + 列表二-3
+ 列表三
    * 列表一
    * 列表二
    * 列表三

#### 有序列表 Ordered Lists (-)

1. 第一行
2. 第二行
3. 第三行

#### GFM task list

- [x] GFM task list 1
- [x] GFM task list 2
- [ ] GFM task list 3
    - [ ] GFM task list 3-1
    - [ ] GFM task list 3-2
    - [ ] GFM task list 3-3
- [ ] GFM task list 4
    - [ ] GFM task list 4-1
    - [ ] GFM task list 4-2

----

### 绘制表格 Tables

| 项目        | 价格   |  数量  |
| --------   | -----:  | :----:  |
| 计算机      | $1600   |   5     |
| 手机        |   $12   |   12   |
| 管线        |    $1    |  234  |

First Header  | Second Header
------------- | -------------
Content Cell  | Content Cell
Content Cell  | Content Cell

| First Header  | Second Header |
| ------------- | ------------- |
| Content Cell  | Content Cell  |
| Content Cell  | Content Cell  |

| Function name | Description                    |
| ------------- | ------------------------------ |
| `help()`      | Display the help window.       |
| `destroy()`   | **Destroy your computer!**     |

| Left-Aligned  | Center Aligned  | Right Aligned |
| :------------ |:---------------:| -----:|
| col 3 is      | some wordy text | $1600 |
| col 2 is      | centered        |   $12 |
| zebra stripes | are neat        |    $1 |

| Item      | Value |
| --------- | -----:|
| Computer  | $1600 |
| Phone     |   $12 |
| Pipe      |    $1 |

----

#### 特殊符号 HTML Entities Codes

&copy; &  &uml; &trade; &iexcl; &pound;
&amp; &lt; &gt; &yen; &euro; &reg; &plusmn; &para; &sect; &brvbar; &macr; &laquo; &middot;

X&sup2; Y&sup3; &frac34; &frac14;  &times;  &divide;   &raquo;

18&ordm;C  &quot;  &apos;

### Emoji表情 :smiley:

> Blockquotes :star:

#### GFM task lists & Emoji & fontAwesome icon emoji & editormd logo emoji :editormd-logo-5x:

- [x] :smiley: @mentions, :smiley: #refs, [links](), **formatting**, and <del>tags</del> supported :editormd-logo:;
- [x] list syntax required (any unordered or ordered list supported) :editormd-logo-3x:;
- [x] [ ] :smiley: this is a complete item :smiley:;
- [ ] []this is an incomplete item [test link](#) :fa-star: @pandao;
- [ ] [ ]this is an incomplete item :fa-star: :fa-gear:;
    - [ ] :smiley: this is an incomplete item [test link](#) :fa-star: :fa-gear:;
    - [ ] :smiley: this is  :fa-star: :fa-gear: an incomplete item [test link](#);

#### 反斜杠 Escape

\*literal asterisks\*

### 科学公式 TeX(KaTeX)

$$E=mc^2$$

行内的公式$$E=mc^2$$行内的公式，行内的$$E=mc^2$$公式。

$$\(\sqrt{3x-1}+(1+x)^2\)$$

$$\sin(\alpha)^{\theta}=\sum_{i=0}^{n}(x^i + \cos(f))$$

多行公式：

```math
\displaystyle
\left( \sum\_{k=1}^n a\_k b\_k \right)^2
\leq
\left( \sum\_{k=1}^n a\_k^2 \right)
\left( \sum\_{k=1}^n b\_k^2 \right)
```

```katex
\displaystyle
    \frac{1}{
        \Bigl(\sqrt{\phi \sqrt{5}}-\phi\Bigr) e^{
        \frac25 \pi}} = 1+\frac{e^{-2\pi}} {1+\frac{e^{-4\pi}} {
        1+\frac{e^{-6\pi}}
        {1+\frac{e^{-8\pi}}
         {1+\cdots} }
        }
    }
```

```latex
f(x) = \int_{-\infty}^\infty
    \hat f(\xi)\,e^{2 \pi i \xi x}
    \,d\xi
```

### 绘制流程图 Flowchart

```flow
st=>start: 用户登陆
op=>operation: 登陆操作
cond=>condition: 登陆成功 Yes or No?
e=>end: 进入后台

st->op->cond
cond(yes)->e
cond(no)->op
```

### 绘制序列图 Sequence Diagram

```seq
Andrew->China: Says Hello
Note right of China: China thinks\nabout it
China-->Andrew: How are you?
Andrew->>China: I am good thanks!
```

### End
