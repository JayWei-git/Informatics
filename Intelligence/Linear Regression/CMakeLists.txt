cmake_minimum_required(VERSION 3.20)
project(linear_regression LANGUAGES CXX)

# --- 语言标准 ---------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # 用 -std=c++20 而不是 -std=gnu++20

# 可执行文件统一放到 build/bin，方便运行
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- 第三方依赖 -------------------------------------------------------------
# nlohmann/json 是单头文件库，用 INTERFACE 库表示：它不产出编译产物，
# 只向链接它的人传递「去哪里找头文件」这一条使用要求。
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# --- 编译警告 ---------------------------------------------------------------
# 同样做成 INTERFACE 库，这样以后新增目标只要 target_link_libraries 一行
# 就能继承全部警告选项，不用到处复制 target_compile_options。
add_library(lr_warnings INTERFACE)
target_compile_options(lr_warnings INTERFACE -Wall -Wextra -Wpedantic)

# --- IO 库 ------------------------------------------------------------------
add_library(lr_io STATIC src/io.cpp src/linear_regression.cpp)
target_include_directories(lr_io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(lr_io
    PUBLIC  lr_warnings    # 用 lr_io 的人也应该开同样的警告
    PRIVATE nlohmann_json) # JSON 是内部实现细节，不向外传播

# --- 验证用可执行文件 -------------------------------------------------------
option(LR_BUILD_DEMO "Build the IO demo executable" ON)

if(LR_BUILD_DEMO)
  add_executable(lr_demo src/main.cpp)
  target_link_libraries(lr_demo PRIVATE lr_io)
endif()
