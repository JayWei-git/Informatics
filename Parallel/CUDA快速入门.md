# CUDA 与 GPU 并行编程快速入门

适合已经学过基本算法、数据结构和计算机系统，但尚未接触 CUDA 的读者；目标是先运行并理解一个 CUDA 程序，再逐步学习性能优化；如果你的目标是用 C++ 实现神经网络算子，可在完成本文后继续阅读 [C/C++ 与 CUDA 学习路线](WhatYouNeedDo.md)；

## 先建立四个概念

1. **CPU 是 host，GPU 是 device**；通常由 CPU 分配数据、启动 GPU 函数，并在需要结果时等待或复制数据；GPU 适合把大量相似计算分给许多线程，任务太小或频繁传输数据时，GPU 未必更快；
2. **kernel 是在 GPU 上执行的函数**；一次启动会产生一个 grid，grid 由多个 block 组成，每个 block 又有多个 thread；起步时把一个线程对应到一个数组元素即可；
3. **线程用索引确定自己的工作**；一维场景最常见的写法是 `int i = blockIdx.x * blockDim.x + threadIdx.x`，并在访问数组前检查 `i < n`；这样既能覆盖任意长度，也能处理最后一个不满的 block；
4. **数据位置与同步决定正确性和速度**；先区分 CPU 内存与 GPU 全局内存，理解拷贝方向和 kernel 启动的异步性；之后再学习 block 内共享内存、`__syncthreads()`、访存合并和 warp；

此时无需先掌握 Tensor Core、PTX、多 GPU、复杂的流调度或全部 GPU 硬件细节；每遇到一个具体性能问题再深入对应主题；

## 准备环境

需要一块支持 CUDA 的 NVIDIA GPU、可用的驱动和 CUDA Toolkit；先运行 `nvidia-smi` 检查驱动是否能看到 GPU，再运行 `nvcc --version` 检查 CUDA 编译器是否可用；`nvidia-smi` 显示的 CUDA 版本代表驱动支持的最高 CUDA 版本，并不等于已经安装了对应版本的 Toolkit；安装步骤依操作系统和发行版而异，使用 NVIDIA 的 [CUDA Quick Start Guide](https://docs.nvidia.com/cuda/cuda-quick-start-guide/index.html) 选择对应平台的步骤；

如果使用 WSL，按官方 WSL 安装说明处理驱动与 Toolkit；不要在 WSL 发行版内另装 Linux GPU 驱动；

### Windows 原生环境安装

以下步骤适用于直接在 Windows 10 或 Windows 11 上编译 CUDA C++，命令在 Windows 的 `cmd` 中执行；安装前对照 [NVIDIA Windows 安装指南](https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html) 确认你的 Windows、GPU 与 Visual Studio 版本受所选 Toolkit 支持；

1. **确认 GPU**；打开「设备管理器 → 显示适配器」，找到 NVIDIA 显卡型号，并在 [CUDA GPU 列表](https://developer.nvidia.com/cuda-gpus) 中确认支持情况；也可以先在 `cmd` 中运行 `nvidia-smi`，确认系统能够识别显卡；
2. **安装 NVIDIA 驱动**；从 [NVIDIA 驱动下载页](https://www.nvidia.com/drivers) 获取适合该显卡和 Windows 版本的驱动，按安装程序提示完成并在需要时重启；当前 Windows CUDA Toolkit 安装包不再附带 GPU 驱动，因此驱动与 Toolkit 都要准备好；
3. **安装 C++ 编译工具**；通过 [Visual Studio 下载页](https://visualstudio.microsoft.com/downloads/) 安装受所选 CUDA 版本支持的 Visual Studio，例如当前 NVIDIA 文档列出的 Visual Studio 2022；在安装器中勾选 **Desktop development with C++／使用 C++ 的桌面开发** 工作负载，确保装上 MSVC 编译器与 Windows SDK；已有 Visual Studio 的话，可在 Visual Studio Installer 中点「修改」补装该工作负载；
4. **安装 CUDA Toolkit**；在 [CUDA Toolkit 下载页](https://developer.nvidia.com/cuda-downloads) 依次选择 Windows、机器架构和系统版本，下载 Network Installer 或 Local Installer；运行安装程序并按提示安装；Network Installer 安装时联网下载组件，Local Installer 预先包含组件；
5. **打开新的命令行并验证**；从开始菜单打开 `x64 Native Tools Command Prompt for VS 2022`，或打开与你安装的 Visual Studio 版本对应的 x64 工具命令提示符，依次运行下面三条命令；`nvidia-smi` 应显示显卡，`cl` 应显示 MSVC 编译器信息，`nvcc --version` 应显示 CUDA 编译器版本；

```bat
nvidia-smi
cl
nvcc --version
```

验证后，把下一节的代码保存到例如 `C:\cuda-start\vector_add.cu`，在同一个 x64 工具命令提示符中运行以下命令；看到 `PASS: 1000 elements` 就完成了第一个 CUDA 程序；

```bat
cd /d C:\cuda-start
nvcc -std=c++17 -O2 vector_add.cu -o vector_add.exe
vector_add.exe
```

如果 `cl` 无法识别，检查是否安装了 C++ 工作负载以及是否打开了 x64 工具命令提示符；如果 `nvcc` 无法识别，重新打开命令提示符并运行 `echo %CUDA_PATH%`，核对 Toolkit 是否已安装，以及 `%CUDA_PATH%\bin\nvcc.exe` 是否存在；如果编译成功但运行时找不到 CUDA 设备，先检查 `nvidia-smi`、驱动安装和显卡支持情况；需要更完整的硬件验证时，可按 [Windows 安装指南的验证章节](https://docs.nvidia.com/cuda/cuda-installation-guide-microsoft-windows/index.html#verify-the-installation) 构建并运行官方 `deviceQuery` 示例；

## 第一个可运行程序：向量加法

把下面代码保存为 `vector_add.cu`；在 Linux 或 WSL 中执行 `nvcc -std=c++17 -O2 vector_add.cu -o vector_add && ./vector_add`，在 Windows 中使用上一节的命令；这个例子让每个 GPU 线程计算一个 `c[i] = a[i] + b[i]`，并用 CPU 检查结果；

```cpp
#include <cuda_runtime.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#define CHECK(call)                                                        \
    do {                                                                   \
        cudaError_t error = (call);                                        \
        if (error != cudaSuccess) {                                        \
            std::cerr << #call << ": " << cudaGetErrorString(error) << '\n'; \
            std::exit(EXIT_FAILURE);                                       \
        }                                                                  \
    } while (0)

__global__ void vector_add(const float* a, const float* b, float* c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) c[i] = a[i] + b[i];
}

int main() {
    constexpr int n = 1000;
    const size_t bytes = n * sizeof(float);
    std::vector<float> a(n), b(n), c(n);
    for (int i = 0; i < n; ++i) {
        a[i] = static_cast<float>(i);
        b[i] = static_cast<float>(2 * i);
    }

    float *d_a = nullptr, *d_b = nullptr, *d_c = nullptr;
    CHECK(cudaMalloc(reinterpret_cast<void**>(&d_a), bytes));
    CHECK(cudaMalloc(reinterpret_cast<void**>(&d_b), bytes));
    CHECK(cudaMalloc(reinterpret_cast<void**>(&d_c), bytes));
    CHECK(cudaMemcpy(d_a, a.data(), bytes, cudaMemcpyHostToDevice));
    CHECK(cudaMemcpy(d_b, b.data(), bytes, cudaMemcpyHostToDevice));

    constexpr int block_size = 256;
    const int grid_size = (n + block_size - 1) / block_size;
    vector_add<<<grid_size, block_size>>>(d_a, d_b, d_c, n);
    CHECK(cudaGetLastError());
    CHECK(cudaDeviceSynchronize());

    CHECK(cudaMemcpy(c.data(), d_c, bytes, cudaMemcpyDeviceToHost));
    for (int i = 0; i < n; ++i) {
        if (c[i] != a[i] + b[i]) {
            std::cerr << "Mismatch at " << i << '\n';
            return EXIT_FAILURE;
        }
    }

    CHECK(cudaFree(d_a));
    CHECK(cudaFree(d_b));
    CHECK(cudaFree(d_c));
    std::cout << "PASS: " << n << " elements\n";
}
```

读代码时按这条链路追踪：CPU 初始化数组 → GPU 分配内存 → CPU 向 GPU 拷贝 → 启动 kernel → 等待并检查错误 → GPU 向 CPU 拷贝 → 校验；`<<<grid_size, block_size>>>` 指定 block 数和每个 block 的线程数，`cudaGetLastError()` 检查启动错误，`cudaDeviceSynchronize()` 等待执行结束并暴露执行错误；

## 一周练习顺序

| 顺序 | 动手任务 | 确认自己学会了什么 |
| --- | --- | --- |
| 1 | 把 `n` 改成 `1`、`255`、`256`、`257`、`1000000`，并改动 `block_size`； | 能解释 grid 大小的计算，以及边界检查为何必要； |
| 2 | 把向量加法改成逐元素平方、逐元素最大值； | 能独立写一维索引和 CPU 正确性对照； |
| 3 | 写矩阵加法，再写每线程计算一个输出元素的朴素矩阵乘法； | 能把二维下标映射到行主序数组，并处理非整除尺寸； |
| 4 | 写求和归约或矩阵转置； | 开始理解线程之间何时需要协作、同步或重新安排内存访问； |
| 5 | 用 CUDA event 分别测量 kernel 时间，并记录完整的传输加计算时间； | 能分清 GPU 计算时间和整个程序的实际耗时； |

每个练习都保留一个简单 CPU 参考实现；先比较结果，再测性能；对浮点归约和矩阵乘法使用合理的误差容限，因为计算顺序变化可能导致舍入结果不同；不要把 `std::chrono` 包在一次异步 kernel 启动外就当作 GPU 执行时间；

## 遇到问题时查什么

- **编译、安装或驱动问题**：查 [CUDA Quick Start Guide](https://docs.nvidia.com/cuda/cuda-quick-start-guide/index.html)；
- **线程索引、grid/block、kernel 启动和同步**：查 [CUDA Programming Guide 的基础章节](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/intro-to-cuda-cpp.html)；
- **越界、竞争和未初始化读取**：用 [Compute Sanitizer](https://docs.nvidia.com/compute-sanitizer/ComputeSanitizer/index.html) 检查；
- **性能优化**：先看 [CUDA Best Practices Guide](https://docs.nvidia.com/cuda/cuda-c-best-practices-guide/index.html) 中的全局内存访问，再用 Nsight Systems 看整体时间线、Nsight Compute 看单个 kernel；

最重要的节奏是：写一个最简单的正确版本，覆盖边界输入，与 CPU 对照，然后测量，再针对测出的瓶颈优化；
