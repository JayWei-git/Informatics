#pragma once

#include <filesystem>
#include <string_view>

#include "lr/dataset.hpp"

namespace lr {

// 训练集 JSON <-> RegressionDataset 的读写接口。
//
// 注意这个头文件刻意不 include nlohmann/json.hpp：JSON 解析是 io.cpp 的
// 实现细节，所以任何 include 了 "lr/io.hpp" 的代码都不会被拖进 900K+ 的
// json.hpp。对应的，CMake 里 nlohmann 是 lr_io 的 PRIVATE 依赖。
class DatasetIO {
 public:
  // 从文件读取并做维度校验。
  // 文件打不开、JSON 非法或字段不合法时抛 std::runtime_error。
  static RegressionDataset load_from_file(
      const std::filesystem::path& file_path);

  // 从 JSON 字符串解析，同样做维度校验。
  static RegressionDataset load_from_string(std::string_view json_str);

  // 序列化写出。indent 是缩进空格数，传 -1 输出紧凑格式。
  static void save_to_file(const RegressionDataset& dataset,
                           const std::filesystem::path& file_path,
                           int indent = 2);
};

}  // namespace lr
