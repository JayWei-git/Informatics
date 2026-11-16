

# C/C++ 与 CUDA 学习路线

目标：配合 Andrej Karpathy 的 [Neural Networks: Zero to Hero](https://karpathy.ai/zero-to-hero.html)，用 C++ 理解张量、自动求导和神经网络的计算，再用 CUDA C++ 手写并优化关键算子；本路线按已有可用 GPU 安排实作；

## 先启动项目

先完成一个 **C++ Tensor + CPU MatMul**，然后写第一个 **CUDA MatMul kernel**；暂时不申请超算、不买树莓派、不碰分布式训练，也不直接读 rocFFT；

建立一个小型仓库，保存 CPU 参考实现、CUDA 实现、正确性检查和性能记录；每推进一个算子，都按「CPU 参考实现 → CUDA 朴素实现 → 校验 → 测量 → 优化」的顺序进行；课程中的 Python 代码可用来理解算法和验证结果，主体实现放在 C++ 与 CUDA C++；

## 教材与课程怎样配合

目录根部的《CUDA C编程权威指南》用于理解 CUDA 编程、执行与内存模型；`Books/` 下的《Programming Massively Parallel Processors》第四版（PMPP）作为更适合逐步做题的配套教材；两本书遇到重复概念时，选一本精读，另一本查漏补缺；

| 阶段 | Zero to Hero | 阅读范围 | 手写成果与完成标准 |
| --- | --- | --- | --- |
| 1：建立 CPU 基线 | micrograd | 《CUDA C编程权威指南》第 1 章；PMPP 第 1 章 | 用 C++ 写标量计算图和反向传播；写连续存储的二维 Tensor、形状与索引、CPU MatMul；用有限差分抽查梯度； |
| 2：写第一个 kernel | makemore 的 bigram 与 MLP 部分 | 《CUDA C编程权威指南》第 2 章；PMPP 第 2–3 章 | 写向量加法与朴素 CUDA MatMul；由线程索引推导输出元素位置；逐项对照 CPU 结果，并覆盖非整块尺寸与小矩阵； |
| 3：理解执行与内存 | 继续用 MLP 的线性层做输入 | 《CUDA C编程权威指南》第 3–5 章；PMPP 第 4–5 章 | 为 MatMul 加共享内存分块；再写一次归约或矩阵转置；比较访存方式、块大小和同步方式的影响； |
| 4：拼出小模型 | GPT 课程 | 按问题回查前述章节；需要时读 PMPP 的归约与 scan 章节 | 用已有 Tensor 和算子完成小规模 `Linear → Softmax → Attention` 前向计算；先将反向传播保留在 CPU； |

每阶段只在正确性通过后测性能；至少记录输入形状、数据类型、GPU 型号、block 与 tile 参数、计时方式及结果；浮点结果按合理容差比较，另测小尺寸、非整除尺寸和随机输入；

## 第一个 GPU 里程碑：MatMul

1. 写 CPU 三重循环 MatMul，明确矩阵的行主序布局与 `M × K`、`K × N`、`M × N` 形状；
2. 写朴素 CUDA kernel，让每个线程计算一个输出元素，先处理边界，再确认结果正确；
3. 写共享内存分块版本，明确每块加载的数据、同步位置和边界块的填充值；
4. 先自行实现，再对照 [siboehm/SGEMM_CUDA](https://github.com/siboehm/SGEMM_CUDA) 的 Kernel 1（Naive）与 Kernel 3（Shared Memory Caching），解释两版各自的全局内存读取和数据复用；
5. 用 CUDA event 测 kernel 时间，单独记录数据传输与端到端时间；用 [Compute Sanitizer](https://docs.nvidia.com/compute-sanitizer/ComputeSanitizer/index.html) 检查越界、未初始化读取和共享内存竞争；

性能优化以测量结果为准；《CUDA C编程权威指南》中的 `nvprof` 与 `nvvp` 属于旧工具，实践时使用 Nsight Systems 观察时间线、Nsight Compute 分析单个 kernel；流、加速库和多 GPU 等内容，等单 GPU MatMul 做通后再按实际项目需要学习；

## 极简书单

1. **《Programming Massively Parallel Processors》（PMPP，胡文美等著）**：先学第 1 章至第 5 章，配合上述四阶段动手；
2. **《CUDA C编程权威指南》**：先学第 1 章至第 5 章，把线程组织、执行模型、全局内存与共享内存用到 MatMul；
3. **开源项目 `siboehm/SGEMM_CUDA`**：先对照 Kernel 1 与 Kernel 3，完成自己的版本后再看后续优化；
