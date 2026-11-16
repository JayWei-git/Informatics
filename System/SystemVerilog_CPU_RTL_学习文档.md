# 面向 CPU RTL 开发的 SystemVerilog 学习文档

## 各章节学习目标、前置知识与预计时间

| 章节 | 学习目标 | 前置知识 | 预计时间 |
|---|---|---|---:|
| 第一部分 | 建立“代码描述硬件、模块并发存在、时钟边沿更新状态”的核心模型。 | 已具备的组合/时序逻辑基础。 | 2–3 小时 |
| 第二部分 | 正确读懂信号声明、数组、常量、位操作、表达式尺寸和 signed 规则。 | 第一部分。 | 5–7 小时 |
| 第三部分 | 独立阅读并编写 CPU 中的 MUX、ALU、译码器、扩展器等组合逻辑。 | 第二部分。 | 4–6 小时 |
| 第四部分 | 正确理解和修改 PC、寄存器堆、流水线寄存器等时序逻辑。 | 第二、三部分。 | 5–7 小时 |
| 第五部分 | 阅读多模块 CPU 工程，理解 package、参数、结构体和 generate。 | 前四部分。 | 4–5 小时 |
| 第六部分 | 将语法映射到完整 MIPS32 单周期 CPU，并为五级流水线做准备。 | 前五部分及 MIPS32 数据通路基础。 | 8–12 小时 |
| 第七部分 | 编写和阅读 Testbench，使用 Verilator、Icarus、GTKWave 定位问题。 | 前六部分。 | 5–7 小时 |
| 第八部分 | 系统审查 AI 生成 RTL，定位常见语言、时序、位宽和验证问题。 | 全部前置章节。 | 5–8 小时 |

建议总学习投入：约 **38–55 小时**。第二、四、六、七、八部分最值得反复结合实际代码练习。

## 知识路线图

```text
数字逻辑与 MIPS 数据通路基础（已具备）
                │
                ▼
硬件描述模型：module、并发、RTL、综合、仿真
                │
                ▼
信号与类型：logic/wire、位宽、signed、数组、常量
                │
        ┌───────┴────────┐
        ▼                ▼
组合逻辑             时序逻辑
assign/always_comb   always_ff/<=/reset/enable
if/case/MUX/ALU      PC/寄存器堆/流水线寄存器
        └───────┬────────┘
                ▼
模块化：实例化、parameter、package、enum、struct、generate
                │
                ▼
单周期 MIPS CPU：取指→译码→执行→访存→写回
                │
                ▼
验证：Testbench、断言、内存初始化、波形、delta cycle
                │
                ▼
调试与审查：lint 警告、位宽、时序、锁存器、竞争、可综合性
                │
                ▼
五级流水线：流水线寄存器、stall、flush、forwarding、hazard
```

## CPU RTL 开发的最小必备语法清单

### 必须掌握

| 范畴 | 内容 |
|---|---|
| 模块 | `module`、端口声明、命名端口实例化、层次连接 |
| 信号 | `logic`、`wire`、单驱动原则、4-state 值 |
| 常量 | `parameter`、`localparam`、尺寸化常量 |
| 位操作 | 位选择、部分选择、拼接、复制拼接、符号扩展、零扩展 |
| 类型 | packed/unpacked array、`signed`/`unsigned`、显式转换 |
| 组合逻辑 | `assign`、`always_comb`、`if`、`case`、`casez`、默认赋值 |
| 时序逻辑 | `always_ff`、`posedge`、复位、时钟使能、非阻塞赋值 `<=` |
| CPU 模块 | PC、寄存器堆、ALU、译码、立即数扩展、指令/数据存储器、MUX |
| 验证 | `initial`、时钟生成、`$display`、`$fatal`、`$readmemh`、VCD 波形 |
| 调试 | Verilator warning/error、位宽检查、锁存器识别、时序与采样竞争 |

### 能够读懂即可

| 范畴 | 内容 |
|---|---|
| 类型组织 | `typedef`、`typedef enum`、`typedef struct packed` |
| 工程组织 | `package`、`import`、``include`` |
| 参数化 | 类型参数、`$clog2`、参数覆盖 |
| 生成结构 | `generate`、`genvar`、条件生成 |
| 验证增强 | immediate assertion、`$monitor`、`$strobe` |
| 常用语言结构 | `for` 循环、`function automatic`、`unique case`、`priority case` |
| 存储器模型 | 组合读/同步读、数组初始化、读写冲突语义 |

### 暂时不必学习

| 范畴 | 原因 |
|---|---|
| class、随机化、约束随机 | 属于高级验证方法学，不是 CPU RTL 阅读的前置条件。 |
| UVM | 对项目长期验证有价值，但应在能写稳定 RTL/Testbench 后学习。 |
| covergroup、覆盖率驱动验证 | 可后置到功能正确性稳定之后。 |
| clocking block、program block | 能帮助复杂 TB 避免竞争，但初期先掌握基础采样时序。 |
| interface、modport | 有助于大型工程接口管理，但不是当前单周期 CPU 的最小需求。 |
| DPI-C | C/C++ 协同仿真主题，可在后续需要参考模型时再学。 |
| SVA 并发断言 | 对流水线形式化/高级验证重要，但可先学习 immediate assertion。 |
| 复杂队列、动态数组、关联数组 | 多用于 Testbench，不是基础 CPU RTL 的核心。 |

## 何时可以开始独立阅读 AI 生成的 CPU RTL

完成以下内容后，就可以开始阅读结构较清晰的单周期 CPU RTL：

- 第一部分：知道模块并发、组合路径和状态更新的基本模型。
- 第二部分的 2.2、2.3、2.4、2.5、2.6：能看懂声明、端口、位切片、拼接、数组和 signed/位宽问题。
- 第三部分：能识别 MUX、译码器、ALU，以及锁存器风险。
- 第四部分的 4.1、4.2、4.5：能理解 PC、寄存器堆和时钟沿更新。
- 第五部分的 5.1、5.3、5.4：能跨文件追踪模块、控制码和流水线数据结构。

这时你应能开始逐模块阅读 AI 生成的单周期 CPU，并能可靠回答：信号来自哪里、何时更新、位宽是否正确、组合路径是否完整、控制信号是否覆盖全部指令。

完成第六部分后，应能整体审查单周期 MIPS32 CPU；完成第七、八部分后，才具备较稳固的能力去修改 RTL、写测试，并依据 Verilator 诊断和 GTKWave 波形调试问题。

---

# 第一部分：SystemVerilog 的硬件描述模型

## 本章目标

完成本章后，你应能：

- 把一段 RTL 看成**在描述电路结构和信号关系**，而不是在描述一个从上到下运行的程序；
- 区分组合逻辑、状态元件和模块边界；
- 解释单周期 CPU 中一条指令在一个时钟周期内经过哪些组合路径、在何处由时钟沿保存状态；
- 初步区分可综合 RTL 和只服务于仿真的 Testbench 语句；
- 阅读模块端口和简单模块实例化，并从代码反推基本硬件。

**前置知识：**组合逻辑、D 触发器、寄存器、同步时序模型，以及 MIPS 数据通路基础。  
**预计学习时间：**2–3 小时。

> 本章刻意不深入 `logic`、`wire`、`always_comb` 和 `always_ff` 的完整语法细节；它们分别在第二、三、四部分严格展开。这里先建立正确的硬件模型。

## 1.1 从“程序”到“硬件描述”

### 1.1.1 SystemVerilog 不是顺序执行的普通编程语言

最重要的规则是：**一个可综合的 RTL 模块描述的是一块始终存在的硬件，而不是一次调用后才开始执行的函数。**

例如，下面的代码可理解为一个 32 位加法器及其输入、输出连线：

```systemverilog
module pc_plus4 (
    input  logic [31:0] pc,
    output logic [31:0] pc_next
);
    assign pc_next = pc + 32'd4;
endmodule
```

逐行理解：

1. `module pc_plus4` 声明一个名为 `pc_plus4` 的硬件模块类型。
2. `pc` 是输入端口；外部电路驱动它。
3. `pc_next` 是输出端口；本模块驱动它。
4. `assign pc_next = ...` 表示连续驱动：只要 `pc` 的值变化，`pc_next` 就会重新反映 `pc + 4` 的结果。
5. `endmodule` 结束模块定义。

它不是“先读取 `pc`，再执行一次加法，然后结束”。加法器在抽象硬件模型中一直存在。若 `pc` 从 `32'h0000_1000` 变为 `32'h0000_1004`，则在传播延迟的抽象之后，`pc_next` 对应变为 `32'h0000_1008`。

**硬件映射：**一个 32 位组合加法器；没有触发器，没有记忆能力。断电或输入不确定时，它不会“记住上次结果”。

**仿真语义：**仿真器不会真的搭建门电路；它在 `pc` 改变的事件之后计算右侧表达式，并更新 `pc_next`。同一个仿真时间点内还可能经历若干个 delta cycle，详见第七部分。

#### 与 C/C++ 的关键差异

在 C 中：

```cpp
pc_next = pc + 4;
```

是一条在某个控制流位置执行一次的语句。在 RTL 中，上例的 `assign` 表达的是永久存在的逻辑关系。SystemVerilog 的过程块中确实存在**过程内语句顺序**，但过程块之间默认并发，且综合结果仍是硬件，而不是软件控制流。

下面是一个常见的错误思维：

```systemverilog
// 错误理解：以为先“执行 ALU”，然后“执行寄存器堆”。
alu_result = rs_data + rt_data;
rd_data    = alu_result;
```

正确理解是：若这些语句位于正确的组合过程块中，它们描述两段组合逻辑的依赖关系。`rd_data` 的硬件最终依赖 `rs_data`、`rt_data`，并不代表 CPU 内部有一个软件解释器按两行代码逐条运行。

**CPU 应用：**指令译码、立即数扩展、ALU、分支目标地址和写回数据选择，通常都是同一周期内并发工作的组合网络。

**小练习 1：**若 `pc` 为 `32'h0040_0000`，`pc_plus4` 的输出是多少？若随后 `pc` 改为 `32'h0040_0004`，该模块需要等待下一个时钟沿才改变输出吗？说明原因。

### 1.1.2 并发硬件与过程块内的顺序语句

一个模块中可以有多个 `assign`、多个组合过程块和多个时序过程块。它们代表不同硬件单元，概念上同时工作。

```systemverilog
module execute_fragment (
    input  logic [31:0] rs_data,
    input  logic [31:0] rt_data,
    input  logic        alu_src_imm,
    input  logic [31:0] imm_ext,
    output logic [31:0] alu_b,
    output logic [31:0] alu_result
);
    assign alu_b      = alu_src_imm ? imm_ext : rt_data;
    assign alu_result = rs_data + alu_b;
endmodule
```

这里有两条并发的连续赋值：

- 第一条描述一个 32 位二选一多路选择器（MUX）。
- 第二条描述一个加法器。

当 `alu_src_imm`、`imm_ext` 或 `rt_data` 任一输入变化时，`alu_b` 会重新计算；当 `rs_data` 或 `alu_b` 变化时，`alu_result` 会重新计算。信号依赖关系是：

```text
rt_data ───────┐
               ├─ MUX ── alu_b ──┐
imm_ext ───────┘                 ├─ ADD ── alu_result
alu_src_imm ──────────────────────┘
rs_data ──────────────────────────┘
```

虽然源文件把 MUX 的语句写在加法器前面，但硬件并不存在“先完成 MUX 的软件语句，再开始执行加法器”的全局顺序。真实组合电路中，输入变化会沿依赖关系传播；仿真器则通过事件调度得到稳定结果。

过程块内部不同。请先只观察语义，完整语法后续再讲：

```systemverilog
always_comb begin
    alu_b      = alu_src_imm ? imm_ext : rt_data;
    alu_result = rs_data + alu_b;
end
```

在**一次进入这个过程块**时，阻塞赋值 `=` 使第一行结束后，第二行能读取本次计算出的 `alu_b`。这种顺序是为了描述组合逻辑的中间计算，最终硬件仍对应 MUX 加法器网络。

不要据此推导：“两个不同 `always_comb` 块也按源码先后顺序运行。”不同过程块的执行顺序不应成为设计正确性的前提。

```systemverilog
// 不应依赖两个并发过程块的源文件排列顺序。
always_comb alu_b      = alu_src_imm ? imm_ext : rt_data;
always_comb alu_result = rs_data + alu_b;
```

这段代码在通常情况下仍可描述同一组合网络，但在复杂设计中更容易因遗漏敏感关系、多个驱动或临时变量使用而难以推理。对于紧密相关的组合计算，后续建议采用明确的“输入 → 默认值 → 分支覆盖 → 输出”组织方式。

**常见错误：**把不同过程块当作线程并试图用源代码位置规定执行先后；或让同一个 `logic` 被两个过程块赋值。后者常导致编译器报多驱动错误，或使综合硬件不明确。

**CPU 应用：**一个单周期 CPU 的“控制单元”“立即数扩展器”“ALU”“下一 PC 选择器”可以是独立模块或独立组合块，它们在同一周期并发计算。

**小练习 2：**对于 `addi $t0, $t1, 4`，`alu_src_imm` 应为 0 还是 1？请沿图追踪 `alu_b` 和 `alu_result` 的来源。

### 1.1.3 信号、状态与时间

CPU 的硬件可先分为两类：

| 类别 | 是否保存过去的值 | 常见 RTL 描述 | CPU 示例 |
|---|---|---|---|
| 组合逻辑 | 否 | `assign`、`always_comb` | ALU、译码器、MUX、`PC + 4` |
| 时序逻辑 | 是 | `always_ff @(posedge clk)` | PC 寄存器、寄存器堆写端口、流水线寄存器 |

组合逻辑满足“输出是当前输入的函数”。时序逻辑除了当前输入，还依赖先前保存的状态。例如 PC 必须保存“当前正在取的指令地址”，因此它是寄存器。

```systemverilog
module pc_register (
    input  logic        clk,
    input  logic        rst_n,
    input  logic        pc_enable,
    input  logic [31:0] pc_d,
    output logic [31:0] pc_q
);
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            pc_q <= 32'b0;
        else if (pc_enable)
            pc_q <= pc_d;
    end
endmodule
```

此处采用低有效异步复位：

- `rst_n` 变为 `0` 时，不等待时钟沿，`pc_q` 被安排复位为 0；
- 正常运行时，每个 `posedge clk`，若 `pc_enable == 1`，PC 才接收 `pc_d`；
- 当 `pc_enable == 0` 时没有赋值，含义是寄存器保持原值，而不是组合逻辑遗漏赋值。

**硬件映射：**32 个 D 触发器构成的 32 位寄存器，加上复位和使能控制。`pc_q` 是当前状态；`pc_d` 是组合逻辑计算出的下一状态。

**执行与仿真语义：**在一个上升沿到来时，所有由该时钟触发的时序块都会从旧状态采样输入；其中的 `<=` 不会立刻改变左值，而会在本时间步的 NBA 更新阶段一起生效。因此，寄存器之间能正确地“同时更新”。第四部分会给出完整调度时间线。

**小练习 3：**PC 当前为 `100`，上升沿前 `pc_d = 104`，`pc_enable = 1`。该上升沿后 `pc_q` 是多少？如果同一时刻另一个寄存器读取 `pc_q` 并使用非阻塞赋值更新，它读到的是 100 还是 104？

## 1.2 module：硬件模块的边界

### 1.2.1 模块定义、端口与实例

**语法定义：**`module ... endmodule` 定义一种可重复实例化的硬件模块。模块定义本身不会制造一份“正在运行的对象”；在顶层或其他模块中实例化后，才形成实际层次结构中的一个硬件实例。

```systemverilog
module mux2 #(
    parameter int WIDTH = 32
) (
    input  logic [WIDTH-1:0] a,
    input  logic [WIDTH-1:0] b,
    input  logic             sel,
    output logic [WIDTH-1:0] y
);
    assign y = sel ? b : a;
endmodule

module writeback_path (
    input  logic [31:0] alu_result,
    input  logic [31:0] mem_read_data,
    input  logic        mem_to_reg,
    output logic [31:0] wb_data
);
    mux2 #(.WIDTH(32)) u_wb_mux (
        .a   (alu_result),
        .b   (mem_read_data),
        .sel (mem_to_reg),
        .y   (wb_data)
    );
endmodule
```

逐行解释：

1. `mux2` 是模块类型名；`WIDTH` 是参数，默认值为 32。
2. `a`、`b`、`y` 的位宽由 `WIDTH` 决定；`sel` 是选择信号。
3. `u_wb_mux` 是 `mux2` 的一个**实例名**，类似原理图中某个具体元件的标号。
4. `#(.WIDTH(32))` 覆盖该实例的参数。此处虽与默认值相同，但明确展示了参数连接语法。
5. `.a(alu_result)` 是命名端口连接：被实例模块的 `a` 端口连接到父模块中的 `alu_result` 信号。

**硬件映射：**`writeback_path` 内部放置一个 32 位二选一 MUX 实例。对于 `lw`，`mem_to_reg=1`，写回数据来自数据存储器；对于算术指令，写回数据来自 ALU。

**仿真语义：**父模块与子模块不是函数调用关系。`u_wb_mux` 和父模块的其他逻辑同时存在；端口连接就是模块边界上的信号连接。

**常见错误：位置端口连接。**

```systemverilog
// 可合法编译，但顺序一旦记错，硬件连线就错。
mux2 u_wb_mux(alu_result, mem_read_data, mem_to_reg, wb_data);
```

初学阶段和 CPU 顶层连接中应优先使用命名端口连接。它能显著降低端口顺序改动造成的静默错误，也使波形和代码审查更清晰。

**CPU 应用：**顶层 CPU 通过实例化 `pc_register`、`regfile`、`decoder`、`alu`、`imem`、`dmem` 和若干 MUX 构成数据通路。

**小练习 4：**若把实例连接误写为 `.sel(mem_read_data[0])`，代码通常仍可编译。它会对 `lw` 的写回路径造成什么硬件错误？为什么 lint 不一定能判断这违反了你的 CPU 设计意图？

### 1.2.2 input、output、inout 的方向语义

端口方向描述模块边界处的驱动方向：

- `input`：信号由模块外部驱动，模块内部读取；
- `output`：信号由模块内部驱动，模块外部读取；
- `inout`：双向网络端口，通常用于芯片引脚、三态总线等场景。

对当前 CPU RTL，绝大多数内部模块只需要 `input` 和 `output`。不要为了“读写方便”把普通端口写成 `inout`；现代片上 CPU 数据通路通常采用明确的单向信号和 MUX，而非内部三态总线。

```systemverilog
module branch_target (
    input  logic [31:0] pc_plus4,
    input  logic [31:0] branch_offset,
    output logic [31:0] target
);
    assign target = pc_plus4 + branch_offset;
endmodule
```

这里 `pc_plus4` 和 `branch_offset` 是从上游组合电路流入的值，`target` 是该模块输出给下一 PC 选择逻辑的值。

**常见错误：**混淆端口方向与数据的“概念流向”。端口方向总是相对于**当前模块**而言。例如顶层的 `imem_addr` 是顶层输出；但在指令存储器子模块看来，这个同一根连线接入的是其输入端口。

**小练习 5：**在顶层 `cpu` 中，`regfile` 的 `write_data` 应连接到顶层信号的输入端还是输出端？请分别从顶层和寄存器堆子模块的视角回答。

### 1.2.3 模块内部信号与模块间连接

内部信号是模块的局部连线或局部状态，不直接暴露给父模块。顶层通常把大量内部信号作为“数据通路网表”的连接点：

```systemverilog
module single_cycle_cpu (
    input logic clk,
    input logic rst_n
);
    logic [31:0] pc_q;
    logic [31:0] pc_d;
    logic [31:0] instruction;
    logic [31:0] rs_data;
    logic [31:0] rt_data;
    logic [31:0] alu_result;

    pc_register u_pc (
        .clk       (clk),
        .rst_n     (rst_n),
        .pc_enable (1'b1),
        .pc_d      (pc_d),
        .pc_q      (pc_q)
    );

    // 省略：指令存储器、译码器、寄存器堆、ALU 和下一 PC 逻辑实例。
endmodule
```

这不是“顶层函数调用 `pc_register` 后获得返回值”。`u_pc` 是一个持续存在的寄存器硬件实例；`pc_q`、`pc_d` 是它与其他硬件之间的连接。

命名习惯上：

- 后缀 `_q` 常表示寄存器当前输出（registered state）；
- 后缀 `_d` 常表示即将写入寄存器的下一状态（next state）；
- `_i` / `_o` 有时分别表示模块输入/输出，但项目必须保持一致。

这些只是编码约定，不是 SystemVerilog 关键字；工具不会因命名正确就保证时序正确。

## 1.3 RTL、仿真与综合

### 1.3.1 RTL 的抽象层次

RTL 是 Register-Transfer Level，意为用“寄存器在时钟边沿间传递数据”以及“寄存器之间的组合逻辑”描述硬件。它关注：

1. 哪些值由寄存器保存；
2. 哪些组合逻辑计算下一状态或输出；
3. 哪些控制信号选择数据路径；
4. 在哪个时钟沿更新状态。

以单周期 MIPS CPU 为例：

```text
PC 寄存器 ─→ 指令存储器 ─→ 译码器/寄存器堆 ─→ ALU ─→ 数据存储器/MUX ─→ 寄存器堆写端口
     ↑                                                                            │
     └────────────────────── 下一 PC 组合逻辑 ────────────────────────────────────┘
```

通常，PC 和通用寄存器堆是状态元件；其余基本数据通路主要为组合逻辑。所谓“单周期”不是说一条指令没有组合传播延迟，而是说在给定的时钟周期内完成取指、译码、执行、访存（如需要）和写回，并在边沿更新有关状态。

### 1.3.2 事件驱动仿真模型

RTL 仿真模拟的是语言定义的事件与调度，而非晶体管级物理过程。粗略过程如下：

1. Testbench 或时钟发生器改变一个输入信号；
2. 依赖该信号的连续赋值和过程块被触发；
3. 组合结果继续触发下游逻辑，直到当前仿真时间点稳定；
4. 在时钟边沿触发的时序块采样旧值，非阻塞赋值在 NBA 阶段共同提交；
5. 更新后的寄存器输出又会驱动下一轮组合逻辑计算。

因此，同一仿真时间，例如 `100 ns`，不一定只有一次“计算”。可能有多个不推进物理仿真时间的 delta cycle。第七部分会严格讨论调度区域、竞争条件和 Testbench 采样。

### 1.3.3 综合如何理解 RTL

综合工具将可识别的 RTL 模式映射成目标工艺库中的硬件：

| RTL 形式 | 典型综合硬件 |
|---|---|
| `assign y = a & b;` | 与门或等价逻辑 |
| `assign y = sel ? b : a;` | 二选一 MUX |
| 组合块中的完整 `if/case` | MUX 与组合门网络 |
| `always_ff @(posedge clk)` | D 触发器或寄存器组 |
| 寄存器数组的规范读写模式 | 触发器阵列，或在特定条件下推断 RAM |

综合并不“执行” `for`、`if` 或 `case`。这些语言结构用于说明硬件连接和选择关系。例如组合块中的 `if (sel)` 通常映射为 MUX；时序块中 `if (enable)` 通常映射为带使能的寄存器实现。

综合工具会优化等价逻辑，但不会自动理解你想实现 MIPS 指令语义。譬如 `slt` 中若误把有符号比较写成无符号比较，得到的仍是一套完全可综合、但功能错误的硬件。

### 1.3.4 可综合 RTL 与 Testbench 代码的边界

**可综合**意味着主流综合工具能够在特定目标和约束下，将代码实现为硬件。它不等于“语言上合法”，也不等于“功能正确”。

通常用于 RTL 的结构包括：模块、静态位向量、组合/时序过程块、固定边界数组、实例化和参数化结构。典型 Testbench 专用结构包括：

```systemverilog
initial begin
    clk = 1'b0;
    forever #5 clk = ~clk;
end

initial begin
    rst_n = 1'b0;
    #12;
    rst_n = 1'b1;
    #1000;
    $finish;
end
```

`initial`、`#5`、`#12`、`$finish` 在这里用于控制仿真时间和结束仿真，通常不能综合为 ASIC/普通 CPU RTL 硬件。它们是验证环境的一部分，而不是被测 CPU 的一部分。

**工具边界：**SystemVerilog 语言标准定义语义；Verilator 和 Icarus Verilog 分别实现其可支持的子集并提供额外诊断。某段代码在一个工具可运行，不自动证明它在另一工具、综合工具或特定 FPGA/ASIC 流程中同样受支持。对 CPU RTL，优先使用清晰、静态、可综合的编码风格，并把仿真延迟和系统任务隔离在 `tb/` 中。

**小练习 6：**为什么时钟生成中的 `forever #5 clk = ~clk;` 适合 Testbench，却不应放进 CPU 的 `rtl/` 顶层模块？

## 1.4 CPU 视角：单周期数据通路的描述方式

### 1.4.1 数据通路、控制通路与状态元件

阅读 CPU RTL 时，可先给每个信号分类：

| 分类 | 典型信号 | 它回答的问题 |
|---|---|---|
| 状态 | `pc_q`、寄存器堆数组、流水线寄存器 | 上一时钟周期保存了什么？ |
| 数据通路 | `instruction`、`rs_data`、`imm_ext`、`alu_result` | 正在处理的操作数或结果是什么？ |
| 控制通路 | `reg_write`、`alu_src_imm`、`mem_write`、`branch` | 数据应经过哪条路径、哪个状态应更新？ |
| 下一状态 | `pc_d`、`if_id_d` | 下一时钟沿应写入什么？ |

例如，对 `add $t0, $t1, $t2`：译码器产生“使用两个寄存器操作数、ALU 执行加法、写回 `rd`、不访存”等控制信号；寄存器堆读出 `$t1` 和 `$t2`；ALU 得到和；下一个时钟沿，和写入 `$t0`。控制信号本身不是“软件的 if 分支结果”，而是实际控制 MUX 选择端、寄存器写使能和存储器写使能的电平。

### 1.4.2 一条指令在组合路径和时钟边沿中的流动

把单周期实现理解为两个相邻上升沿之间的稳定过程：

```text
时钟沿 k 后：PC 寄存器输出 pc_q
      ↓ 组合传播
取指 → 译码 → 读寄存器 → 扩展立即数 → ALU → 访存/写回选择
      ↓
时钟沿 k+1：PC 更新；寄存器堆在需要时写入结果
```

注意两点：

1. “读寄存器堆”在教学用的异步读寄存器堆模型中通常是组合读；写回则一般在时钟沿同步发生。
2. 一条指令的结果在边沿写回后，下一条指令通过寄存器堆读端口才能看到更新后的状态。具体的同沿读写语义和流水线旁路问题将在第四、六部分讨论。

### 1.4.3 从代码反推硬件结构的方法

面对 AI 生成的 RTL，建议按以下顺序而不是按文件文字顺序阅读：

1. **找状态：**搜索 `always_ff`、`posedge`、数组写入；列出 PC、寄存器堆和流水线寄存器。
2. **找下一状态：**追踪写入状态元件的右值，例如 `pc_d`、`wb_data`、`*_d`。
3. **找组合逻辑：**搜索 `assign`、`always_comb`、`if`、`case`，确认输出是否完整赋值。
4. **找控制来源：**从 `reg_write`、`alu_op`、`mem_to_reg` 等信号反向追踪到 opcode/funct 译码。
5. **做位宽核对：**检查地址、立即数、寄存器编号、控制码以及拼接表达式。
6. **最后看顶层连线：**验证每个子模块的输入来自正确的上游，输出到正确的下游。

下面这段是足以阅读的 PC 骨架：

```systemverilog
always_comb begin
    pc_d = pc_q + 32'd4;
    if (take_branch)
        pc_d = branch_target;
    if (jump)
        pc_d = jump_target;
end

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        pc_q <= 32'b0;
    else
        pc_q <= pc_d;
end
```

反推结果：

- `pc_q` 是 32 位状态寄存器；
- `pc_q + 4` 是默认下一 PC 加法器；
- 两个条件赋值形成覆盖默认路径的 MUX 选择网络；
- 由于 `jump` 的赋值写在后面，若 `take_branch` 和 `jump` 意外同时为 1，`jump_target` 在该过程内具有优先级；
- 上述优先级是否符合 ISA 设计，要由控制单元保证或显式断言，而不能只看代码“能编译”。

这就是从语法到硬件、从硬件到时序语义的基本阅读链条。

## 本章总结

- `module` 定义硬件边界；实例化建立实际硬件层次和连线，不是函数调用。
- RTL 的核心是“状态元件 + 计算下一状态和输出的组合逻辑”。
- 模块和不同过程块在概念上并发；过程块内部的语句顺序只在该过程被执行时有意义。
- 组合逻辑的输出随当前输入变化；寄存器在指定时钟边沿采样并保存状态。
- 仿真器按事件和调度区域计算 RTL；综合工具把可综合模式映射为硬件。两者相关但不等同。
- CPU RTL 阅读应先找状态，再追踪下一状态、组合路径、控制来源和顶层连线。

## 核心语法速查表

| 语法/概念 | 本章中的含义 | CPU 例子 |
|---|---|---|
| `module ... endmodule` | 定义可实例化硬件模块 | `alu`、`regfile`、`pc_register` |
| `input` / `output` | 相对当前模块的端口驱动方向 | `alu` 的操作数输入与结果输出 |
| `assign` | 连续描述组合逻辑关系 | `pc_plus4 = pc_q + 4` |
| `always_ff @(posedge clk)` | 描述边沿触发状态更新 | PC、寄存器堆写端口 |
| `<=` | 时序逻辑中安排在 NBA 阶段更新 | `pc_q <= pc_d` |
| `#delay`、`initial` | 通常为 Testbench 仿真控制 | 时钟和复位生成 |
| `_q` / `_d` | 常见状态/下一状态命名约定，非语言规则 | `pc_q` / `pc_d` |

## 自测题

1. 用自己的话区分“模块定义”“模块实例”和“模块端口连接”。
2. 下列各项应属于组合逻辑还是时序逻辑：`PC + 4`、ALU、寄存器堆写端口、立即数扩展、IF/ID 流水线寄存器？
3. 为什么不能依赖两个独立 `always` 块在源文件中的先后顺序？
4. 对 `lw $t0, 8($t1)`，从 `pc_q` 开始列出至少五个组合信号或组合模块，直到形成写回数据。
5. 阅读下面代码，说明它在硬件上可能形成什么，并判断 `sel=1` 时 `y` 的来源：

```systemverilog
assign y = sel ? from_mem : from_alu;
```

6. 指出本章 PC 骨架中 `take_branch` 和 `jump` 同时为 1 时的优先级，并说明应如何在控制逻辑或验证中处理这种组合。

---

# 第二部分：基本语法、信号与数据类型

## 本章目标

完成本章后，你应能正确阅读 CPU RTL 中绝大多数声明和表达式，并能主动检查以下高频问题：端口是否被合适地驱动、寄存器堆数组维度是否正确、指令字段切片是否正确、立即数是否被正确扩展、比较是否误用了 unsigned、结果是否被意外截断。

**前置知识：**第一部分。  
**预计学习时间：**5–7 小时。

## 2.1 标识符、注释、常量与编译单元

### 2.1.1 标识符与命名

SystemVerilog 标识符通常由字母、数字、`_` 和 `$` 构成，不能以数字开头；语言关键字不能作为普通名称。建议在 CPU 工程中使用有硬件含义的名字，而不是只用 `a`、`b`、`tmp`：

```systemverilog
logic [31:0] pc_q;          // 寄存器保存的当前 PC
logic [31:0] pc_d;          // 组合逻辑产生的下一 PC
logic        reg_write_en;  // 寄存器堆写使能
logic [4:0]  rs_idx;        // rs 字段，即寄存器读地址
```

`_q`、`_d`、`_en`、`_idx` 都是团队约定，不是语法。其价值在于让读者无需先读完所有实现，就能初步判断一个信号是状态、下一状态、使能还是索引。

### 2.1.2 注释

```systemverilog
// 单行注释：MIPS $zero 永远不写入。
if (write_en && (write_addr != 5'd0)) begin
    regs[write_addr] <= write_data;  /* 块注释也可以跨行 */
end
```

注释不参与仿真和综合。对 AI 生成代码尤其要警惕：注释说“同步读 RAM”不等于代码真的描述了同步读；以 `always_ff`、`assign`、赋值位置和端口时序为准。

### 2.1.3 编译单元、文件与 `include`

仿真器会根据文件列表编译模块和 package。一个常见结构是：

```text
rtl/
  cpu_pkg.sv          # 公共类型、opcode、ALU 操作码
  pc_register.sv
  regfile.sv
  alu.sv
  decoder.sv
  single_cycle_cpu.sv
tb/
  tb_single_cycle_cpu.sv
```

`include` 是文本替换：预处理器把被包含文件的内容原样插入当前位置。它适合宏定义或少量共享声明，但不应把完整模块在多个地方反复 `include`，否则可能造成重复定义。共享类型和常量更适合放入 `package`，第五部分讨论。

**工具说明：**Verilator 与 Icarus 都要求 package 在使用它的编译单元之前可见；具体文件列表和选项写法不同。不要把“某次恰好能编译”当作跨工具的语言保证。

## 2.2 `logic`、`wire`、`reg`：驱动方式与硬件含义

这是 CPU RTL 初学者最需要校正的概念。

### 2.2.1 直观模型：类型不等于硬件器件

`logic`、`wire` 和旧式 `reg` 都不是“加法器”“触发器”之类的硬件元件名称。是否推断寄存器，主要由**赋值所在的时序过程和赋值覆盖方式**决定，而不是由变量是否叫 `logic` 或 `reg` 决定。

```systemverilog
logic [31:0] sum;
logic [31:0] pc_q;

assign sum = a + b;  // sum 描述组合加法器输出，不是寄存器。

always_ff @(posedge clk) begin
    pc_q <= pc_d;    // pc_q 描述寄存器，即使它的类型是 logic。
end
```

因此，“`logic` 就是逻辑信号”是较安全的直觉；“`logic` 就是硬件寄存器”是错误的。

### 2.2.2 `wire`：网络（net）与连接

**语法定义：**`wire` 声明一种 net。net 的值来自连接到它的驱动源，例如模块输出或连续赋值。它建模的是“这根网络被谁驱动”，自身不是过程赋值保存的变量。

```systemverilog
module alu_link (
    input  logic [31:0] rs_data,
    input  logic [31:0] rt_data,
    output wire  [31:0] sum
);
    assign sum = rs_data + rt_data;
endmodule
```

**逐行解释：**`sum` 是一根 32 位网络；`assign` 在持续驱动该网络。右侧输入发生变化，网络的解析值随之更新。

**硬件映射：**`wire` 的存在本身通常只表示连接；本例的 `+` 产生组合加法器。不能从 `wire` 一词推断有或没有寄存器。

**常见错误：**在过程块中给普通 `wire` 赋值：

```systemverilog
wire [31:0] result;

always_comb begin
    result = rs_data + rt_data; // 错误：过程赋值目标应为变量。
end
```

应改为 `logic [31:0] result;`，或直接保留 `wire` 并使用 `assign`。

### 2.2.3 `logic`：变量（variable）与单驱动编码风格

**语法定义：**`logic` 是 SystemVerilog 的四态变量数据类型，可保存过程赋值的当前值。它可用于组合或时序过程的左值；在现代 RTL 中通常用它替代旧 Verilog 的大部分 `reg`。

```systemverilog
module alu_src_mux (
    input  logic [31:0] rt_data,
    input  logic [31:0] imm_ext,
    input  logic        alu_src_imm,
    output logic [31:0] alu_b
);
    always_comb begin
        alu_b = rt_data;
        if (alu_src_imm)
            alu_b = imm_ext;
    end
endmodule
```

**执行语义：**`alu_b` 是过程变量。`always_comb` 对输入变化重新执行，且每次执行都先写默认值，因此 `alu_b` 始终由当前输入决定；综合结果为 MUX，而非锁存器。

在 SystemVerilog 项目中，常见且清晰的约定是：

- 模块内部与输出端口，若由一个 `always_comb` 或 `always_ff` 驱动，使用 `logic`；
- 明确表示模块间连续连线、或需要网络解析语义时，使用 `wire`；
- 每个普通 RTL 信号坚持一个驱动源。

不少现代工具也接受由单个连续赋值或单个模块输出驱动的 `logic`。但从代码审查角度，`wire` 更能传达“这是网络/连续连接”，`logic` 更能传达“这是过程变量”。遵从项目既有风格比机械替换更重要。

### 2.2.4 `reg`：遗留 Verilog 名称

旧 Verilog 中，过程块左值必须声明为 `reg`：

```systemverilog
// 合法的旧式写法；在新代码中通常改用 logic。
reg [31:0] alu_result;
always @(*) begin
    alu_result = rs_data + rt_data;
end
```

`reg` 的语言含义是“过程变量”，不是“必然综合成寄存器”。上述代码完整地描述组合逻辑；下面的代码才描述触发器：

```systemverilog
reg [31:0] pc_q;
always @(posedge clk) begin
    pc_q <= pc_d;
end
```

阅读 AI 或旧课程代码时，把 `reg` 翻译为“可被过程赋值的变量”，再根据 `always` 的触发条件判断硬件。

### 2.2.5 多驱动与 `logic`/`wire` 的边界

```systemverilog
logic [31:0] wb_data;

always_comb wb_data = alu_result;
always_comb wb_data = mem_read_data; // 错误：两个过程驱动同一变量。
```

这不是“后写的语句覆盖前写的语句”。它描述了两个独立硬件源同时试图驱动同一对象；工具通常报错或强烈警告。正确硬件是 MUX：

```systemverilog
always_comb begin
    wb_data = mem_to_reg ? mem_read_data : alu_result;
end
```

多驱动 net 在有意使用三态、`wand`/`wor` 等网络解析时有专门语义，但这不是内部 CPU 数据通路的常规做法。

**CPU 应用：**PC、ALU 输出、扩展立即数、写回数据、寄存器堆读数据几乎都适合使用 `logic` 并确保单驱动；模块间纯连接可使用 `wire`，也可依项目风格使用单驱动 `logic`。

**小练习 1：**判断下面两个 `logic` 分别映射什么硬件，并解释为什么不能仅看类型判断。

```systemverilog
logic [31:0] next_pc;
logic [31:0] pc_q;
assign next_pc = pc_q + 32'd4;
always_ff @(posedge clk) pc_q <= next_pc;
```

## 2.3 位向量、整数类型与常量

### 2.3.1 四态值与常见类型

`logic` 可以取 `0`、`1`、`X`（未知）和 `Z`（高阻）。`X` 往往表示未初始化、冲突驱动或未知输入；`Z` 常见于显式三态建模。CPU 内部通常不希望出现 `Z`，而 `X` 是很有价值的 bug 信号。

| 类型 | 状态数 | 常见 RTL 用途 | 注意事项 |
|---|---:|---|---|
| `bit [31:0]` | 2-state | 少数确定值建模 | X/Z 被压成 0/1，不利于发现初始化问题 |
| `logic [31:0]` | 4-state | CPU 数据、控制、状态 | 首选通用 RTL 向量类型 |
| `int` | 32 位、有符号、4-state | 循环变量、测试计数 | 不宜替代明确位宽的数据通路信号 |
| `integer` | 至少 32 位、有符号、4-state | 遗留代码、循环变量 | 宽度依实现；CPU 数据路径应避免 |

CPU 的寄存器数据、地址和控制码应显式声明位宽，如 `logic [31:0]`、`logic [4:0]`。不要用 `int` 表示 MIPS32 通用寄存器值：它的 signed 属性可能悄悄影响比较和算术。

### 2.3.2 尺寸化常量

**语法定义：**尺寸化整数字面量形式为 `<位数>'<基数><数字>`。基数可为 `b`、`o`、`d`、`h`，数字中可用 `_` 分组。

```systemverilog
logic [31:0] reset_vector = 32'hBFC0_0000;
logic [4:0]  zero_reg     = 5'd0;
logic        enabled      = 1'b1;
logic [15:0] imm          = 16'hFF80;
```

常量位宽是设计的一部分：

```systemverilog
assign pc_plus4 = pc_q + 32'd4; // 明确 32 位加法操作数。
assign rs_is_zero = (rs_idx == 5'd0);
```

无尺寸十进制常量如 `4` 不是“4 位数”，通常至少按 `int` 的规则参与表达式，且会带来有符号和尺寸扩展问题。CPU 数据通路中，除循环计数等局部场景外，推荐写成 `32'd4`、`5'd31` 等尺寸化常量。

`'0`、`'1`、`'x`、`'z` 是特殊的未尺寸化填充字面量，会按上下文宽度填充。例如：

```systemverilog
logic [31:0] pc_q;
logic [31:0] regs [0:31];

always_ff @(posedge clk) begin
    if (!rst_n) begin
        pc_q <= '0;  // 这里按 pc_q 的宽度填满 0。
    end
end
```

`'0` 对清零很方便；但对运算操作数，仍推荐明确写出意图和尺寸。

### 2.3.3 `parameter` 与 `localparam`

**语法定义：**`parameter` 是可被实例化处覆盖的 elaboration-time 常量；`localparam` 是模块内部不可从外部覆盖的常量。

```systemverilog
module simple_imem #(
    parameter int ADDR_WIDTH = 10
) (
    input  logic [ADDR_WIDTH-1:0] word_addr,
    output logic [31:0]           instruction
);
    localparam int DEPTH = 1 << ADDR_WIDTH;
    logic [31:0] mem [0:DEPTH-1];

    assign instruction = mem[word_addr];
endmodule
```

**硬件映射：**参数在仿真开始、综合开始前的 elaboration 阶段确定；每个不同参数值的实例可产生不同位宽或不同规模硬件。`DEPTH` 不会成为运行中可修改的寄存器。

**常见错误：**把指令 opcode 写成可覆盖的 `parameter`，使实例化者能无意改变 ISA 编码。指令编码通常应是 `localparam`，或更理想地放入 package 的枚举/常量定义。

**小练习 2：**为什么 `ADDR_WIDTH` 合适用 `parameter`，而 `DEPTH` 合适用 `localparam`？若 `ADDR_WIDTH=10`，`DEPTH` 是多少个字？

## 2.4 packed array：位向量与字段操作

### 2.4.1 packed 与位序

**语法定义：**变量名前、类型后的维度是 packed dimension。`logic [31:0] instruction;` 是一个连续的 32 位 packed 向量，可视作一个 32 位无符号数（除非声明 `signed`）。左边界为最高有效位（MSB），右边界为最低有效位（LSB）。

```systemverilog
logic [31:0] instruction;
logic [4:0]  rs_idx;

assign rs_idx = instruction[25:21];
```

`instruction[25:21]` 是从 bit 25 向下到 bit 21 的 5 位部分选择，正好对应 MIPS 的 `rs` 字段。范围方向不能写反为 `[21:25]`；普通 range part-select 的左边界必须不小于右边界。

另一种常用形式是 indexed part-select：

```systemverilog
logic [7:0] byte0;
assign byte0 = instruction[0 +: 8];   // 从 bit 0 起向上取 8 位，即 [7:0]
```

`base +: width` 向更高编号取位，`base -: width` 向更低编号取位。对固定 MIPS 字段，`[25:21]` 更直观；对参数化总线切片，indexed part-select 更有用。

### 2.4.2 位选择、部分选择与拼接

```systemverilog
logic [31:0] instruction;
logic [5:0]  opcode;
logic [15:0] imm16;
logic [31:0] jump_target;
logic [31:0] pc_plus4;

assign opcode      = instruction[31:26];
assign imm16       = instruction[15:0];
assign jump_target = {pc_plus4[31:28], instruction[25:0], 2'b00};
```

逐项解释：

- `instruction[31:26]` 宽度为 6；
- `instruction[15:0]` 宽度为 16；
- `{...}` 是拼接（concatenation），把三段位串按书写顺序拼为一段；
- `4 + 26 + 2 = 32`，所以 `jump_target` 恰好为 32 位；
- 末尾 `2'b00` 等价于跳转目标字对齐时补的两个低位 0。

**硬件映射：**字段选择和拼接通常只是重连导线，不需要算术单元。拼接写错宽度可能被赋值目标截断或补零，因而很危险。

复制拼接重复位模式：

```systemverilog
logic [15:0] imm16;
logic [31:0] sign_ext;
logic [31:0] zero_ext;

assign sign_ext = {{16{imm16[15]}}, imm16};
assign zero_ext = {16'b0, imm16};
```

`{16{imm16[15]}}` 产生 16 个 `imm16[15]`。若 `imm16 = 16'hFF80`，则：

```text
sign_ext = 32'hFFFF_FF80  // 表示 -128 的 32 位补码
zero_ext = 32'h0000_FF80  // 表示 65408
```

这不是由 `imm16` 是否声明为 `signed` 自动决定的；这里通过拼接把扩展方式明确写入硬件结构。

### 2.4.3 packed 拼接的 signed 陷阱

拼接结果按语言规则视为 unsigned，即使被拼接的某些操作数是 signed。不要依赖拼接表达式的 signed 属性再做算术或比较；需要时显式转换：

```systemverilog
logic signed [15:0] offset16;
logic signed [31:0] offset32;

assign offset32 = $signed({{16{offset16[15]}}, offset16});
```

不过，对于单纯的位模式传递，如上面的 branch offset 拼接，signed 属性不影响连线结果。真正参加加法、比较和算术右移时才必须认真处理 signed。

**小练习 3：**计算 `{pc_plus4[31:28], instruction[25:0], 2'b00}` 的总位宽。若错写为 `{pc_plus4[31:28], instruction[25:0]}` 并赋给 32 位 `jump_target`，低两位会是什么？该跳转地址为什么错误？

## 2.5 unpacked array：寄存器堆和存储器

### 2.5.1 根本区别

维度的位置决定 packed 与 unpacked：

```systemverilog
logic [31:0] word;          // 一个 32 位 packed 向量
logic [31:0] regs [0:31];   // 32 个元素；每个元素是一个 32 位 packed 向量
```

`regs` 的 `[0:31]` 位于变量名后，因此是 unpacked dimension。它更像“32 个 word 的数组”；`regs[3]` 选出第 4 个 32 位元素，`regs[3][7:0]` 再选出该元素的低字节。

```text
logic [31:0] regs [0:31]
       └──── 每个元素的位：31 ... 0（packed）
                    └──── 元素索引：0 ... 31（unpacked）
```

不要把 `logic [31:0][0:31] regs;` 当作寄存器堆的等价写法。两个维度都在名称前时它是一个两维 packed 向量，索引、端口传递和硬件意图都不同。

### 2.5.2 32×32 位寄存器堆

```systemverilog
module regfile (
    input  logic        clk,
    input  logic        write_en,
    input  logic [4:0]  write_addr,
    input  logic [31:0] write_data,
    input  logic [4:0]  read_addr_a,
    input  logic [4:0]  read_addr_b,
    output logic [31:0] read_data_a,
    output logic [31:0] read_data_b
);
    logic [31:0] regs [0:31];

    assign read_data_a = (read_addr_a == 5'd0) ? 32'b0 : regs[read_addr_a];
    assign read_data_b = (read_addr_b == 5'd0) ? 32'b0 : regs[read_addr_b];

    always_ff @(posedge clk) begin
        if (write_en && (write_addr != 5'd0))
            regs[write_addr] <= write_data;
    end
endmodule
```

**逐行解释：**

- `regs [0:31]` 有 32 个 32 位元素，对应 `$0` 到 `$31`；
- 两条 `assign` 描述两个异步读端口；读地址变化后，读数据经组合逻辑变化；
- 写端口在上升沿写入；
- 对地址 0 的读强制输出零，并禁止写入地址 0，满足 MIPS `$zero` 约束。

**硬件映射：**它可以综合为 32×32 的触发器阵列加读 MUX，也可能在某些目标和编码约束下推断存储器。目标技术、读端口数量、读写时序决定具体实现，不能仅从“数组”二字断言一定是 RAM。

**常见错误：**

```systemverilog
logic regs [0:31][31:0]; // 每个元素只有 1 bit；维度与意图不符。
```

或者误把寄存器编号当作 32 位地址：

```systemverilog
regs[rs_data] // 错误意图：rs_data 是寄存器内容，不是 5 位寄存器编号。
```

### 2.5.3 指令/数据存储器数组与地址单位

教学用字寻址指令存储器可以这样描述：

```systemverilog
logic [31:0] imem [0:1023];
logic [31:0] instruction;
logic [31:0] pc_q;

assign instruction = imem[pc_q[11:2]];
```

`imem` 有 1024 个 32 位字。MIPS 的 PC 是**字节地址**，而 `imem` 元素索引是**字地址**，所以取 `[11:2]`，相当于除以 4；低两位应为 `2'b00`。若误写 `imem[pc_q]`，地址既可能越界，也混淆了字节和字。

数组越界在仿真器中的呈现可能是 X、警告、错误或工具特定行为；不能依赖越界后的值。设计和 TB 应主动检查地址范围与对齐。

### 2.5.4 数组端口与兼容性

SystemVerilog 支持 unpacked array 端口，但不同工具版本对复杂数组端口、参数化数组和综合推断的支持程度可能不同。对于当前 CPU，通常让寄存器堆和存储器把数组保持为模块内部实现，只暴露地址、读数据、写使能和写数据端口，接口最稳定。

**小练习 4：**对于 `logic [31:0] dmem [0:255]`，每个元素多宽、共有多少元素、总容量是多少字节？若 `addr` 是字节地址，访问一个 32 位字应使用 `addr` 的哪些位作为 256 项数组索引？

## 2.6 signed、unsigned 与表达式位宽

这一节的规则较密集。先记住一条工程原则：**CPU 数据通路中，不要让关键位宽和符号扩展依赖隐式规则；在声明、常量、拼接和转换处明确表达意图。**

### 2.6.1 声明 signed

默认 packed 向量是 unsigned：

```systemverilog
logic [31:0]        u_data;  // unsigned
logic signed [31:0] s_data;  // signed，两者位模式都可为 32 位补码
```

位模式 `32'hFFFF_FFFF` 在 `u_data` 中解释为 4,294,967,295，在 `s_data` 中解释为 -1。位没有改变；改变的是算术比较等表达式如何解释最高位。

```systemverilog
logic [31:0]        a_u = 32'hFFFF_FFFF;
logic [31:0]        b_u = 32'd1;
logic signed [31:0] a_s = 32'hFFFF_FFFF;
logic signed [31:0] b_s = 32'd1;
logic less_unsigned;
logic less_signed;

assign less_unsigned = (a_u < b_u); // 0：0xFFFF_FFFF 远大于 1
assign less_signed   = (a_s < b_s); // 1：-1 小于 1
```

MIPS `slt` 要求有符号比较，`sltu` 要求无符号比较。即使操作数在顶层统一存为 `logic [31:0]`，也可以在操作处使用显式转换：

```systemverilog
assign slt_result  = ($signed(rs_data) < $signed(rt_data));
assign sltu_result = (rs_data < rt_data);
```

结果是 1 位布尔值；若要写回 32 位寄存器，应显式扩展：

```systemverilog
assign alu_result = {31'b0, ($signed(rs_data) < $signed(rt_data))};
```

### 2.6.2 实用的表达式尺寸规则

完整 LRM 的 context-determined/self-determined 规则有细节，但 CPU RTL 先必须掌握以下可靠结论：

1. 赋给较窄目标时，高位会截断；赋给较宽目标时会扩展，扩展方式受右侧表达式 signed 属性影响。
2. 二元算术和比较会先把操作数调整到足以容纳操作数的共同宽度；若任一操作数为 unsigned，混合表达式通常按 unsigned 处理。不要依赖混合 signed/unsigned 的隐式结果。
3. 拼接表达式的位宽为所有片段位宽之和，并按 unsigned 处理。
4. 比较结果是 1 位；逻辑运算结果也是 1 位。
5. 左、右移结果的位宽通常等于左操作数宽度；移位不会自动把结果扩宽。

最直观的截断例子：

```systemverilog
logic [31:0] wide_sum;
logic [15:0] low_sum;

assign wide_sum = 32'hFFFF_0000 + 32'h0002_0003; // 32'h0001_0003，按 2^32 模运算
assign low_sum  = 32'hFFFF_0000 + 32'h0002_0003; // 只保留低 16 位：16'h0003
```

第二行没有报错也不代表意图正确。Verilator 常会对宽度不匹配发出 `%Warning-WIDTH`；应把它视为需审查的问题，而非习惯性关闭。

### 2.6.3 无尺寸常量、算术与显式扩展

```systemverilog
logic [4:0] shamt;
logic [31:0] shifted;

assign shifted = rs_data << shamt;    // 结果仍为 32 位
assign pc_d    = pc_q + 32'd4;        // 推荐：明确 32 位常量
```

无尺寸 `4` 常以至少 32 位有符号整数语义参与运算。多数 `pc_q + 4` 的简单场景结果看似正常，但当它与不同宽度、signed 信号混合，或代码被参数化时，隐式转换会更难判断。因此 RTL 中宁可多写几位常量尺寸。

需要保留加法进位时，必须把操作数先扩展，而不是只把结果左值加宽：

```systemverilog
logic [31:0] a;
logic [31:0] b;
logic [32:0] sum_with_carry;

assign sum_with_carry = {1'b0, a} + {1'b0, b};
```

这里两个加数已经是 33 位，因此结果可保留 carry。若只写 `sum_with_carry = a + b;`，右侧表达式在常见规则下先按 32 位完成，丢失的进位无法被左侧“找回”。

### 2.6.4 逻辑移位与算术移位

```systemverilog
logic [31:0]        value_u;
logic signed [31:0] value_s;
logic [31:0]        srl_result;
logic [31:0]        sra_result;

assign srl_result = value_u >> 5'd4;  // 逻辑右移：高位补 0
assign sra_result = value_s >>> 5'd4; // 算术右移：signed 左操作数时高位补符号位
```

以 `32'h8000_0000` 为例：

```text
逻辑右移 4 位：0x0800_0000
算术右移 4 位：0xF800_0000
```

`>>>` 不会神奇地把 unsigned 数当负数处理；必须保证左操作数为 signed，或显式写 `$signed(value_u) >>> shamt`。这是 `sra` 指令常见 bug。

### 2.6.5 条件表达式与转换

三目运算符 `cond ? a : b` 是 MUX 的紧凑写法，但 `a` 和 `b` 仍会经历位宽、signed 协调。两个分支最好已具有相同明确位宽。

```systemverilog
logic [31:0] alu_b;
logic [31:0] rt_data;
logic [31:0] imm_ext;
logic        alu_src_imm;

assign alu_b = alu_src_imm ? imm_ext : rt_data;
```

若一个分支是 16 位立即数而另一个是 32 位寄存器数据，虽然工具能进行扩展，但你应先明确扩展：

```systemverilog
// 不推荐：立即数扩展方式隐藏在表达式规则中，且可能不符合指令语义。
assign alu_b = alu_src_imm ? imm16 : rt_data;

// 推荐：先形成明确的 32 位 sign_ext 或 zero_ext。
assign alu_b = alu_src_imm ? imm_ext : rt_data;
```

常用显式转换包括 `$signed(expr)` 和 `$unsigned(expr)`。类型转换只改变该表达式的解释/尺寸上下文，不会在硬件中凭空改变比特；若需要真实的扩位连线，仍要用拼接或明确的目标类型。

**常见错误：**

```systemverilog
// 错误：branch_imm 是 16 位，直接左移并相加，符号扩展意图不明确。
assign branch_target = pc_plus4 + (branch_imm << 2);
```

推荐按语义拆开：

```systemverilog
logic [31:0] branch_offset;
assign branch_offset = {{14{branch_imm[15]}}, branch_imm, 2'b00};
assign branch_target = pc_plus4 + branch_offset;
```

这精确描述“先将 16 位有符号立即数扩到 32 位，再按字对齐左移两位”。

**CPU 应用：**立即数扩展、`slt/sltu`、`sra`、branch offset、ALU carry/overflow 检测、跳转地址拼接都依赖本节知识。

**小练习 5：**令 `rs_data=32'h8000_0000`。分别预测 `$signed(rs_data) < 32'sd0`、`rs_data < 32'd0`、`$signed(rs_data) >>> 3` 和 `rs_data >> 3` 的结果。说明每个表达式的 signed 来源。

## 本章总结

- `logic` 是四态变量类型，既可描述组合输出，也可描述寄存器状态；它不等于硬件寄存器。
- `wire` 是网络类型，适合表达连续连接；`reg` 是旧 Verilog 的过程变量名称，不等于寄存器。
- 是否形成寄存器由时序事件和赋值覆盖决定，是否形成锁存器由组合过程是否完整赋值决定。
- packed 维度在变量名前，表示连续位；unpacked 维度在变量名后，表示元素数组。`logic [31:0] regs [0:31]` 是标准 32×32 寄存器堆声明。
- 字段选择、拼接、复制拼接是 CPU 指令译码、立即数扩展和跳转地址构造的基础。
- 默认向量为 unsigned；拼接结果也按 unsigned 处理。对 `slt`、`sra`、branch offset 等语义敏感位置，要显式处理 signed 和位宽。
- 把 Verilator 的 WIDTH、signed 和索引类警告当作设计审查入口。

## 核心语法速查表

| 写法 | 含义 | CPU 典型用途 |
|---|---|---|
| `logic [31:0] x` | 32 位四态变量，默认 unsigned | 数据通路、寄存器状态 |
| `logic signed [31:0] x` | 32 位有符号变量 | 算术右移、有符号比较的中间值 |
| `wire [31:0] x` | 32 位网络 | 连续连接、明确的导线 |
| `localparam logic [5:0] OP_LW = 6'h23` | 不可被实例覆盖的常量 | opcode、控制编码 |
| `x[25:21]` | 5 位部分选择 | MIPS `rs` 字段 |
| `{a, b}` | 拼接，宽度为各段宽度之和 | jump target、总线打包 |
| `{{16{x[15]}}, x}` | 16 位符号位复制后拼接 | 16→32 位符号扩展 |
| `logic [31:0] regs [0:31]` | 32 个 32 位元素 | 寄存器堆 |
| `$signed(x)` / `$unsigned(x)` | 显式按有/无符号解释表达式 | `slt`、`sra` |
| `{1'b0,a}+{1'b0,b}` | 先扩宽操作数再相加 | 获取 32 位加法 carry |

## 自测题

1. 解释 `logic`、`wire`、`reg` 的语言角色；为什么三者不能直接翻译为“寄存器/非寄存器”？
2. 写出从 `instruction` 中提取 MIPS `opcode`、`rs`、`rt`、`rd` 和 `funct` 的声明与连续赋值。
3. `logic [31:0] regs [0:31]` 中，`regs[17][31]` 表示什么？
4. 给定 `imm16=16'h8001`，分别写出它的 32 位符号扩展和零扩展十六进制结果。
5. 修复下列代码，使它保留两个 32 位操作数相加的 carry：

```systemverilog
logic [32:0] sum;
assign sum = a + b;
```

6. 为什么 `assign result = value >>> shamt;` 不能仅凭 `>>>` 保证实现 MIPS `sra`？
7. 阅读 AI 生成的 `logic [31:0][0:31] regs;`，说明它与寄存器堆期望声明的差异，并给出更清晰的替代写法。

---

# 第三部分：组合逻辑

## 本章目标

能够用 `assign`、`always_comb`、`if` 和 `case` 阅读或修改 MUX、ALU、译码器与下一 PC 选择逻辑；能发现锁存器、多驱动和组合环路。

**前置知识：**第一、二部分。  
**预计学习时间：**4–6 小时。

## 3.1 连续赋值 `assign`

**语法定义：**`assign lhs = rhs;` 是模块级连续赋值。右侧任一依赖信号变化时，左侧被持续重新驱动；适合简单、直接的组合关系。

```systemverilog
module add_pc4 (
    input  logic [31:0] pc_q,
    output logic [31:0] pc_plus4
);
    assign pc_plus4 = pc_q + 32'd4;
endmodule
```

逐行看，端口定义一条 32 位输入和输出；`assign` 将一个 32 位加法器结果持续连接到 `pc_plus4`。它没有时钟、没有保存状态。仿真中 `pc_q` 改变会触发该表达式重新求值；综合中得到加法器和连线。

连续赋值也很适合字段提取、固定扩展和简单 MUX：

```systemverilog
assign rs_idx  = instruction[25:21];
assign imm_ext = {{16{instruction[15]}}, instruction[15:0]};
assign alu_b   = alu_src_imm ? imm_ext : rt_data;
```

**常见错误：**同一信号既有连续赋值又有过程赋值，或有两个 `assign`：

```systemverilog
assign wb_data = alu_result;
assign wb_data = mem_read_data; // 两个驱动源，不是“覆盖”。
```

应改为一个 MUX 表达式。`assign` 也不适合需要多分支、临时变量或默认值的复杂译码；此时使用 `always_comb` 更清晰。

**CPU 应用：**`PC+4`、立即数固定扩展、指令字段、写回 MUX、寄存器堆异步读端口。

**小练习：**用一条 `assign` 写出 `lui` 的结果 `instruction[15:0] << 16`，要求不依赖隐式位宽规则。

## 3.2 `always_comb` 与阻塞赋值

**语法定义：**`always_comb begin ... end` 声明组合过程块。语言会自动纳入右侧读取信号的敏感关系，并对组合逻辑意图提供额外检查；组合块内通常使用阻塞赋值 `=`。

```systemverilog
always_comb begin
    alu_b = rt_data;                 // 默认路径
    if (alu_src_imm)
        alu_b = imm_ext;             // 覆盖默认路径
end
```

每次相关输入变化时，该块执行一次。块内第一行马上更新过程变量 `alu_b`，因此后续语句能读取本次值；综合后是一个 MUX。阻塞赋值只表示**该过程执行期间**立即更新，不表示硬件中存在顺序执行器。

复杂 ALU 的标准骨架：

```systemverilog
always_comb begin
    alu_result = 32'b0;
    unique case (alu_op)
        ALU_ADD:  alu_result = rs_data + alu_b;
        ALU_SUB:  alu_result = rs_data - alu_b;
        ALU_AND:  alu_result = rs_data & alu_b;
        ALU_OR:   alu_result = rs_data | alu_b;
        ALU_SLT:  alu_result = {31'b0, ($signed(rs_data) < $signed(alu_b))};
        default:  alu_result = 32'b0;
    endcase
end
```

**完整赋值规则：**每条可达路径都必须赋值给组合输出。最易维护的方法是块开头给所有输出默认值，再在分支中覆盖。

```systemverilog
// 错误：alu_op 不匹配时 alu_result 保持上次值，综合出锁存器。
always_comb begin
    if (alu_op == ALU_ADD)
        alu_result = rs_data + rt_data;
end
```

锁存器为了“保持上次值”需要存储状态，违背了你本想描述纯 ALU 的意图，也可能制造难以分析的时序路径。修复方式是默认赋值或完整 `else/case default`。

`always @(*)` 是 Verilog-2001 的组合逻辑写法；新 SystemVerilog RTL 优先写 `always_comb`。两者都不应依赖不完整赋值；`always_comb` 的意图更明确，工具也更容易检查。

**CPU 应用：**控制单元、ALU、下一 PC 逻辑、forwarding 选择器。

**小练习：**给 `alu_result` 增加 `ALU_XOR`，并说明为什么要同时更新 enum/package 和 `case` 分支。

## 3.3 `if`、`case`、`casez`

`if/else` 自然描述优先级 MUX：后面的条件只有在前面不成立时才有效。

```systemverilog
always_comb begin
    pc_d = pc_plus4;
    if (take_branch)
        pc_d = branch_target;
    if (jump)
        pc_d = jump_target;
end
```

硬件是以 `pc_plus4` 为默认输入、branch 和 jump 覆盖的选择网络。若两者同时为 1，`jump` 优先。这必须是 ISA/控制单元明确保证的设计，而不能是偶然的源码副作用。

`case` 更适合互斥编码选择：

```systemverilog
always_comb begin
    reg_write = 1'b0;
    mem_write = 1'b0;
    unique case (opcode)
        OP_RTYPE: reg_write = 1'b1;
        OP_LW:    reg_write = 1'b1;
        OP_SW:    mem_write = 1'b1;
        default: ;
    endcase
end
```

`unique case` 表达“至多一个分支应匹配”的设计意图；仿真器可在无匹配或多重匹配时给出诊断。它不自动修复译码错误，也不能替代 `default` 和默认赋值。

`casez` 将 `z` 和 `?` 当作无关位，常用于带掩码的指令分类：

```systemverilog
casez (instruction)
    32'b000000_?????_?????_?????_00000_100000: alu_op = ALU_ADD;
    default:                                     alu_op = ALU_INVALID;
endcase
```

实际 CPU 译码常先按 `opcode`，R 型再按 `funct`，可读性通常优于把整条指令写成巨大 `casez`。避免 `casex`：它把 X 也当作无关位，可能隐藏未初始化或连接错误。

**常见错误：**将 `casez` 中的 `?` 误放到实际必须匹配的 funct 位；缺少 `default` 导致控制信号沿用默认不当或推断锁存器。对非法指令，默认应关闭写使能并给 `ALU_INVALID`，而不是随意执行加法。

**小练习：**为 `beq` 写出最少的控制默认值：是否写寄存器、是否写内存、ALU 应做什么、何时选择 branch target？

## 3.4 MUX、ALU、完整赋值与组合环路

三目表达式和 `if/case` 都能描述 MUX；选择一种项目内一致、便于审查的写法。一个输出只有两种简单来源时三目表达式简洁；多个控制信号共同生成时 `always_comb` 通常更清晰。

组合环路是没有触发器打断的反馈：

```systemverilog
assign a = ~b;
assign b = ~a; // 组合反馈；仿真可能不断迭代，综合时序也不可控。
```

在 CPU 中，`pc_d` 依赖 `pc_q` 是正常的，因为 `pc_q` 是寄存器；让 `pc_d` 又通过组合路径依赖自身则是错误。读 AI 代码时，沿每个组合输出反向追踪，必须最终回到模块输入或寄存器输出，而不是回到自己。

## 本章总结

- `assign` 是持续的组合驱动，适合直接关系；`always_comb` 适合多语句组合决策。
- 组合块内通常使用 `=`，并为每个输出提供默认赋值或完整分支。
- `if` 描述优先级选择，`case` 描述编码选择，`casez` 仅在掩码意图明确时使用；避免 `casex`。
- 未完整赋值会推断锁存器；多驱动和组合环路是结构错误。

## 核心语法速查表

| 写法 | 硬件/语义 |
|---|---|
| `assign y = a ? b : c;` | 连续驱动的二选一 MUX |
| `always_comb` | 自动敏感的组合过程块 |
| 默认值后 `if` 覆盖 | 无锁存器的优先级 MUX |
| `case (...) ... default` | 编码选择网络，default 处理其他编码 |
| `unique case` | 声明互斥匹配意图，辅助仿真诊断 |

## 自测题

1. 为什么组合块内的默认赋值不是“给寄存器初始化”？
2. 找出 `pc_d` 选择代码中的优先级，并说明如何把它改为 branch 优先。
3. 为什么 CPU 控制译码中通常避免 `casex`？
4. 将一个 `lw/sw/R-type` 控制单元写成一个默认赋值加 `case` 的组合块。

---

# 第四部分：时序逻辑

## 本章目标

能够正确阅读和修改 `always_ff`、复位、使能、PC、寄存器堆写端口和流水线寄存器；准确解释同一时钟沿的非阻塞赋值语义。

**前置知识：**第二、三部分。  
**预计学习时间：**5–7 小时。

## 4.1 `always_ff`、时钟沿与复位

**语法定义：**`always_ff @(posedge clk)` 描述由时钟上升沿触发的时序过程；`always_ff @(posedge clk or negedge rst_n)` 是常见低有效异步复位形式。

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n)
        pc_q <= 32'h0000_0000;
    else if (pc_enable)
        pc_q <= pc_d;
end
```

`rst_n` 为 0 时触发复位，不必等待时钟；否则仅在上升沿检查 `pc_enable`。使能为 0 时不赋值，表示触发器保持状态，这是时序块中合法且常见的“保持”。综合硬件是 32 位寄存器，带异步清零和使能。

同步复位只把时钟放进敏感事件：

```systemverilog
always_ff @(posedge clk) begin
    if (!rst_n)
        pc_q <= '0;
    else
        pc_q <= pc_d;
end
```

两者的选择是芯片/课程/时序约束决策，不是语法偏好。同步复位只有上升沿才生效；异步复位在任意时刻断言就生效，释放时则需要遵循目标设计的恢复/撤销时序要求。

## 4.2 非阻塞赋值 `<=`：旧状态并行更新

**语法定义：**非阻塞赋值在过程运行时先计算右值并安排左值更新；更新在当前时间步的 NBA 区域统一发生。时序逻辑应使用 `<=`。

```systemverilog
always_ff @(posedge clk) begin
    if (en) begin
        stage2_q <= stage1_q;
        stage1_q <= input_d;
    end
end
```

若沿到来前 `stage1_q=A`、`stage2_q=B`、`input_d=C`，沿后结果必为 `stage1_q=C`、`stage2_q=A`。`stage2_q` 不会看见同一沿刚写入 `stage1_q` 的 C；这正是流水线寄存器并行更新需要的语义。

错误地写阻塞赋值：

```systemverilog
always_ff @(posedge clk) begin
    stage1_q = input_d;
    stage2_q = stage1_q; // 仿真中读到新 stage1_q，破坏流水级边界。
end
```

该仿真行为像 C 语言变量顺序覆盖，可能与你期待的触发器网络不一致。即使某些综合结果看似合理，也不要依赖这种写法。

寄存器交换是最小反例：

```systemverilog
// 正确：沿后 a_q 获得旧 b_q，b_q 获得旧 a_q。
always_ff @(posedge clk) begin
    a_q <= b_q;
    b_q <= a_q;
end
```

**CPU 应用：**PC 更新、寄存器堆写回、IF/ID 等流水线寄存器全部依赖该规则。

## 4.3 next-state / state 分离

推荐把复杂的下一状态计算放入组合块，把状态写入集中在时序块：

```systemverilog
always_comb begin
    pc_d = pc_q + 32'd4;
    if (take_branch) pc_d = branch_target;
    if (jump)        pc_d = jump_target;
end

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) pc_q <= '0;
    else        pc_q <= pc_d;
end
```

这使读者立即看出：`pc_q` 是状态；所有跳转选择只影响下一状态；一周期内 PC 不会因组合信号抖动而多次“写入”。

## 4.4 流水线寄存器、stall 与 flush

流水线寄存器不是一堆随意的寄存器，而是阶段边界。应同时保存数据和后续阶段需要的控制信号：

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
        id_ex_valid_q     <= 1'b0;
        id_ex_reg_write_q <= 1'b0;
        id_ex_rs_data_q   <= '0;
    end else if (flush_id_ex) begin
        id_ex_valid_q     <= 1'b0; // bubble：使后续阶段无副作用
        id_ex_reg_write_q <= 1'b0;
    end else if (id_ex_enable) begin
        id_ex_valid_q     <= if_id_valid_q;
        id_ex_reg_write_q <= decoded_reg_write;
        id_ex_rs_data_q   <= rs_data;
    end
end
```

常见优先级是 reset > flush > enable/stall；但具体选择取决于你的冒险策略。stall 通常保持当前寄存器值，flush 通常把 valid/写使能清零形成 bubble。不要只清数据、不清写使能，否则“无效指令”仍可能写寄存器或内存。

## 4.5 寄存器数组读写与同沿问题

典型 MIPS 寄存器堆：异步组合读、同步写。

```systemverilog
assign rs_data = (rs_idx == 5'd0) ? '0 : regs[rs_idx];
always_ff @(posedge clk) begin
    if (reg_write && (rd_idx != 5'd0))
        regs[rd_idx] <= wb_data;
end
```

同一个上升沿，读端口在沿前看到旧数组；写入在 NBA 更新后生效，随后组合读端口重新反映新数组。若你的单周期数据通路或测试依赖“同沿读写同地址”的特殊 read-first/write-first 行为，必须显式旁路并测试，不能假设不同 RAM 推断目标具有相同行为。

## 本章总结

- `always_ff` 描述状态更新；时序块中使用 `<=`。
- 非阻塞赋值使所有寄存器在同沿基于旧状态采样、在 NBA 阶段共同更新。
- 同步/异步复位的差别在触发条件和物理实现要求，不在复位值。
- 状态 `_q` 与下一状态 `_d` 分离，是阅读和维护 CPU 时序的最可靠结构。
- 流水线 flush 必须消除副作用控制，stall 必须定义保持哪些状态。

## 核心语法速查表

| 写法 | 含义 |
|---|---|
| `always_ff @(posedge clk)` | 上升沿寄存器 |
| `always_ff @(posedge clk or negedge rst_n)` | 带低有效异步复位的寄存器 |
| `q <= d;` | 采样 d，NBA 阶段更新 q |
| `if (enable) q <= d;` | 带使能的寄存器；否则保持 |
| `if (flush) valid_q <= 0;` | 插入无副作用 bubble 的常见形式 |

## 自测题

1. 沿前 `a_q=1,b_q=2`，代码 `a_q<=b_q; b_q<=a_q;` 后各是多少？
2. 同步复位和异步复位分别在何时响应 `rst_n=0`？
3. 为什么流水线寄存器必须流水化 `reg_write`、`mem_write` 等控制信号？
4. 解释为什么时序块里缺少 `else` 有时正确，而组合块里常常错误。

---

# 第五部分：模块化与工程组织

## 本章目标

能够跨文件追踪 CPU 模块接口和参数，读懂 package、枚举、packed 结构体与 generate，并建立可维护的 RTL/TB 工程组织。

**前置知识：**前四部分。  
**预计学习时间：**4–5 小时。

## 5.1 实例化、命名端口与参数化

模块实例化是在父模块中放置一个子模块硬件实例。优先使用命名端口：

```systemverilog
alu #(.WIDTH(32)) u_alu (
    .a      (rs_data),
    .b      (alu_b),
    .op     (alu_op),
    .result (alu_result),
    .zero   (alu_zero)
);
```

`alu` 是模块类型，`u_alu` 是实例名；`.a(rs_data)` 将父模块信号连接到子模块端口。`#(.WIDTH(32))` 在 elaboration 阶段覆盖参数，产生对应位宽实例。实例化不是运行时函数调用，所有实例并发存在。

位置端口连接 `alu u_alu(rs_data, alu_b, alu_op, alu_result, alu_zero);` 在语法上可用，但端口顺序错位时可能仍能编译，CPU 项目应避免。

参数化 MUX 示例：

```systemverilog
module mux2 #(parameter int WIDTH = 32) (
    input  logic [WIDTH-1:0] a, b,
    input  logic             sel,
    output logic [WIDTH-1:0] y
);
    assign y = sel ? b : a;
endmodule
```

参数必须保持可综合的静态值。`WIDTH=0`、以参数产生负 range、或接口两侧参数不一致，都是 AI 生成参数化代码常见的边界 bug。

## 5.2 package 与共享常量

`package` 组织跨模块复用的类型和常量。它比散落的 ``include`` 更有命名空间和类型检查优势：

```systemverilog
package cpu_pkg;
    typedef enum logic [3:0] {
        ALU_ADD, ALU_SUB, ALU_AND, ALU_OR,
        ALU_SLT, ALU_SLTU, ALU_SLL, ALU_SRL, ALU_SRA,
        ALU_INVALID
    } alu_op_t;

    localparam logic [5:0] OP_RTYPE = 6'h00;
    localparam logic [5:0] OP_LW    = 6'h23;
    localparam logic [5:0] OP_SW    = 6'h2B;
endpackage
```

使用处：

```systemverilog
module alu (
    input  logic [31:0] a, b,
    input  cpu_pkg::alu_op_t op,
    output logic [31:0] result
);
    import cpu_pkg::*;
    // 使用 ALU_ADD 等名称。
endmodule
```

可以使用限定名 `cpu_pkg::alu_op_t`，或 `import cpu_pkg::*;` 后使用短名。工程文件列表中 package 必须先编译。避免循环依赖：package 通常只包含类型、常量、函数，避免反向实例化模块。

## 5.3 enum 与 packed struct

`typedef enum logic [3:0]` 让控制码有明确类型和名称，比裸 `4'd3` 更适合审查与波形阅读。枚举宽度必须覆盖所有枚举值；非法或未初始化编码仍可能发生，因此译码/ALU 需要 `default`。

流水线中，多个相关信号可打包为 `struct packed`：

```systemverilog
typedef struct packed {
    logic        valid;
    logic        reg_write;
    logic        mem_write;
    logic        mem_to_reg;
    logic [4:0]  rd_idx;
    logic [31:0] alu_result;
    logic [31:0] store_data;
} ex_mem_t;

ex_mem_t ex_mem_d, ex_mem_q;
```

packed struct 是一个连续位向量，字段可用 `ex_mem_q.alu_result` 访问，也可整体作为流水线寄存器的左/右值：

```systemverilog
always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) ex_mem_q <= '0;
    else        ex_mem_q <= ex_mem_d;
end
```

**硬件映射：**这仍是一组触发器；struct 只是给总线字段命名。其好处是数据与控制不易在流水线边界遗漏。注意：整体 `<= '0` 需要确认每个字段清零确实符合 reset/flush 语义；有些字段清零不足以消除副作用时，应显式检查 `valid` 和写使能。

## 5.4 generate：展开期重复硬件

`generate`/`genvar` 发生在 elaboration，不是在仿真运行时循环。适合静态重复实例：

```systemverilog
genvar i;
generate
    for (i = 0; i < 32; i++) begin : g_reg_bit
        dff u_dff (.clk(clk), .d(d[i]), .q(q[i]));
    end
endgenerate
```

综合前它等价于 32 个具名 `dff` 实例。对可用向量赋值描述的简单寄存器，不需要为了“看起来硬件化”而使用 generate；`always_ff` 更简单。`generate if` 的条件也必须是参数等常量表达式，不能用运行期控制信号。

## 5.5 工程组织与阅读顺序

建议把综合 RTL 与验证代码隔离：

```text
rtl/       package、子模块、CPU 顶层
tb/        testbench、reference model、断言
tests/     程序映像 .hex 和预期结果
sim/       Makefile/脚本、波形输出（可忽略版本控制）
```

读 AI 工程时，先读 `cpu_pkg.sv` 和顶层端口，再追踪 PC、指令存储器、寄存器堆、控制单元、ALU、存储器、写回；不要先被一大段 Testbench 干扰。

## 本章总结

- 命名端口连接是 CPU 顶层的默认选择；参数决定实例的静态硬件形状。
- package 集中共享类型和 ISA 编码，并要求正确编译顺序。
- enum 增强控制码可读性；packed struct 使流水线数据和控制作为整体传递。
- generate 展开静态重复结构，不是运行时循环。

## 核心语法速查表

| 写法 | 用途 |
|---|---|
| `mod #(.P(v)) u (.a(x));` | 参数覆盖和命名端口实例化 |
| `package ... endpackage` | 公共类型、常量命名空间 |
| `typedef enum logic [N:0]` | 有名控制码类型 |
| `typedef struct packed` | 可整体传递的命名总线/流水线束 |
| `generate for (genvar ...)` | 静态重复实例化 |

## 自测题

1. 为什么 package 应先于使用它的模块编译？
2. 将 EX/MEM 阶段所需的数据和控制列入一个 packed struct。
3. `generate if (branch_taken)` 为什么通常错误？
4. 检查一个位置端口实例化时，应优先核对什么？

---

# 第六部分：CPU RTL 中的典型语法

## 本章目标

把前五部分的语言知识映射到一个基础 MIPS32 单周期 CPU，并能够识别五级流水线版中同一功能的对应位置。

**前置知识：**前五部分与 MIPS32 指令格式。  
**预计学习时间：**8–12 小时。

以下示例是教学骨架：省略异常、中断、未对齐访问、乘除多周期语义等扩展。每个接口与控制信号都应由项目 ISA 规范定义。

## 6.1 PC 更新

```systemverilog
assign pc_plus4     = pc_q + 32'd4;
assign branch_offset = {{14{imm16[15]}}, imm16, 2'b00};
assign branch_target = pc_plus4 + branch_offset;
assign jump_target   = {pc_plus4[31:28], instruction[25:0], 2'b00};

always_comb begin
    pc_d = pc_plus4;
    if (branch_taken) pc_d = branch_target;
    if (jump)         pc_d = jump_target;
    if (jump_reg)     pc_d = rs_data;
end

always_ff @(posedge clk or negedge rst_n) begin
    if (!rst_n) pc_q <= RESET_PC;
    else if (pc_enable) pc_q <= pc_d;
end
```

四个 `assign` 描述加法器、符号扩展/左移连线、加法器和跳转地址拼接；`always_comb` 描述有优先级的下一 PC MUX；`always_ff` 才是 PC 寄存器。`jr` 时通常还要确认 `rs_data[1:0]==0` 的对齐约束。若 branch、jump、jump_reg 同时为真，最后一项优先；控制单元应使这种组合不可能，或通过断言检测。

## 6.2 指令字段与立即数

```systemverilog
assign opcode = instruction[31:26];
assign rs_idx = instruction[25:21];
assign rt_idx = instruction[20:16];
assign rd_idx = instruction[15:11];
assign shamt  = instruction[10:6];
assign funct  = instruction[5:0];
assign imm16  = instruction[15:0];

always_comb begin
    unique case (imm_kind)
        IMM_SIGN: imm_ext = {{16{imm16[15]}}, imm16};
        IMM_ZERO: imm_ext = {16'b0, imm16};
        IMM_LUI:  imm_ext = {imm16, 16'b0};
        default:  imm_ext = '0;
    endcase
end
```

字段提取只是位连线；扩展器是控制信号选择下的 MUX 网络。`addi/lw/sw/beq` 通常用 `IMM_SIGN`，`andi/ori/xori` 用 `IMM_ZERO`，`lui` 用 `IMM_LUI`。不要以“立即数字段最高位是 1”为由自动符号扩展所有 I 型指令。

## 6.3 寄存器堆与写回

```systemverilog
assign rs_data = (rs_idx == 5'd0) ? '0 : regs[rs_idx];
assign rt_data = (rt_idx == 5'd0) ? '0 : regs[rt_idx];
assign wb_rd   = reg_dst_rd ? rd_idx : rt_idx;
assign wb_data = mem_to_reg ? mem_read_data : alu_result;

always_ff @(posedge clk) begin
    if (reg_write && (wb_rd != 5'd0))
        regs[wb_rd] <= wb_data;
end
```

前四条为双读端口、目的寄存器 MUX 和写回数据 MUX；最后一块为同步写端口。`$zero` 的保护应同时覆盖读和写，不能只在 reset 时初始化 `regs[0]`，否则后续错误写入会破坏它。

## 6.4 ALU 与控制生成

```systemverilog
always_comb begin
    reg_write   = 1'b0;
    mem_write   = 1'b0;
    mem_to_reg  = 1'b0;
    alu_src_imm = 1'b0;
    alu_op      = ALU_INVALID;
    imm_kind    = IMM_SIGN;

    unique case (opcode)
        OP_RTYPE: begin reg_write = 1'b1; alu_op = decode_funct(funct); end
        OP_ADDI:  begin reg_write = 1'b1; alu_src_imm = 1'b1; alu_op = ALU_ADD; end
        OP_LW:    begin reg_write = 1'b1; mem_to_reg = 1'b1; alu_src_imm = 1'b1; alu_op = ALU_ADD; end
        OP_SW:    begin mem_write = 1'b1; alu_src_imm = 1'b1; alu_op = ALU_ADD; end
        OP_ANDI:  begin reg_write = 1'b1; alu_src_imm = 1'b1; imm_kind = IMM_ZERO; alu_op = ALU_AND; end
        default: ;
    endcase
end
```

默认值确保非法 opcode 不产生写寄存器/写内存副作用。`decode_funct` 应同样对未知 funct 产生 `ALU_INVALID`，并让 ALU 默认结果安全、验证环境报告错误。译码不是“执行指令”，而是在当前 instruction 位上产生控制电平。

ALU 的 `slt/sltu/sra` 要显式有符号性：

```systemverilog
ALU_SLT:  alu_result = {31'b0, ($signed(rs_data) < $signed(alu_b))};
ALU_SLTU: alu_result = {31'b0, (rs_data < alu_b)};
ALU_SRA:  alu_result = $signed(rs_data) >>> shamt;
```

## 6.5 数据存储器

```systemverilog
logic [31:0] dmem [0:255];
logic [7:0]  dmem_word_addr;

assign dmem_word_addr = alu_result[9:2];
assign mem_read_data  = dmem[dmem_word_addr];

always_ff @(posedge clk) begin
    if (mem_write)
        dmem[dmem_word_addr] <= rt_data;
end
```

该教学模型是组合读、同步写的字存储器。`alu_result` 为字节地址，`[9:2]` 为 256 个字的索引；低两位不是索引而是字内字节偏移。真实字节/半字访存还需字节使能、对齐检查、符号/零扩展及端序定义。不要在未实现这些功能时声称支持 `lb/lh/sb/sh`。

## 6.6 五级流水线对应关系

单周期中的组合信号在流水线中会跨阶段寄存：

| 单周期位置 | 流水线位置 | 必须随数据传递的控制 |
|---|---|---|
| 取指结果 | IF/ID | valid、PC、instruction |
| 译码/寄存器读结果 | ID/EX | ALU op、ALUSrc、RegWrite、MemWrite、MemToReg、rd |
| ALU 结果 | EX/MEM | MemWrite、MemToReg、RegWrite、store data、rd |
| 存储器读结果 | MEM/WB | RegWrite、MemToReg、ALU result、load data、rd |

冒险单元是组合逻辑：比较各阶段寄存器号，产生 forwarding select、stall 和 flush；这些信号由时序逻辑在边界落实。若将 `reg_write` 忘记从 ID/EX 带到 MEM/WB，数据即使正确到达也不会写回。

## 本章总结

- PC：组合计算 `pc_d`，时钟沿写入 `pc_q`。
- 指令字段/拼接/扩展基本是组合连线与 MUX；扩展方式来自译码。
- 寄存器堆通常双组合读、单时序写，`$zero` 必须永久保护。
- 控制单元要先给安全默认值；ALU 的 signed 语义需显式表达。
- 存储器地址先明确字节地址还是字地址；流水线必须让控制随数据前进。

## 核心语法速查表

| 模块 | 关键语法模式 |
|---|---|
| PC | `_d` 的 `always_comb` + `_q` 的 `always_ff` |
| 译码 | 默认赋值 + `unique case(opcode)` |
| regfile | `logic [31:0] regs [0:31]` + 组合读/时序写 |
| ALU | enum op + 完整 `case` |
| dmem | 数组索引 + 明确读写时序 |
| 流水线 | packed struct + `valid`/写使能控制 |

## 自测题

1. 为 `ori` 补全控制信号，并解释其立即数不能符号扩展。
2. 追踪 `lw $t0,8($t1)` 从 `rs_idx` 到 `regs[8]` 写回的全部数据和控制信号。
3. 解释为什么 `dmem[alu_result]` 对字节地址模型通常错误。
4. 列出 load-use hazard 处理中需要 stall 或 flush 的流水线寄存器和原因。

---

# 第七部分：仿真与验证

## 本章目标

能够读写基础 Testbench，正确生成时钟/复位、装载程序、检查 CPU 结果，并用 Verilator、Icarus 和 GTKWave 定位时序与功能问题。

**前置知识：**前六部分。  
**预计学习时间：**5–7 小时。

## 7.1 Testbench 的职责

Testbench（TB）是仿真环境，不是 CPU 硬件。它负责实例化 DUT（device under test）、产生时钟和复位、提供输入程序和数据、检查结果、保存波形并结束仿真。

```systemverilog
`timescale 1ns/1ps
module tb_single_cycle_cpu;
    logic clk;
    logic rst_n;

    single_cycle_cpu dut (
        .clk   (clk),
        .rst_n (rst_n)
    );

    initial begin
        clk = 1'b0;
        forever #5 clk = ~clk;
    end

    initial begin
        rst_n = 1'b0;
        repeat (2) @(posedge clk);
        rst_n = 1'b1;
    end
endmodule
```

`initial` 在仿真开始时执行一次；`forever #5` 产生周期 10 ns 的时钟；`repeat (2) @(posedge clk)` 比硬编码 `#17` 更清楚地表达“复位跨越两个时钟沿”。这些都是仿真控制语句，通常不可综合。

`timescale 1ns/1ps` 指明本编译单元的时间单位/精度；具体工程可改用 SystemVerilog `timeunit`/`timeprecision`。不要让不同文件无意使用不同时间单位。

## 7.2 采样、显示、fatal 与断言

最小检查应发生在设计状态稳定之后：

```systemverilog
initial begin
    wait (rst_n == 1'b1);
    repeat (20) @(posedge clk);
    #1step;

    if (dut.u_regfile.regs[8] !== 32'd42)
        $fatal(1, "expected $t0=42, got %h", dut.u_regfile.regs[8]);
    $display("PASS: $t0=%0d", dut.u_regfile.regs[8]);
    $finish;
end
```

- `$display` 在执行处打印；`$monitor` 在任一参数变化时重复打印，适合短调试，不适合长 CPU 波形；
- `$fatal` 报告失败并终止（具体退出码/格式受工具影响）；`$error` 报错但通常继续；
- `!==` 是 case inequality，把 X/Z 与 0/1 的差异也视为失败；验证已知结果时通常比 `!=` 更好；
- `#1step` 表示推进一个仿真精度步以避开同一时刻 NBA 更新竞争。并非所有旧工具版本对该写法支持完全一致；可用明确的小时间延迟作为工程替代，但应了解其时间精度含义。

immediate assertion 是过程语句：

```systemverilog
always @(posedge clk) begin
    if (rst_n) begin
        assert (!(dut.mem_write && dut.reg_write && illegal_instruction))
            else $error("illegal instruction has architectural side effect");
    end
end
```

它在该过程执行到 assertion 时立即求值。并发 SVA 断言、覆盖率和 UVM 可后续学习；基础 CPU 项目先把确定的架构结果检查清楚。

## 7.3 `$readmemh` 与初始化

```systemverilog
initial begin
    $readmemh("tests/add.hex", dut.u_imem.mem);
end
```

`$readmemh` 将十六进制文本装入数组；`$readmemb` 读二进制文本。每行通常是一个数组元素的值，文件路径相对于仿真启动目录还是源文件目录取决于工具/脚本，应固定工作目录或传入绝对/明确相对路径。验证前应确认映像的元素单位：若 `imem` 是 `logic [31:0] mem [0:N-1]`，每一行应是一条 32 位机器码，而不是一个字节。

初始化语句服务仿真。某些 FPGA 流程可把特定初始化转换为配置内容，ASIC 通常不把任意 `initial` 当作可综合复位逻辑；不要把工具特性误当语言通用保证。

## 7.4 delta cycle 与 Testbench 竞争

同一仿真时刻存在多个调度区域。理解基础次序即可：时钟变化触发 `always_ff`；时序块计算右值并安排 `<=`；NBA 区域更新寄存器；更新后的寄存器又触发组合逻辑。组合链可能经历多个 delta cycle 才稳定，但仿真时间不前进。

竞争例子：

```systemverilog
// TB 和 DUT 都在 posedge 中活动；TB 是否读到写回后的值不应依赖调度先后。
always @(posedge clk) begin
    if (dut.u_regfile.regs[8] != 32'd42)
        $error("may sample too early");
end
```

修复方法之一是在沿后采样：

```systemverilog
@(posedge clk);
#1step;
assert (dut.u_regfile.regs[8] === 32'd42);
```

更根本的原则是：TB 驱动输入应在采样沿之前建立稳定，检查状态应在 DUT 的 NBA 更新和相关组合传播后进行。不要通过“把 TB `always` 写在 DUT 后面”解决竞争；文件顺序不是同步机制。

## 7.5 Verilator、Icarus 与 GTKWave

典型命令（文件名按你的工程调整）：

```bash
# WSL/Ubuntu 中：先做 lint；--Wall 打开较多有用诊断。
verilator --lint-only --Wall rtl/cpu_pkg.sv rtl/*.sv

# 构建并运行带 SystemVerilog Testbench 的仿真。
verilator --binary --timing --trace --top-module tb_single_cycle_cpu \
  rtl/cpu_pkg.sv rtl/*.sv tb/tb_single_cycle_cpu.sv
./obj_dir/Vtb_single_cycle_cpu

# Icarus 的常见快速流程。
iverilog -g2012 -o simv rtl/cpu_pkg.sv rtl/*.sv tb/tb_single_cycle_cpu.sv
vvp simv

# 打开 VCD 波形（文件实际名称由 TB/工具决定）。
gtkwave sim/wave.vcd
```

以上是工具使用范例，不是语言规范：Verilator 偏向编译/静态检查，某些完整 SystemVerilog TB 特性或延迟行为需要特定选项；Icarus 的 SystemVerilog 支持也随版本变化。以项目锁定的版本、CI 命令和工具文档为准。

若需要显式记录波形，常见 TB 代码为：

```systemverilog
initial begin
    $dumpfile("sim/wave.vcd");
    $dumpvars(0, tb_single_cycle_cpu);
end
```

`$dumpfile/$dumpvars` 是常见 VCD 仿真系统任务，工具对支持和性能可能不同。GTKWave 的调试顺序建议：先加 `clk/rst_n`，再加 `pc_q/instruction`，然后加译码控制、寄存器读数、ALU、访存、写回；每次只追踪一条指令，核对其在每个边沿前后的值。

## 本章总结

- TB 用 `initial`、延迟和系统任务操纵仿真，不属于 CPU 可综合 RTL。
- 以时钟沿为参照产生激励和采样；沿后检查要避开 NBA/delta cycle 竞争。
- `$fatal`、立即断言和 `!==` 使失败尽早且可定位。
- `$readmemh` 的数组元素单位、文件路径和初始化边界必须明确。
- Verilator warning 是设计审查信息；GTKWave 应按一条指令的数据/控制路径追踪。

## 核心语法速查表

| 写法 | 用途 |
|---|---|
| `initial begin ... end` | 一次性 TB 过程 |
| `forever #5 clk=~clk;` | 仿真时钟发生器 |
| `@(posedge clk)` | 等待时钟上升沿 |
| `$display/$error/$fatal` | 输出、报错、终止 |
| `$readmemh(file, mem)` | 装载十六进制数组映像 |
| `assert (expr) else ...` | 即时检查 |
| `$dumpfile/$dumpvars` | 常见 VCD 记录 |

## 自测题

1. 为什么 `@(posedge clk)` 后立刻检查寄存器堆可能产生竞争？
2. 为一条 `addi` 程序写一个 `$readmemh`、运行 3 个周期、检查 `$t0` 的 TB 片段。
3. `$readmemh` 文件每行应存字节还是 32 位指令，取决于什么？
4. GTKWave 中要验证 `sw`，至少应观察哪些地址、数据和控制信号？

---

# 第八部分：常见错误、代码审查与调试

## 本章目标

能把 Verilator/Icarus 报错、失败断言和异常波形归因到位宽、类型、时序、数组、初始化或 TB 竞争问题，并能系统审查 AI 生成 CPU RTL。

**前置知识：**全书前七部分。  
**预计学习时间：**5–8 小时。

## 8.1 位宽、signed 与隐式转换

典型问题：

```systemverilog
logic [15:0] imm16;
logic [31:0] alu_b;
assign alu_b = imm16; // 能编译，但默认零扩展；对 addi/lw/sw/beq 通常错误。
```

修复不是压制 WIDTH warning，而是按 ISA 明确扩展：

```systemverilog
assign alu_b = {{16{imm16[15]}}, imm16};
```

另一个高频错误是混用 signed：

```systemverilog
assign less = (rs_data < $signed(rt_data)); // 混合类型，规则不直观。
```

应明确两边：`$signed(rs_data) < $signed(rt_data)`，或两边均 unsigned。审查时重点搜索：无尺寸常量、窄 RHS 赋给宽 LHS、拼接后算术、`>>>` 的左操作数、比较两边类型不一致。

## 8.2 阻塞/非阻塞、锁存器与多驱动

| 症状 | 根因 | 修复 |
|---|---|---|
| 流水线级跳过一级 | `always_ff` 中用 `=` | 时序状态用 `<=`，检查旧值传播 |
| 控制信号偶尔保留上条指令 | 组合块未完整赋值 | 块首默认值 + 完整分支 |
| 一个信号为 X 或工具报 MULTIDRIVEN | 多个过程/assign 驱动 | 合并为单一 MUX/单一驱动源 |
| 仿真不收敛或组合时序异常 | 组合反馈 | 沿组合依赖图找到无寄存器环并断开 |

不要用非阻塞赋值“修复”组合逻辑：它可能掩盖过程内依赖、增加 delta cycle，并使仿真/综合更难推理。组合逻辑优先 `always_comb` + `=`；状态逻辑优先 `always_ff` + `<=`。

## 8.3 数组、地址与初始化

检查数组时逐项回答：元素宽度是什么？元素数目是什么？索引是字节地址还是字地址？边界如何处理？读是组合还是同步？

```systemverilog
assign data = dmem[address[9:2]]; // 256 个 32 位字、字节地址模型。
```

`address[9:2]` 只有在数据存储器确为 256 个字、且仅支持对齐 word access 时才正确。AI 代码若将 `address[31:2]` 直接作为有限数组索引，可能发生越界；若没有 reset/初始化，读取到 X 是合理仿真结果而非“随机数据”。

## 8.4 Testbench 竞争与仿真/综合差异

“仿真通过”不等于“硬件正确”。危险来源包括：

- TB 在同一 `posedge` 驱动或采样 DUT，依赖调度竞态；
- 用 `initial/#delay` 实现本应存在的 RTL 行为；
- 依赖未初始化寄存器恰好为 0；
- 使用工具支持有限的语言特性，或忽略 lint warning；
- 存储器模型的组合读/同步读与目标 FPGA RAM/ASIC SRAM 不一致。

修复路径是：用边沿定义接口时序；明确 reset；对内存读写语义写测试；同时跑 lint 和功能测试；在必要时用综合/实现流程报告验证 RAM 推断与时序。

## 8.5 典型报错的阅读方法

工具措辞不同，但可按类别处理：

| 诊断类别 | 常见含义 | 首先检查 |
|---|---|---|
| WIDTH | 截断、扩展或操作数宽度异常 | 常量尺寸、拼接、赋值两侧宽度 |
| SIGNED | 有/无符号混用 | 比较、右移、算术中 `$signed`/`$unsigned` |
| LATCH | 组合过程有未赋值路径 | 默认值、`else`、`default` |
| MULTIDRIVEN | 多个驱动源 | `assign`、过程块、模块输出连接 |
| CASEINCOMPLETE/CASEOVERLAP | case 覆盖或掩码问题 | `default`、`casez` 模式、unique/priority 意图 |
| 未使用/未连接 | 端口或信号可能接错 | 顶层实例化、控制信号是否遗漏 |

不要只看报错行。先定位该信号的声明、所有驱动点和所有使用点，再依据硬件意图决定修复。`rg -n "signal_name" rtl tb` 是比盲目改代码更可靠的第一步。

## 8.6 AI 生成 CPU RTL 审查清单

拿到一段 AI RTL 时，按以下清单逐项确认：

1. **接口：**每个端口的位宽、方向、时钟/复位极性和地址单位是否明确？
2. **状态：**所有 PC、寄存器堆、流水线寄存器是否只在 `always_ff` 中更新？是否有 reset/enable/flush 语义？
3. **组合：**每个 `always_comb` 输出是否完整赋值？每个 `case` 是否有安全默认路径？
4. **类型：**立即数、比较、移位、拼接、加法进位是否显式处理宽度和 signed？
5. **控制：**非法指令、bubble、flush 时是否关闭 `reg_write`、`mem_write` 等副作用？
6. **寄存器堆：**`$zero` 是否不能写、总能读 0？读写同址语义是否被测试？
7. **存储器：**字节/字地址、对齐、读写时序、容量边界是否一致？
8. **流水线：**数据和控制是否一起寄存？forwarding/stall/flush 是否覆盖 load-use、branch 等冒险？
9. **验证：**每条已支持指令是否有正例、边界例和非法例？是否有波形和断言证据？
10. **工具：**Verilator lint 是否干净或每个 warning 都有已记录的理由？Icarus/目标综合工具是否验证过关键子集？

独立能力的标志不是“不再使用 AI”，而是能对 AI 的每项连接、每个控制位和每个时钟边沿提出可验证的问题，并能用代码、仿真和波形证明答案。

## 本章总结

- 先确认硬件意图，再处理工具警告；不要把 warning 当噪声。
- 位宽、符号、地址单位和赋值类型是 CPU RTL 最常见且最隐蔽的错误来源。
- 组合完整赋值、单驱动、时序 `<=`、明确 reset 是稳定 RTL 的底线。
- TB 采样时序和存储器模型必须明确，否则通过的仿真也可能没有证明目标硬件正确。
- 用审查清单把“AI 生成代码”转化为可逐项证伪和验证的设计假设。

## 核心语法速查表

| 看到的现象 | 优先动作 |
|---|---|
| WIDTH/SIGNED warning | 显式声明宽度、扩展或转换；验证数值边界 |
| LATCH warning | 给组合输出默认赋值，覆盖所有路径 |
| 同沿结果不稳定 | 分离 TB 驱动/采样与 DUT NBA 更新 |
| X 扩散 | 查 reset、初始化、数组越界和多驱动 |
| 存储器错位 | 重查地址单位、切片和对齐要求 |

## 自测题

1. 修复一段 `addi` 将 16 位立即数零扩展的 RTL，并给出 `imm16=16'hFF80` 的失败用例。
2. 给出一个会推断锁存器的控制单元片段，并改为安全默认值版本。
3. 当 `sw` 写错地址时，如何在 GTKWave 中区分“ALU 地址算错”和“地址切片算错”？
4. 用本章清单审查一个五级流水线的 flush 逻辑：至少列出三个必须被清除或屏蔽的副作用控制。

---

# 学习完成后的实践建议

完成本学习文档后，推荐按以下节奏把知识转为能力：

1. 为 PC、立即数扩展器、ALU、寄存器堆分别写独立 Testbench；每个模块先通过边界用例。
2. 以 `add`、`addi`、`lw`、`sw`、`beq`、`j` 为顺序逐条接入单周期顶层，并为每条指令建立最小 `.hex` 回归程序。
3. 每增加一条指令，先写指令语义表（输入、写回目标、ALU 操作、内存副作用、PC 选择），再实现 RTL。
4. 开启 Verilator 的 lint 警告；对每个 warning 修复、证明合理或记录原因。
5. 改造五级流水线前，先在单周期版本中明确每条控制信号的产生位置、使用位置与副作用；然后将其随数据通过流水线寄存器传递。

最终请始终用同一组问题阅读任何 RTL：它保存了什么状态？何时更新？组合输出依赖什么？位宽和 signed 是否明确？控制信号是否会产生意外副作用？仿真和波形是否证明了这些答案？
