# Machine Learning

- 应用最广泛的机器学习方式：和强化学习；

##### Supervised Learning

- **形式化定义**：训练数据集提供输入特征变量及其对应的真实标签（Label / Ground Truth）。

  $$\mathcal{D} = \left\{ \left( \mathbf{x}^{(i)}, y^{(i)} \right) \right\}_{i=1}^{m}$$

- **优化目标**：在给定的假设空间 $\mathcal{H}$ 中学习一个映射函数 $f: \mathcal{X} \to \mathcal{Y}$，使预测值 $\hat{y} = f(\mathbf{x})$ 与真实标签 $y$ 的全局误差最小化。

- **任务分支划分依据**：取决于目标变量（Label）$y$ 的取值类型，而非结果集合的大小或取值是否有限。

  1. **回归（Regression）**：
     - 目标变量 $y \in \mathbb{R}$，取值空间为**连续数值**。
     - 目标：预测一个连续的标量输出。
     - 典型示例：基于房屋面积、房间数量与位置特征预测房屋成交价格；基于传感器读数预测环境温度。
  2. **分类（Classification）**：
     - 目标变量 $y \in \{c_1, c_2, \dots, c_k\}$，取值空间为**离散类别标签**。
     - 目标：将样本空间划分至给定的有限离散集合中。
     - 典型示例：二分类任务（如判断邮件是否为垃圾邮件 $y \in \{0, 1\}$）；多分类任务（如手写数字识别 $y \in \{0, 1, \dots, 9\}$）。

##### Unsupervised Learning

- **形式化定义**：训练数据集仅包含输入特征变量，不存在对应的目标标签或外部反馈信号。

  $$\mathcal{D} = \left\{ \mathbf{x}^{(i)} \right\}_{i=1}^{m}$$

- **优化目标**：算法不依赖预设标签，直接从数据本身的几何分布、拓扑结构或统计特征中提取潜在模式。

- **主要任务类型**：

  1. **聚类（Clustering）**：根据样本间的距离度量（如欧氏距离、曼哈顿距离）将无标签样本集自动划分为若干紧密子集。
  2. **降维（Dimensionality Reduction）**：在保留数据主要方差或拓扑流形的前提下，压缩特征空间的维度（如主成分分析 PCA）。
  3. **异常检测（Anomaly Detection）**：通过建立正常样本的概率分布模型 $p(\mathbf{x})$，筛选低概率密度区域的离群点。

##### Comparison of Learning Paradigms

| **特征维度**     | **监督学习 (Supervised Learning)**                           | **非监督学习 (Unsupervised Learning)**                       |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **训练数据形式** | $\mathcal{D} = \{(\mathbf{x}^{(i)}, y^{(i)})\}_{i=1}^m$（包含输入与标签） | $\mathcal{D} = \{\mathbf{x}^{(i)}\}_{i=1}^m$（仅输入，无标签） |
| **映射目标**     | 求解映射函数 $f: \mathbf{x} \mapsto y$                       | 挖掘底层数据分布结构 $p(\mathbf{x})$ 或集合划分              |
| **核心反馈机制** | 预测输出与监督标签计算显式残差                               | 无显式外部监督信号，依赖内在目标准则优化                     |
| **典型算法范式** | 线性回归、对数几率回归、支持向量机、决策树                   | K-Means 聚类、高斯混合模型 (GMM)、PCA                        |

**End of 1.1.**

#### 1.2. Mathematical Notations and Conventions

##### Indexing and Variable Conventions

- **样本维度索引（圆括号上标）**：表示样本在训练集合中的序号。
  - $\mathbf{x}^{(i)}$：第 $i$ 个训练样本的输入特征向量（若特征为单标量，写作 $x^{(i)}$）。
  - $y^{(i)}$：第 $i$ 个训练样本的真实观测标签。
  - $\hat{y}^{(i)}$：模型对第 $i$ 个样本的输出预测值（即 $\hat{y}^{(i)} = f(\mathbf{x}^{(i)})$）。
- **特征维度索引（无括号下标）**：表示特征向量中的分量序号。
  - $x_j$：特征向量中的第 $j$ 维特征分量。
  - $x_j^{(i)}$：第 $i$ 个样本中的第 $j$ 个特征值。
- **层级索引（方括号上标，进阶深度网络）**：
  - $\mathbf{W}^{[l]}, \mathbf{b}^{[l]}$：多层结构中第 $l$ 层的权重矩阵与偏置向量。
  - $\mathbf{a}^{[l]}$：第 $l$ 层的激活值输出向量。

##### Dimensionality and Matrix Representation

- $m$：训练样本总量（$m = \vert{}\mathcal{D}\vert{}$）。

- $n$：输入特征的维度数量。

- **设计矩阵（Design Matrix, $X$）**：由全体样本的特征向量按行堆叠而成的实数矩阵，维度为 $m \times n$：

  $$X = \begin{bmatrix} (\mathbf{x}^{(1)})^T \\ (\mathbf{x}^{(2)})^T \\ \vdots \\ (\mathbf{x}^{(m)})^T \end{bmatrix} = \begin{bmatrix} x_1^{(1)} & x_2^{(1)} & \dots & x_n^{(1)} \\ x_1^{(2)} & x_2^{(2)} & \dots & x_n^{(2)} \\ \vdots & \vdots & \ddots & \vdots \\ x_1^{(m)} & x_2^{(m)} & \dots & x_n^{(m)} \end{bmatrix} \in \mathbb{R}^{m \times n}$$

- **目标向量（Target Vector, $\mathbf{y}$）**：所有样本真实标签构成的列向量，维度为 $m \times 1$：

  $$\mathbf{y} = \begin{bmatrix} y^{(1)} \\ y^{(2)} \\ \vdots \\ y^{(m)} \end{bmatrix} \in \mathbb{R}^m$$

**End of 1.2.**

## Part Two: Linear Regression Model

### 2. Univariate Linear Regression (单变量线性回归)

#### 2.1. Model Representation & Pipeline

##### Supervised Learning Pipeline

```
  Training Set: {(x^(i), y^(i))}
                │
                ▼
      ┌──────────────────┐
      │Learning Algorithm│
      └──────────────────┘
                │
                ▼
           Hypothesis
          f: x ────► y^ (Prediction)
```

1. **输入阶段**：算法接收结构化训练集 $\mathcal{D} = \{(x^{(i)}, y^{(i)})\}_{i=1}^m$；
2. **训练阶段**：学习算法在特定的假设空间（Hypothesis Space）中搜索最优参数组合；
3. **推断阶段**：导出模型函数 $f$，对于给定的任意新特征输入 $x$，计算其对应的估计值 $\hat{y} = f(x)$。

##### Univariate Linear Model Formulation

- **模型定义**：当输入特征维度 $n = 1$ 时，假设函数 $f$ 定义为输入变量 $x$ 的一阶仿射变换：

  $$f_{w, b}(x) = wx + b$$

- **模型参数（Parameters）**：

  - **$w$（Weight / 权重）**：直线的斜率，决定特征 $x$ 变化时预测值 $\hat{y}$ 的变化率。
  - **$b$（Bias / 偏置）**：直线的截距项，决定当 $x = 0$ 时预测值的基准位移，避免直线强制穿过原点。

- **几何含义**：在二维欧氏空间 $\mathbb{R}^2$ 中寻找一条拟合直线，使所有样本坐标点 $(x^{(i)}, y^{(i)})$ 到该直线的竖直距离误差整体最小。

- **假设空间（Hypothesis Space, $\mathcal{H}$）**：

  单变量线性回归约束其可选择的函数族为实数域上的线性一元多项式集合：

  $$\mathcal{H} = \left\{ f_{w, b} : \mathbb{R} \to \mathbb{R} \;\middle\vert{}\; f_{w, b}(x) = wx + b,\; w \in \mathbb{R},\; b \in \mathbb{R} \right\}$$

**End of 2.1.**

#### 2.2. Cost Function (代价函数)

##### Mathematical Formulation

为了确定最优参数对 $(w, b)$，必须定义量化预测值 $\hat{y}^{(i)} = f_{w,b}(x^{(i)})$ 与真实值 $y^{(i)}$ 之间差异的标量函数。

- **单个样本的误差损失（Squared Error Loss）**：

  $$L\left( f_{w,b}(x^{(i)}), y^{(i)} \right) = \left( \hat{y}^{(i)} - y^{(i)} \right)^2 = \left( wx^{(i)} + b - y^{(i)} \right)^2$$

- **全数据集代价函数（Mean Squared Error, MSE Cost Function）**：

  $$J(w, b) = \frac{1}{2m} \sum_{i=1}^{m} \left( f_{w, b}(x^{(i)}) - y^{(i)} \right)^2 = \frac{1}{2m} \sum_{i=1}^{m} \left( wx^{(i)} + b - y^{(i)} \right)^2$$

- **数学常数因子的设计原因**：

  - **分母 $m$**：对样本集整体误差求算术平均，消除代价数值对训练集样本容量 $m$ 的规模依赖，使得不同大小数据集上的代价评估标准具有可比性。
  - **分母 $2$**：在后续对参数求一阶偏导数时，二次幂项求导产生的系数 $2$ 与分母常数 $\frac{1}{2}$ 刚好对消（$\frac{\partial}{\partial u} \left(\frac{1}{2} u^2\right) = u$），从而简化解析导数表达。

- **参数优化数学目标**：

  $$(w^*, b^*) = \arg\min_{w, b} J(w, b)$$

  即寻找一对全局最优解 $(w^*, b^*)$，使得曲面 $J(w, b)$ 的高度达到全局最小值。

**End of 2.2.**

#### 2.3. Cost Function Geometry and Properties

##### Geometric Interpretation: Simplified Case ($b = 0$)

当假设偏置参数 $b = 0$ 时，模型退化为穿过原点的正比例函数 $f_w(x) = wx$，代价函数简化为关于单一标量 $w$ 的一元二次多项式：

$$J(w) = \frac{1}{2m} \sum_{i=1}^{m} \left( w x^{(i)} - y^{(i)} \right)^2$$

展开代数式：

$$J(w) = \frac{1}{2m} \left[ \left( \sum_{i=1}^m (x^{(i)})^2 \right) w^2 - 2 \left( \sum_{i=1}^m x^{(i)} y^{(i)} \right) w + \sum_{i=1}^m (y^{(i)})^2 \right]$$

- 几何形态：在二维坐标系 $(w, J(w))$ 中，该函数是一条**开口向上的单峰抛物线**。
- 最优性质：存在唯一的对称轴极小值点 $w^*$，满足一阶导数 $\frac{\mathrm{d}J}{\mathrm{d}w} = 0$。

##### Geometric Interpretation: General Case ($w$ and $b$)

当保留两个参数 $w$ 与 $b$ 时，代价函数 $J(w, b)$ 投影在三维欧氏空间中：

1. **三维空间曲面（3D Surface Plot）**：
   - 参数 $(w, b)$ 构成水平底面的二维正交坐标系，垂直轴表示标量代价值 $J(w, b)$。
   - $J(w, b)$ 的三维空间几何曲面为**椭圆抛物面（Elliptic Paraboloid）**。
   - 该曲面具备全局单极小值几何特征，向外侧非负递增发散。
2. **二维等高线图（Contour Plot）**：
   - 使用一系列水平切平面 $J(w, b) = C$（$C$ 为常数）截取该抛物面，并投影至 $(w, b)$ 平面上。
   - 截线表现为**同心椭圆族**。
   - 椭圆长短轴由特征分布的二阶矩矩阵决定。
   - 处于同一椭圆轨迹上的所有参数点 $(w, b)$ 具有完全相同的代价数值 $J$；椭圆环带中心即为全局唯一的极小值驻点 $(w^*, b^*)$。

##### Convexity and Global Minimum (严格凸性保证)

均方误差代价函数 $J(w, b)$ 关于参数集具有优良的优化性质：

- **凸函数（Convex Function）判定**：

  函数定义域内任意两点 $\boldsymbol{\theta}_1, \boldsymbol{\theta}_2 \in \mathbb{R}^2$ 及常数 $\lambda \in [0, 1]$，恒满足割线位于函数曲面上方的凸性不等式：

  $$J(\lambda \boldsymbol{\theta}_1 + (1 - \lambda)\boldsymbol{\theta}_2) \le \lambda J(\boldsymbol{\theta}_1) + (1 - \lambda)J(\boldsymbol{\theta}_2)$$

- **二阶 Hessian 矩阵半正定性**：

  构建 $J(w, b)$ 关于参数 $(w, b)$ 的二阶偏导数矩阵（Hessian Matrix）：

  $$H = \begin{bmatrix} \frac{\partial^2 J}{\partial w^2} & \frac{\partial^2 J}{\partial w \partial b} \\ \frac{\partial^2 J}{\partial b \partial w} & \frac{\partial^2 J}{\partial b^2} \end{bmatrix} = \frac{1}{m} \begin{bmatrix} \sum_{i=1}^m (x^{(i)})^2 & \sum_{i=1}^m x^{(i)} \\ \sum_{i=1}^m x^{(i)} & m \end{bmatrix}$$

  根据柯西-施瓦茨不等式（Cauchy-Schwarz Inequality），其顺序主子式满足：

  $$\det(H) = \frac{1}{m^2} \left[ m \sum_{i=1}^m (x^{(i)})^2 - \left( \sum_{i=1}^m x^{(i)} \right)^2 \right] \ge 0$$

  若训练样本中的特征取值 $x^{(i)}$ 不全相同，则 $\det(H) > 0$，Hessian 矩阵处处**严格正定**。

- **优化理论推论**：

  严格凸函数的局部极小值点必然等价于全局最优解，且曲面上**不存在任何非全局的局部极小值（Local Minima）或鞍点（Saddle Points）**。任何有效的一阶下降方法或解析求解均可稳定收敛至全局最优参数。

**End of 2.3.**

### 3. Multivariate Linear Regression Formulation (多变量线性回归表述)

#### 3.1. Vectorized Representation of Hypothesis & Cost

##### Model Formulation with Multiple Features

当每个样本包含 $n$ 维特征属性时：

- **特征向量**：$\mathbf{x} = [x_1, x_2, \dots, x_n]^T \in \mathbb{R}^n$。
- **权重向量**：$\mathbf{w} = [w_1, w_2, \dots, w_n]^T \in \mathbb{R}^n$。
- **标量偏置**：$b \in \mathbb{R}$。

假设函数从二维平面直线拓展为 $n$ 维特征空间中的仿射**超平面（Hyperplane）**：

$$f_{\mathbf{w}, b}(\mathbf{x}) = \mathbf{w}^T \mathbf{x} + b = \sum_{j=1}^{n} w_j x_j + b$$

##### Vectorized Cost Function

利用设计矩阵 $X \in \mathbb{R}^{m \times n}$、参数列向量 $\mathbf{w} \in \mathbb{R}^n$ 与全 1 列向量 $\mathbf{1}_m \in \mathbb{R}^m$，可将全体样本的预测表达式写为紧凑的矩阵向量乘法：

$$\hat{\mathbf{y}} = X \mathbf{w} + b \mathbf{1}_m$$

代价函数 $J(\mathbf{w}, b)$ 转换为残差向量的二范数平方形式：

$$J(\mathbf{w}, b) = \frac{1}{2m} \Vert{} X \mathbf{w} + b \mathbf{1}_m - \mathbf{y} \Vert{}_2^2 = \frac{1}{2m} (X \mathbf{w} + b \mathbf{1}_m - \mathbf{y})^T (X \mathbf{w} + b \mathbf{1}_m - \mathbf{y})$$

> 注：如何系统性地求解使 $J(\mathbf{w}, b)$ 最小化的参数极值点 $(\mathbf{w}^*, b^*)$？工程上最广泛采用的数值优化算法为**梯度下降法（Gradient Descent）**，该算法的核心迭代规则与性质将在下一章（Part Three）深入展开。

**End of 3.1.**

## Appendix

### Appendix: A

#### Hessian Matrix Derivation for Univariate Linear Regression

设单变量线性回归均方误差目标函数为：

$$J(w, b) = \frac{1}{2m} \sum_{i=1}^m \left( wx^{(i)} + b - y^{(i)} \right)^2$$

1. **一阶偏导数计算**：

   $$\frac{\partial J}{\partial w} = \frac{1}{m} \sum_{i=1}^m \left( wx^{(i)} + b - y^{(i)} \right) x^{(i)}$$

   $$\frac{\partial J}{\partial b} = \frac{1}{m} \sum_{i=1}^m \left( wx^{(i)} + b - y^{(i)} \right)$$

2. **二阶偏导数计算**：

   $$\frac{\partial^2 J}{\partial w^2} = \frac{1}{m} \sum_{i=1}^m (x^{(i)})^2$$

   $$\frac{\partial^2 J}{\partial b^2} = \frac{1}{m} \sum_{i=1}^m 1 = 1$$

   $$\frac{\partial^2 J}{\partial w \partial b} = \frac{\partial^2 J}{\partial b \partial w} = \frac{1}{m} \sum_{i=1}^m x^{(i)}$$

3. **正定性判定**：

   其行列式为：

   $$\det(H) = \left( \frac{1}{m} \sum_{i=1}^m (x^{(i)})^2 \right) \cdot 1 - \left( \frac{1}{m} \sum_{i=1}^m x^{(i)} \right)^2 = \frac{1}{m} \sum_{i=1}^m (x^{(i)} - \bar{x})^2 = \operatorname{Var}(x)$$

   只要特征分量存在样本方差（即特征值不恒为同一常数），就有 $\det(H) > 0$ 且 $\operatorname{Tr}(H) > 0$，故该二次曲面具有严格的强凸性。

**End of Appendix: A**