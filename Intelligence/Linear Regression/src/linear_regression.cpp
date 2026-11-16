#include "lr/linear_regression.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace lr {

LinearRegression::LinearRegression(const std::size_t num_features)
    : weights_(num_features, 0.0) {}

double LinearRegression::predict(const std::vector<double>& features) const {
  if (features.size() != weights_.size()) {
    throw std::invalid_argument("Feature dimension does not match the model");
  }

  double result = bias_;
  for (std::size_t j = 0; j < weights_.size(); ++j) {
    result += weights_[j] * features[j];
  }
  return result;
}

std::vector<double> LinearRegression::predict(
    const RegressionDataset& dataset) const {
  if (dataset.num_features != weights_.size()) {
    throw std::invalid_argument("Dataset dimension does not match the model");
  }

  std::vector<double> result;
  result.reserve(dataset.num_samples);
  for (std::size_t i = 0; i < dataset.num_samples; ++i) {
    const auto row = dataset.row(i);
    result.push_back(bias_);
    for (std::size_t j = 0; j < weights_.size(); ++j) {
      result.back() += weights_[j] * row[j];
    }
  }
  return result;
}

void LinearRegression::fit(const RegressionDataset& dataset,
                           const double learning_rate,
                           const std::size_t epochs) {
  if (dataset.num_features != weights_.size()) {
    throw std::invalid_argument("Dataset dimension does not match the model");
  }
  if (dataset.num_samples == 0) {
    throw std::invalid_argument("Cannot train on an empty dataset");
  }
  if (!(learning_rate > 0.0) || !std::isfinite(learning_rate)) {
    throw std::invalid_argument("Learning rate must be a positive finite number");
  }

  bias_ = 0.0;
  std::fill(weights_.begin(), weights_.end(), 0.0);
  history_.clear();
  history_.reserve(epochs);

  const double sample_count = static_cast<double>(dataset.num_samples);
  for (std::size_t epoch = 0; epoch < epochs; ++epoch) {
    std::vector<double> gradient(weights_.size(), 0.0);
    double bias_gradient = 0.0;
    double squared_error = 0.0;

    // 对每个样本计算误差，并累计梯度。
    for (std::size_t i = 0; i < dataset.num_samples; ++i) {
      const auto row = dataset.row(i);
      double prediction = bias_;
      for (std::size_t j = 0; j < weights_.size(); ++j) {
        prediction += weights_[j] * row[j];
      }

      const double error = prediction - dataset.y[i];
      squared_error += error * error;
      bias_gradient += error;
      for (std::size_t j = 0; j < weights_.size(); ++j) {
        gradient[j] += error * row[j];
      }
    }

    // MSE = mean((prediction - target)^2)。
    // 对 MSE 的导数多一个 2；这里除以 2，等价于使用 half-MSE，
    // 只会改变梯度的整体缩放，不影响最终拟合结果。
    const double loss = squared_error / (2.0 * sample_count);
    history_.push_back({epoch, loss});

    bias_ -= learning_rate * bias_gradient / sample_count;
    for (std::size_t j = 0; j < weights_.size(); ++j) {
      weights_[j] -= learning_rate * gradient[j] / sample_count;
    }
  }
}

}  // namespace lr
