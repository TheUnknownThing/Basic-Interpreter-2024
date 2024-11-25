### BASIC解释器 2024 项目待办清单

1. **阅读并理解项目需求**
   - **阅读 README.md 文件**：理解整体项目结构和需求。
   - **阅读《Minimal BASIC Interpreter - 2023.pdf》**：详细了解解释器的指令和需求。

2. **理解提供的文件**
   - **Basic/Utils/error.hpp**：了解 C++ 中的错误处理机制。
   - **Basic/Utils/strlib.hpp**：理解提供的字符串库函数。
   - **Basic/Utils/tokenScanner.hpp**：学习如何使用词法分析器解析输入。

3. **了解实现步骤**
   - **阅读 README.md 中关于 `解释器实现步骤及待实现文件介绍` 的部分**：
     - **读取和切分指令**：学习如何使用 `Basic/Utils/tokenScanner` 完成此操作。
     - **指令有效性检查**：了解如何验证指令的有效性。
     - **指令存储**：学习如何使用 `map` 或 `set` 等数据结构存储指令。
     - **指令执行**：了解如何使用 `statement.hpp` 中的类执行指令。

4. **编写和实现代码**
   - **实现文件**：
   `Basic/Basic.cpp` `Basic/program.hpp` `Basic/program.cpp` `Basic/statement.hpp` `Basic/statement.cpp`
   - **Basic/Basic.cpp**：
     - 实现读取和切分指令。
     - 实现指令有效性检查。
     - 阅读：
       - `Basic/Utils/tokenScanner`
   - **Basic/program.hpp** 和 **Basic/program.cpp**：
     - 实现指令存储。
   - **Basic/statement.hpp** 和 **Basic/statement.cpp**：
     - 实现语句的设计和执行。
   - READ: **Basic/parser.hpp**：
     - 实现表达式的正确性检查并解析为表达式树。
   - READ: **Basic/exp.hpp**：
     - 实现表达式的求值。
   - READ: **Basic/evalstate.hpp**：
     - 实现程序执行过程中变量和值的存储。

5. **附加任务**
   - **构建测试点**：
     - 创建边界测试用例，测试其他学生的程序。
   - **实现四则运算计算器**：
     - 完成 `Calculatorm` 文件夹中的附加任务。

### 阅读顺序
1. README.md

2. Minimal BASIC Interpreter - 2023.pdf

3. error.hpp

4. strlib.hpp

5. tokenScanner.hpp

6. parser.hpp

7. exp.hpp

8. evalstate.hpp

### 编写顺序
1. Basic.cpp

2. program.hpp

3. program.cpp

4. statement.hpp

5. statement.cpp

6. parser.hpp

7. exp.hpp

8. evalstate.hpp