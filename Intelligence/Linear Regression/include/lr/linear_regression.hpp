#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "lr/dataset.hpp"

namespace lr {

// 一轮训练后的记录。把它保存下来，Python 就可以画出学习曲线。
struct TrainingRecord {
  std::size_t epoch{};
  double loss{};
};

// 最基础的多元线性回归：
//
//     y_hat = b + w0*x0 + w1*x1 + ... + w(n-1)*x(n-1)
//
// 这里故意不依赖矩阵库，所有计算都用 for 循环展开，便于学习梯度下降。
class LinearRegression {
 public:
  explicit LinearRegression(std::size_t num_features);

  // 用批量梯度下降训练。每一轮先计算全部样本的梯度，再统一更新参数。
  void fit(const RegressionDataset& dataset, double learning_rate,
           std::size_t epochs);

  double predict(const std::vector<double>& features) const;
  std::vector<double> predict(const RegressionDataset& dataset) const;

  double bias() const noexcept { return bias_; }
  const std::vector<double>& weights() const noexcept { return weights_; }
  const std::vector<TrainingRecord>& history() const noexcept {
    return history_;
  }

 private:
  double bias_{0.0};
  std::vector<double> weights_;
  std::vector<TrainingRecord> history_;
};

}  // namespace lr
