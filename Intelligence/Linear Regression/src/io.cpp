#include "lr/io.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace lr {
namespace {

using nlohmann::json;

// 取必需键。缺失时报出键名，比 nlohmann 默认的异常信息好定位。
const json& require(const json& object, const char* key) {
  const auto it = object.find(key);
  if (it == object.end()) {
    throw std::runtime_error(std::string("Missing required key: \"") + key +
                             "\"");
  }
  return *it;
}

RegressionDataset parse_json(const json& root) {
  RegressionDataset dataset;

  const json& names = require(root, "feature_names");
  const json& target_name = require(root, "target_name");
  const json& samples = require(root, "samples");

  if (!names.is_array()) {
    throw std::runtime_error("\"feature_names\" must be an array");
  }
  if (!target_name.is_string()) {
    throw std::runtime_error("\"target_name\" must be a string");
  }
  if (!samples.is_array()) {
    throw std::runtime_error("\"samples\" must be an array");
  }

  dataset.feature_names = names.get<std::vector<std::string>>();
  dataset.target_name = target_name.get<std::string>();
  dataset.num_features = dataset.feature_names.size();
  dataset.num_samples = samples.size();

  dataset.reserve(dataset.num_samples, dataset.num_features);

  for (std::size_t i = 0; i < samples.size(); ++i) {
    const json& sample = samples[i];
    const std::string where = "Sample " + std::to_string(i);

    if (!sample.is_object()) {
      throw std::runtime_error(where + " must be an object");
    }

    const json& features = require(sample, "features");
    if (!features.is_array()) {
      throw std::runtime_error(where + ": \"features\" must be an array");
    }
    if (features.size() != dataset.num_features) {
      throw std::runtime_error(
          "Dimension mismatch at sample " + std::to_string(i) + ": expected " +
          std::to_string(dataset.num_features) + " features, got " +
          std::to_string(features.size()));
    }

    for (const json& value : features) {
      dataset.X.push_back(value.get<double>());
    }
    dataset.y.push_back(require(sample, "target").get<double>());
  }

  return dataset;
}

json to_json(const RegressionDataset& dataset) {
  json root;
  root["version"] = "1.0";
  root["feature_names"] = dataset.feature_names;
  root["target_name"] = dataset.target_name;

  json samples = json::array();
  for (std::size_t i = 0; i < dataset.num_samples; ++i) {
    const auto row = dataset.row(i);

    json sample;
    sample["features"] = std::vector<double>(row.begin(), row.end());
    sample["target"] = dataset.y[i];
    samples.push_back(std::move(sample));
  }

  root["samples"] = std::move(samples);
  return root;
}

// 把 nlohmann 的异常和内部错误统一成带来源信息的 std::runtime_error，
// 这样调用方只需要 catch 一种异常类型。
RegressionDataset parse_with_context(const json& root,
                                     const std::string& source) {
  try {
    return parse_json(root);
  } catch (const json::exception& e) {
    throw std::runtime_error(source + ": " + e.what());
  } catch (const std::runtime_error& e) {
    throw std::runtime_error(source + ": " + e.what());
  }
}

}  // namespace

RegressionDataset DatasetIO::load_from_file(
    const std::filesystem::path& file_path) {
  std::ifstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + file_path.string());
  }

  json root;
  try {
    file >> root;
  } catch (const json::exception& e) {
    throw std::runtime_error("Invalid JSON in " + file_path.string() + ": " +
                             e.what());
  }

  return parse_with_context(root, file_path.string());
}

RegressionDataset DatasetIO::load_from_string(std::string_view json_str) {
  json root;
  try {
    root = json::parse(json_str);
  } catch (const json::exception& e) {
    throw std::runtime_error(std::string("Invalid JSON: ") + e.what());
  }

  return parse_with_context(root, "<string>");
}

void DatasetIO::save_to_file(const RegressionDataset& dataset,
                             const std::filesystem::path& file_path,
                             int indent) {
  std::ofstream file(file_path);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file for writing: " +
                             file_path.string());
  }

  file << to_json(dataset).dump(indent) << '\n';
  if (!file) {
    throw std::runtime_error("Failed to write file: " + file_path.string());
  }
}

}  // namespace lr
