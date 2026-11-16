# Compilers Principles

> Author: Jay Wei (GitHub: [JayWei-git](https://github.com/JayWei-git) | Email: jaywei0610@163.com) — Please let me know if you spot any errors.
>
> (Last Updated: 2026-06-02)

### 0. The Structure of a Compiler

##### Front End

前端的主要任务是分析源代码，确保符合规范，并将其转化为编译器能理解的内部数据结构；该阶段通常与源语言紧密相关，而与运行的机器硬件无关；

- 词法分析 (Lexical Analysis / Scanning)： 将字符流(源代码)切割成一个个记号(Tokens)，如关键字、变量名、操作符等，并过滤掉空格和注释；
- 语法分析 (Syntax Analysis / Parsing)： 根据语言的语法规则，将 Tokens 组合成一棵抽象语法树(AST，Abstract Syntax Tree)，并检查代码是否存在语法错误 (如少括号)；
- 语义分析 (Semantic Analysis)： 检查代码是否有逻辑错误，比如类型是否匹配，变量是否未定义就使用等；

---

##### Middle End

前端通过后，代码会被转化为一种承上启下的标准格式——中间表示 (IR，Intermediate Representation) (如 LLVM IR)；中端的核心工作就是对 IR 进行优化：

- 独立于机器的优化： 寻找更高效的等价代码，而不需要考虑具体的硬件芯片；
- 如删除永不执行的死代码、把循环里不变量提到循环外、合并常数等；

---

##### Back End

后端的任务是合成；它接收优化后的 IR，并将其翻译成目标机器的汇编语言或二进制机器码；这个阶段与目标硬件架构 ( x86, ARM, RISC-V) 强绑定：

- 指令选择 (Instruction Selection)： 将 IR 操作映射到目标芯片的具体机器指令上；
- 寄存器分配 (Register Allocation)： 决定哪些数据放在芯片极其有限的“高速缓存”寄存器里，哪些放内存，这是影响运行效率的关键；
- 目标代码优化与生成： 针对特定硬件的流水线、缓存特性进行最后的指令重排，最终输出可执行文件；

---

**End of 0.**



## Part One: Front End

### 1. Lexical Analysis

- 词素 (Lexeme)：源代码中具有独立含义的最小字符序列；
- 词法单元 (Token)：词法分析器输出的语法标签，二元组 `<TokenName, AttributeValue>`；
- 模式 (Pattern)：描述某一类词素生成的规则 (通常用正则表达式)；

#### 1.1. Input Buffering

> 如何



## Part Two: Middle End

## Part Three: Back End