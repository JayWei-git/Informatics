# C++ Neural Networks: Zero to Hero

这是一个配合 Andrej Karpathy 的 **Neural Networks: Zero to Hero** 系列课程进行的 C++ 学习项目。

目标是在理解课程内容的同时，使用 C++ 从零实现神经网络所需的核心组件，而不是只停留在调用 PyTorch 等现成框架的层面。

## 对课程内容的理解

这套课程会带你实现一个很小的“类 PyTorch”核心，但不会从零实现完整的 PyTorch。

- 第一讲 `micrograd`：从零实现标量计算图、反向传播和简单神经网络，确实类似 PyTorch 的自动求导核心。
- 后续 `makemore`：开始大量使用 `torch.Tensor`，但会深入讲张量、梯度和 BatchNorm，并有一讲手动推导反向传播。
- GPT：所谓 “from scratch” 是自己搭建 Transformer/GPT 架构，但底层张量运算、自动求导等仍由 PyTorch 完成。
- Tokenizer：会从零实现 BPE tokenizer。

课程资料：[官方课程仓库](https://github.com/karpathy/nn-zero-to-hero)

因此，本项目采用“双轨制”：不完全抛弃课程里的 Python，而是将 Python 当作课程讲解语言、数学公式的可执行伪代码，以及 C++ 实现的正确性参考；每学完一部分，再使用 C++ 独立复现。

## 学习路线

### 第一阶段：C++ 版 micrograd

首先实现标量自动求导：

```cpp
struct Value {
    double data;
    double grad;

    std::vector<std::shared_ptr<Value>> parents;
    std::function<void()> backward;
};
```

需要支持：

- `+`、`-`、`*`、`/`
- `pow`
- `exp`
- `tanh`
- `relu`
- 计算图的拓扑排序
- 反向传播
- `Neuron`
- `Layer`
- `MLP`
- SGD

这部分几乎可以与第一讲同步完成，而且非常适合 C++。它还会涉及：

- 对象生命周期
- `shared_ptr` / `weak_ptr`
- 运算符重载
- lambda 捕获
- 计算图的数据结构

### 第二阶段：自己实现 Tensor

不要一开始就试图复刻整个 PyTorch。先实现一个 CPU、连续内存、只支持 `float` 或 `double` 的 Tensor：

```cpp
class Tensor {
public:
    std::vector<float> data;
    std::vector<std::size_t> shape;
    std::vector<std::size_t> strides;
};
```

第一批功能只做：

- 多维索引
- `reshape`
- `transpose`
- broadcasting
- 逐元素加减乘除
- 矩阵乘法
- `sum`、`mean`、`max`
- `exp`、`log`、`tanh`、`relu`
- 随机初始化

第一版先不要涉及：

- CUDA
- 多线程
- SIMD
- 稀疏张量
- 混合精度
- 分布式训练

这些内容会过早地把“学习神经网络”变成“调试系统工程”。

### 第三阶段：张量级自动求导

把 micrograd 从标量推广到 Tensor：

```text
Tensor
 ├── data
 ├── grad
 ├── parents
 ├── requires_grad
 └── backward_fn
```

至少支持以下算子的反向传播：

- element-wise operations
- broadcasting
- matrix multiplication
- reduction
- reshape / transpose
- embedding lookup
- ReLU / tanh
- softmax
- cross entropy

这一步完成后，就拥有了一个迷你版 PyTorch。

### 第四阶段：神经网络组件

继续实现：

- `Parameter`
- `Module`
- `Linear`
- `Embedding`
- `BatchNorm`
- `LayerNorm`
- `Dropout`
- `Sequential`
- `SGD`
- `Adam`

然后使用自己的框架复现课程模型：

```text
bigram
  ↓
MLP
  ↓
deeper MLP + BatchNorm
  ↓
WaveNet
  ↓
Transformer / GPT
```

## 推荐的实现循环

不要逐行把 Python 翻译成 C++，而是采用以下循环：

1. 用课程的 Python 代码理解数学和算法。
2. 合上代码，设计自己的 C++ 接口。
3. 用小输入手算，或使用 PyTorch 生成参考结果。
4. 对比前向计算结果。
5. 用有限差分检查梯度。
6. 最后再训练完整模型。

梯度检查尤其重要：

$$
\frac{\partial L}{\partial x}
\approx
\frac{L(x+\epsilon)-L(x-\epsilon)}{2\epsilon}
$$

否则，C++ 反向传播中一个索引或 broadcasting 错误，可能只会表现为模型“训练得不好”，很难定位。

## 学习比例

建议采用以下比例：

- 20%：运行和阅读课程里的 Python
- 80%：使用 C++ 重新实现

“从零实现”不等于拒绝所有参考实现。PyTorch 可以作为测试 oracle。只要最终训练模型使用的是自己的 C++ Tensor、autograd 和神经网络模块，就已经是相当扎实的从零实现。

## 第一版的范围

第一版限定为：

> C++17、CPU-only、单线程、连续内存、float32、动态图自动求导。

做到能够使用自己的框架训练 `makemore` 后，再决定是否加入线程池、SIMD 或 CUDA。这个边界既足够深入，又不至于在学到 GPT 之前先花半年复刻内存分配器。

