# 介绍 Intro

*Warriors The New Prophecy Quest Game* Flash游戏，经过反编译后的C++重新实现，主要用来**加速**。原游戏现已被移除，但在[archive](https://web.archive.org/web/20161121090738/http://www.warriorcats.com/games-and-extras/games/the-new-prophecy-quest)上仍能找到。仅供测试，版权为原游戏所有。

Decompiled from *Warriors The New Prophecy Quest Game* Flash version, reimplemented in C++ with the purpose of accelerating playing. The original game has been removed, but you can find it on [archive](https://web.archive.org/web/20161121090738/http://www.warriorcats.com/games-and-extras/games/the-new-prophecy-quest). Test purpose only. I don't own it. 

# 构建 Build

大概用支持C++11的编译器随便就可以编译吧，当然我用的是GCC。如果你不会编译或有问题，请到[Releases](https://github.com/zhzh2001/tnpquest/releases)下载二进制文件。

Compilers with C++11 support will probably work, and GCC is recommended.

# 更新

在 OOP 大程中复用了本项目，不过基本上完全重写了，更加符合 OOP 规范。由于原始提交存在大量个人信息，因此不提供。旧版代码位于 `src.old` 中存档。项目代码基本上都是我写的，部分名词中文翻译、一些 bug 的发现和很多文档由另一位组员完成。介绍详见文档。

新版代码采用 CMake，使用现代 C++ 编译器按照标准方式（`mkdir build && cd build && cmake .. && make`）即可构建。

项目依赖 ncurses，类 UNIX 请自行安装 `libncurses6`，Windows 请自行编译安装 `pdcurses`。详细支持信息参见第二月的文档。
