# Stack

> Author：玖沐 (Jura)  |  Last Updated：2026/11/09

- [左程云 - 算法015 最小栈](https://www.bilibili.com/video/BV15X4y177cM)；

## Theory

- 这里的栈指一种实现只允许元素先进后出的集合的数据结构，而非程序运行时的系统栈/栈空间；
  - 系统栈见`递归.md`；

##### std::stack

- `push(x)` 入栈，`top()` 查看栈顶，`pop()` 删除栈顶，`empty()` 判空，`size()` 查看数量；

  ```cpp
  std::stack<int> st;
  st.push(3);
  st.push(5);
  
  while (!st.empty()) {
      int x = st.top();  // 先取值
      st.pop();          // 再删除
  }
  ```

---

##### Min Stack

- 实现一个 $O(1)$ 的`getMin()`方法；

  封装`data`和`min`两个栈即可，二者同步更新；

  ```cpp
  class MinStack {
  private:
      stack<int> data, min;
  public:
      MinStack() = default;
      
      void push(int value) {
          data.push(value);
          
          if (min.empty()) min.push(value);
          else min.push(value > min.top() ? min.top() : value); // 无论压入还是弹出，min.top始终是最小值
      }
      
      void pop() {
          data.pop();
          min.pop();
      }
      
      int top() { return data.top(); }
      
      int getMin() { return min.top(); }
  };
  ```







## Practice