#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace lr {

// 一份回归训练集。
//
// X 按行主序扁平存储：第 i 个样本的第 j 个特征是 X[i * num_features + j]。
// 这样一整行在内存里是连续的，后面扫样本、算梯度时对缓存友好，也和 Eigen
// 等矩阵库的默认布局一致。
struct RegressionDataset {
  std::vector<std::string> feature_names;  // 长度 == num_features
  std::string target_name;
  std::vector<double> X;  // 长度 == num_samples * num_features
  std::vector<double> y;  // 长度 == num_samples
  std::size_t num_features{0};
  std::size_t num_samples{0};

  double& at(std::size_t i, std::size_t j) noexcept {
    return X[i * num_features + j];
  }

  const double& at(std::size_t i, std::size_t j) const noexcept {
    return X[i * num_features + j];
  }

  // 第 i 个样本的特征视图，零拷贝。返回 std::span 而不是 vector，
  // 是为了避免每次取行都复制一份数据。
  std::span<double> row(std::size_t i) noexcept {
    return {X.data() + i * num_features, num_features};
  }

  std::span<const double> row(std::size_t i) const noexcept {
    return {X.data() + i * num_features, num_features};
  }

  // 预分配，避免逐个 push_back 时的多次扩容。
  void reserve(std::size_t n_samples, std::size_t n_features) {
    X.reserve(n_samples * n_features);
    y.reserve(n_samples);
  }

  bool empty() const noexcept { return num_samples == 0; }
};

}  // namespace lr
