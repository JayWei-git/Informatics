# 基础多元线性回归（C++ + Python）

这是一个为了学习而写的、故意保持简单的多元线性回归示例。C++ 不依赖矩阵库，使用批量梯度下降逐项计算：

```text
y_hat = bias + w0*x0 + w1*x1 + ... + wn*xn
```

每一轮的目标函数是 half-MSE：

```text
loss = 1/(2N) * sum((y_hat - y)^2)
```

## 编译和运行

在项目根目录执行：

```powershell
cmake -S . -B build-local -G Ninja
cmake --build build-local
.\build-local\bin\lr_demo.exe data\sample.json build-local\results
```

也可以省略命令行参数，此时默认读取 `data/sample.json`，输出到 `build/results`。

程序会尝试三个学习率，并输出：

- `loss_history.csv`：每个学习率、每个 epoch 的 loss；
- `predictions.csv`：每个样本的真实值和不同学习率下的预测值。

## Python 可视化

需要 `pandas` 和 `matplotlib`：

```powershell
python -m pip install pandas matplotlib
python scripts/visualize.py build-local/results
```

脚本会显示并保存 `training_visualization.png`，左图比较不同学习率的 loss 下降，右图比较最终预测值。

## 学习时可以重点观察

1. `fit` 中的 `error` 如何累计出 bias 和每个权重的梯度。
2. 学习率过小时 loss 下降慢，过大时可能震荡或发散。
3. 当前实现没有做特征标准化；如果特征量纲差异很大，学习率会更难选择。
4. 这里使用的是训练集上的预测，只用于演示算法，不代表模型泛化能力。
