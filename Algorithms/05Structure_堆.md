# Heap

> Author：玖沐 (Jura)  |  Last Updated：2026/11/02

## Theory

##### Binary Heap

- 底层是一颗被存储为数组的完全二叉树；0-based索引下：

  - 父节点索引为 $p$，则左孩子为 $2*p+1$，右孩子为 $2*p+2$；

  - 最后一个非叶节点索引 可由 最后一个左孩子索引 推出；

- 二叉堆常用于实现优先队列，支持 $O(\log n)$ 插入和取出当前最值，以及 $O(1)$ 查看最值；

  ```cpp
  // 示例，大根堆
  // 底层vector管理size和capacity；若已知最大容量，可预先reserve以避免扩容
  // 这里只存储int类型的键
  class Heap {
  private:
      vector<int> data;
      
      void shiftUp(size_t i) { // 调用者保证i < data.size()
          int temp = data[i];
          while (i > 0) {
              if (temp > data[(i - 1) / 2]) {
                  data[i] = data[(i - 1) / 2];
                  i = (i - 1) / 2;
              } else {
                  break;
              }
          }
          data[i] = temp;
      }
      
      void shiftDown(size_t i) {
          int temp = data[i];
          while (i * 2 + 1 < size()) {
              size_t left = i * 2 + 1;
              size_t right = left + 1;
              size_t better = (right < size() && data[right] > data[left]) ? right : left;
              
              if (data[better] > temp) {
                  data[i] = data[better];
                  i = better;
              } else {
                  break;
              }
          }
          data[i] = temp;
      }
      
  public:
      size_t size() const { return data.size(); }
      bool empty() const { return data.empty(); }
      
      void push(int val) {
          data.push_back(val);
          shiftUp(size() - 1);
      }
      
      void pop() {
          if (data.empty()) throw std::out_of_range("heap is empty");
          swap(data[0], data.back());
          data.pop_back();
          if (!data.empty()) shiftDown(0);
      }
      
      int top() const {
          if (data.empty()) throw std::out_of_range("heap is empty");
          return data[0];
      }
  };
  ```

  还可以提供一个建堆的公共方法：一般从最后一个非叶子开始逐个下沉 (不是必需的)；
  
  ```cpp
  void build(vector<int> values) {
      data = move(values);
  
      for (size_t i = size() / 2; i > 0; --i) {
          shiftDown(i - 1);
      }
  }
  ```

- 对顶堆：动态维护一个序列上第k小的元素，k可能会变化；

  一个大根堆一个小根堆；

  ```cpp
  class DualHeap {
  private:
      size_t k;
      MaxHeap lower; // 较小的 min(k, 元素总数) 个数
      MinHeap upper; // 其余的数
  
  public:
      explicit DualHeap(size_t k) : k(k) {
          if (k == 0) throw invalid_argument("k must be positive");
      }
  
      void insert(int x) {
          // lower空就扔进lower
          if (lower.size() < k) {
              lower.push(x);
          // lower不空，看x和lower.top大小关系
          } else if (x < lower.top()) {
              upper.push(lower.top());
              lower.pop();
              lower.push(x);
          } else {
              upper.push(x);
          }
      }
  
      void setK(size_t newK) {
          if (newK == 0) throw invalid_argument("k must be positive");
          k = newK;
  
          // "沙漏"
          while (lower.size() > k) { // k调小了
              upper.push(lower.top());
              lower.pop();
          }
          while (lower.size() < k && !upper.empty()) { // k调大了
              lower.push(upper.top());
              upper.pop();
          }
      }
  
      int kth() const {
          if (lower.size() < k) {
              throw out_of_range("fewer than k elements");
          }
          return lower.top(); // 第 k 小
      }
  
      size_t size() const {
          return lower.size() + upper.size();
      }
  };
  ```

---









## Practice



