// 线性回归示例：C++ 训练，CSV 保存训练过程，Python 负责可视化。
//
// 用法：
//   build/bin/lr_demo.exe data/sample.json build/results
// 然后：
//   python scripts/visualize.py build/results

#include "lr/io.hpp"
#include "lr/linear_regression.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {

void write_csv(const std::filesystem::path& output_dir,
               const lr::RegressionDataset& dataset,
               const std::vector<double>& learning_rates,
               const std::vector<std::vector<lr::TrainingRecord>>& histories,
               const std::vector<std::vector<double>>& predictions) {
  std::filesystem::create_directories(output_dir);

  std::ofstream history_file(output_dir / "loss_history.csv");
  history_file << "learning_rate,epoch,loss\n";
  for (std::size_t i = 0; i < learning_rates.size(); ++i) {
    for (const auto& record : histories[i]) {
      history_file << learning_rates[i] << ',' << record.epoch << ','
                   << record.loss << '\n';
    }
  }

  std::ofstream prediction_file(output_dir / "predictions.csv");
  prediction_file << "sample,target";
  for (const double rate : learning_rates) {
    prediction_file << ",prediction_lr_" << rate;
  }
  prediction_file << '\n';
  for (std::size_t row = 0; row < dataset.num_samples; ++row) {
    prediction_file << row << ',' << dataset.y[row];
    for (const auto& model_predictions : predictions) {
      prediction_file << ',' << model_predictions[row];
    }
    prediction_file << '\n';
  }
}

}  // namespace

int main(int argc, char** argv) {
  const std::filesystem::path input =
      argc > 1 ? argv[1] : std::filesystem::path("data/sample.json");
  const std::filesystem::path output_dir =
      argc > 2 ? argv[2] : std::filesystem::path("build/results");

  // 原始特征有不同量纲时，学习率会比较敏感；这组值适合当前 sample.json。
  const std::vector<double> learning_rates{0.00001, 0.00005, 0.0001};
  constexpr std::size_t epochs = 1000;

  try {
    const lr::RegressionDataset dataset = lr::DatasetIO::load_from_file(input);
    std::vector<std::vector<lr::TrainingRecord>> histories;
    std::vector<std::vector<double>> predictions;
    histories.reserve(learning_rates.size());
    predictions.reserve(learning_rates.size());

    for (const double rate : learning_rates) {
      lr::LinearRegression model(dataset.num_features);
      model.fit(dataset, rate, epochs);
      histories.push_back(model.history());
      predictions.push_back(model.predict(dataset));

      std::cout << "learning_rate=" << rate
                << ", final_loss=" << model.history().back().loss
                << ", bias=" << model.bias() << ", weights=[";
      for (std::size_t j = 0; j < model.weights().size(); ++j) {
        if (j != 0) std::cout << ", ";
        std::cout << model.weights()[j];
      }
      std::cout << "]\n";
    }

    write_csv(output_dir, dataset, learning_rates, histories, predictions);
    std::cout << "CSV files written to: " << output_dir.string() << '\n';
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << '\n';
    return 1;
  }
}
