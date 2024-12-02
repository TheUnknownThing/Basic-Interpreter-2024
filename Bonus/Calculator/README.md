# Basic2024Bonus-计算器

## 题目描述

要求用 `Antlr` 实现正整数四则运算的功能，即加减乘除，意在让大家熟悉 `Antlr` 的使用。

## 配置 Antlr 运行环境

完成该 Bonus 需要用到的 `Antlr` 运行时库是 `lib/antlr4-runtime_4.13.1_amd64.deb`. 
打开文件所在目录，执行以下命令，该库会被安装到`/usr/include`：

```shell
sudo apt install ./antlr4-runtime_4.13.1_amd64.deb
```

这个包含有 `Antlr 4.13.1` 的动态链接库、静态链接库以及头文件，如果不装这个包，
将导致你的程序在编译时找不到 Antlr 的头文件和动态链接库，从而编译失败。

使用 Archlinux 的同学可以直接使用以下命令安装运行环境:
```shell
pacman -S antlr4-runtime
```

使用 macOS 的同学可以直接使用以下命令安装运行环境:
```shell
brew install antlr4-cpp-runtime
```

## 项目结构

```
Calculator
├── src
│   ├── Expr.g4
│   ├── Expr.interp
│   ├── Expr.tokens
│   ├── ExprLexer.cpp
│   ├── ExprLexer.h
│   ├── ExprLexer.interp
│   ├── ExprLexer.tokens
│   ├── ExprParser.cpp
│   ├── ExprParser.h
│   ├── ExprVisitor.cpp
│   └── ExprVisitor.h
├── README.md
├── CMakeLists.txt
├── EvalVisitor.h
└── main.cpp
```

`src` 中的文件全部由 `antlr4` 生成，不需要手动修改，但需要了解其中一些接口的作用。
你需要阅读并了解 [ExprParser.h](src/ExprParser.h) 中各种内嵌类的作用，及其成员变量和接口方法的使用。

`main.cpp` 是程序的入口，我们已经写好了测试代码，也不需要修改。

`EvalVisitor.h` 是计算器类，继承自 `ExprVisitor`，你需要补全其中的方法。

## std::any

需要注意的是，在我们的代码中出现了 `std::any`，可以保存任何类型的值。通过 `std::any_cast` 函数 ，我们可以判断 `any` 是否判断了某一类型的值，并且获取其值。具体如何使用，请参考 [cpprefence](https://en.cppreference.com/w/cpp/utility/any) 。

最基本的用法大致如下:

```C++
std::any x = (int)1;
x = 2;

// 如果存储的是 int 类型，返回指向该值的指针
// 否则返回空指针 nullptr
int *ptr = std::any_cast <int> (&x);

// 如果存储的是 int 类型，返回该值
// 否则抛出异常
int  val = std::any_cast <int>  (x);

// 如果存储的是 int 类型，返回该值的引用
// 否则抛出异常
int &ref = std::any_cast <int&> (x);
```

在本题中，我们保证数字值都是正的 `int` 类型。

## 你需要了解的

下面以 `*Context` 表示所有以 `Context` 结尾的类，例如 `ExprParser::AddSubContext` 等。

- 初步了解表达式树的概念，知道 `*Context` 类的含义；
- 类之间的派生关系，例如各种 `*Context` 类都继承自 `antlr4::ParserRuleContext` 类；
- `visit` 函数的使用，用于访问树上的节点；
```cpp
virtual std::any visit(ParseTree *tree) override {
  return tree->accept(this);
}
```
- `*Context` 每个类及其成员，用于访问子树；
- `ExprParser` 中的枚举类 `enum`，用于判断表达式类型。

## 评测

本 bonus 部分不设 OJ 评测。如果你完成了此项目，请 cr 时向助教展示。加分多少取决于你对代码的理解。
