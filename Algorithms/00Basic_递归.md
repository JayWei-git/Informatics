# Recursion

> Author：玖沐 (Jura)  |  Last Updated：2026/11/14

- [左程云 - 算法038](https://www.bilibili.com/video/BV19m4y1n7mo)；

## Theory

- 递归：函数直接或间接调用自身，将原问题转化为规模更小或更接近终止状态的同类子问题；

- 写递归先明确三件事：
  1. **函数含义**：参数表示什么状态，返回值解决什么问题；
  2. **递归基**：哪些情况可以直接返回，不再继续调用；
  3. **递推关系**：如何利用子问题的结果解决当前问题，并保证每条递归路径最终到达递归基；

- 思考时可以先相信子调用能完成约定的任务，只关注当前这一层如何拆分、处理和组合结果，不必在脑中逐层展开；

- 每次调用都有独立的栈帧，保存参数、局部变量和返回位置；递归调用前的操作在向下展开时执行，调用后的操作在返回时执行；

- 带状态的递归中，值传递通常让各分支保留独立副本；若通过引用修改共享状态，且修改会影响后续分支，就需要在返回后恢复现场，这就是回溯中的“选择 → 递归 → 撤销选择”；

- 时间复杂度看总调用次数及每次调用的工作量；额外空间需计入同时存在的栈帧，若每层占用常数空间，则调用栈空间为 $O(h)$，其中 $h$ 为最大递归深度；递归过深可能导致栈溢出；

---

##### From Recursion to Iteration

- 非递归并不意味着不使用栈；递归与否取决于函数是否直接或间接调用自身，而不是程序是否使用了先进后出的数据结构；

- 用一个例子展示：

  ```cpp
  int f(int n) {
      return (n <= 1) ? 1 : f(n - 1) + g(n - 2);
  }
  
  int g(int n) {
      return (n <= 1) ? 1 : f(n + 1) + g(n - 1);
  }
  ```

  假设以`f`为调用起点，如何改为非递归？

  ```cpp
  struct StackFrame {
  	int n, temp; // 临时变量，一个入参，一个中间结果
  	int pc;      // 0刚进入，1执行了f，2执行了g (f/g都是先f后g不用区分)
      bool flag;   // 标识是哪个函数的栈帧true->f, false->g
  };
  
  int func(int n) {
  	stack<StackFrame> s;
  	s.push({true, n, 0, 0}); // f为调用起点
  	int eax = 0; // 返回值寄存器
  
  	while (!s.empty()) {
  		StackFrame& sf = s.top(); // 引用，要修改pc
  		if (sf.n <= 1) { // 到递归基了
  			eax = 1;
  			s.pop();
  			continue;
  		}
  
  		switch (sf.pc) {
  			case 0 : {
  				int nextN = sf.flag ? sf.n - 1 : sf.n + 1;
  				sf.pc = 1; // 保存返回地址
  				s.push({true, nextN, 0, 0}); // 调用子函数
  				break;
  			}
  			case 1 : { 
  				sf.temp = eax; // 保存中间结果
  				int nextN = sf.flag ? sf.n - 2 : sf.n - 1;
  				sf.pc = 2;
  				s.push({false, nextN, 0, 0});
  				break;
  			}
  			case 2 : { 
  				eax += sf.temp; // 累加得到整体的返回值
  				s.pop();
  				break;
  			}
  		}
  	}
  
  	return eax; // 返回最终的返回值即可
  }
  ```
  
  `二叉树.md`中有用这种方法改写二叉树三种遍历的dai ma

---

##### From Explicit Stack to Recursion

- 反过来，也可以用系统调用栈代替显式辅助栈：每层局部变量暂存元素，递归深入，回溯时再处理或放回；

- 下面两题保留原栈 `s`，不创建辅助容器，但递归调用栈仍占用 $O(n)$ 空间；

1. 例一：递归逆序栈

   不显式创建辅助容器，递归逆序一个 `stack`，允许使用系统调用栈；

   `reverseStack`：取出栈底，递归逆序剩余栈，再把原栈底压到栈顶；

   `popBottom`：弹出栈顶；若栈空，它就是栈底，直接返回；否则递归取出栈底，再将栈顶放回；

   ```cpp
   // 调用前提：s 非空
   int popBottom(stack<int>& s) {
       int topVal = s.top();
       s.pop();
       if (s.empty()) return topVal; // base case
       
       int bottomVal = popBottom(s);
       
       s.push(topVal);
       
       return bottomVal;
   }
   
   void reverseStack(stack<int>& s) {
       if (s.empty()) return;
       int bottomVal = popBottom(s);
       reverseStack(s);
       s.push(bottomVal);
   }
   ```

   每层的 `topVal`、`bottomVal` 由调用栈暂存，回溯时再压回原栈；

   时间 $\Theta(n^2)$，辅助空间 $O(n)$；

2. 例二：递归排序栈

   不显式创建辅助容器，递归排序一个 `stack`，使最小元素位于栈顶；

   `insertSorted`：将元素插入有序栈，保持栈顶最小；

   `sortStack`：弹出栈顶，递归排好剩余栈，再用 `insertSorted` 插回；

   ```cpp
   void insertSorted(stack<int>& s, int val) {
       if (s.empty() || val <= s.top()) { // base case
           s.push(val);
           return;
       }
       
       int topVal = s.top();
       s.pop();
       
       insertSorted(s, val);
       
       s.push(topVal);
       
       return;
   }
   
   void sortStack(stack<int>& s) {
       if (s.empty()) return;
       
       int topVal = s.top();
       s.pop();
       
       sortStack(s);
       
       insertSorted(s, topVal);
       
       return;
   }
   ```

   `insertSorted` 用调用栈暂存挡住插入位置的元素，插入 `val` 后，回溯时逐个放回；

   最坏时间 $O(n^2)$，辅助空间 $O(n)$；

---

##### Nested Recursion

- 嵌套类递归：每层递归处理一层嵌套，并记录全局解析位置；
  - 函数遇到嵌套条件开始就递归，调用下层子函数去处理嵌套；

  - 下级子函数处理完成后要更新全局解析位置变量，上层函数才能得知从哪里继续；








## Practice

##### NC - 字符串子序列

- 题目：[牛客 - 字符串的全部子序列](https://www.nowcoder.com/practice/92e6247998294f2c933906fdedbc6e6a)；

  回溯即可，两个分支，选或者不选，选了当前字符要回溯恢复现场；

  ```cpp
  class Solution {
  private:
      string path;
      unordered_set<string> ans;
  
      void dfs(string& s, int index) {
          if (index >= s.size()) {
              ans.insert(path); // 哈希表会去重
              return;
          }
  
          dfs(s, index + 1); // 不选
  
          path.push_back(s[index]); // 选
          dfs(s, index + 1);
          path.pop_back(); // 恢复现场，不理解为什么举个例子
      }
  
  public:
      vector<string> generatePermutation(string s) {
          path.clear();
  		ans.clear();
          dfs(s, 0);
          return {ans.begin(), ans.end()};
    }
  };
```

---

##### LC90

- 题目：[LeetCode - 90. 子集II](https://leetcode.cn/problems/subsets-ii/)；注意该题中的集合可包含相同元素；

  此题难点在于子集的去重；方法是排序原数组让相同元素相邻，假设当前重复数字有`count`个，那么分支就是选择`0`到`count`个当前重复数字；

  ```cpp
  class Solution {
  private:
      vector<vector<int>> ans;
      vector<int> path;
  
      void dfs(vector<int>& nums, int idx) {
          if (idx >= nums.size()) {
              ans.push_back(path);
              return;
          }
  
          int cnt = 0; // 统计当前重复元素个数
          while (cnt + idx < nums.size() && nums[idx] == nums[idx + cnt]) cnt++;
  
          int val = nums[idx];
          // 分支：选 0 ~ cnt 个重复元素
          for (int k = 0; k <= cnt; k++) {
              for (int i = 0; i < k; i++) path.push_back(val);
              dfs(nums, idx + cnt); // 别忘了要跳到下一个重复元素
              for (int i = 0; i < k; i++) path.pop_back();
          }
  
      }
  
  public:
      vector<vector<int>> subsetsWithDup(vector<int>& nums) {
          ans.clear();
          path.clear();
          sort(nums.begin(), nums.end());
          dfs(nums, 0);
          return ans;
      }
  };
```


---

##### LC46

- 题目：[LeetCode - 46. 全排列](https://leetcode.cn/problems/permutations)；

  比如{3,1,2,4,5}，假设path目前为{3,4}，那么第三个位置会依次放入1,2,5，若path来到{3,4,2}那么第四个位置会依次放入1,5；

  所以：每次分支必须从头开始遍历，且需要一个 `added[]` 数组来判断哪些已经在path里面了；

  ```cpp
  class Solution {
  private:
      vector<bool> added;
      vector<int> path;
      vector<vector<int>> ans;
  
      void dfs(vector<int>& nums) {
          if (path.size() >= nums.size()) {
              ans.push_back(path);
              return;
          }
  
          for (int i = 0; i < nums.size(); i++) {
              if (added[i]) continue; // 你可不要给我写个i < nums.size() && !added[i] 直接把整个循环结束了
  
              path.push_back(nums[i]);
              added[i] = true;
              dfs(nums);
              path.pop_back();
              added[i] = false;
          }
      }
  
  public:
      vector<vector<int>> permute(vector<int>& nums) {
          added.assign(nums.size(), false);
          path.clear();
          ans.clear();
          dfs(nums);
          return ans;
      }
  };
  ```

  还是上面的例子{3,1,2,4,5}，path目前为{3,4}，由于第三个位置依次放入1,2,5或者2,1,5或者5,2,1是不影响的，所以可以直接在nums上原地swap；
  
  相当于分成了选择区和未选择区；
  
  ```cpp
  class Solution {
  private:
      vector<vector<int>> ans;
      // [0, bound)是选择区，[bound, size)是未选择区
      void dfs(vector<int>& nums, int bound) {
          if (bound >= nums.size()) {
              ans.push_back(nums);
              return;
          }
  
          for (int i = bound; i < nums.size(); i++) {
              swap(nums[bound], nums[i]);
              dfs(nums, bound + 1);
              swap(nums[bound], nums[i]); // 别忘了恢复现场
          }
      }
  
  public:
      vector<vector<int>> permute(vector<int>& nums) {
          ans.clear();
          dfs(nums, 0);
          return ans;
      }
  };
  ```

---

##### LC47

- 题目：[LeetCode - 47. 全排列II](https://leetcode.cn/problems/permutations-ii/)；

  思路与 LC46 一致，但由于有重复元素，直接套用 LC46 的解法会生成重复的全排列，比如{$1_1,1_2,2$}和{$1_2,1_1,2$}；

  画出分支图，可以发现剪枝条件是同一位置不重复放置相同数字，比如 {$1_1,1_2,2$}，path为 { $2$ }，那么两个分支 {$2,1_1$} 和 {$2,1_2$} 是相同的，需要减掉一个；

  用哈希表即可；

  ```cpp
  class Solution {
  private:
      vector<vector<int>> ans;
  
      void dfs(vector<int>& nums, int bound) {
          if (bound >= nums.size()) {
              ans.push_back(nums);
              return;
          }
  
          unordered_set<int> used;
          for (int i = bound; i < nums.size(); i++) {
              if (used.count(nums[i])) continue; // 同一位置不重复放置相同数字，剪掉
              used.insert(nums[i]);
              
              swap(nums[bound], nums[i]);
              dfs(nums, bound + 1);
              swap(nums[bound], nums[i]);
          }
      }
      
  public:
      vector<vector<int>> permuteUnique(vector<int>& nums) {
          ans.clear();
          dfs(nums, 0);
          return ans;
      }
  };
  ```

---

##### LC22

- 题目：[LeetCode - 22. 括号生成](https://leetcode.cn/problems/generate-parentheses)；

  想放右括号必须是有未配对的左括号才行；

  ```cpp
  class Solution {
  private:
      vector<string> ans;
      string path;
  
      void dfs(int n, int r, int l, int unmatched) {
          if (r + l == 2 * n) {
              ans.push_back(path);
              return;
          }
  
          if (l < n) { // 不能一直放左括号，最多放n个
              path.push_back('(');
              dfs(n, r, l + 1, unmatched + 1);
              path.pop_back();
          }
  
          if (unmatched > 0) { // 有未匹配左括号才能放右括号
              path.push_back(')');
              dfs(n, r + 1, l, unmatched - 1);
              path.pop_back();
          }
      }
  
  public:
      vector<string> generateParenthesis(int n) {
          ans.clear();
          path.clear();
          dfs(n, 0, 0, 0);
          return ans;
      }
  };
  ```


---

##### LC(面试题)08.06

- 题目：[LeetCode - 面试题 08.06. 汉诺塔问题](https://leetcode.cn/problems/hanota-lcci/)；

  ```cpp
  // 定义 move(n, A, B, C) 为将A的上面n个圆盘移动到C
  // 先将A上面的n-1个盘子移动到B，然后将A最大的盘子移到C，然后将B上面剩余的移动到C即可
  // 注意：先将A最小的移到B，再将A下面的n-1个移到C，再将B的移到C 是错的，会违背小的只能在大的上面(想想为什么)
  class Solution {
  private:
      void move(int n, vector<int>& A, vector<int>& B, vector<int>& C) {
          if (n == 1) {
              C.push_back(A.back());
              A.pop_back();
              return;
          }
  
          move(n - 1, A, C, B);
          C.push_back(A.back());
          A.pop_back();
          move(n - 1, B, A, C);
      }
  
  public:
      void hanota(vector<int>& A, vector<int>& B, vector<int>& C) {
          move(A.size(), A, B, C);
      }
  };
  ```

---

##### LC394

- 题目：[LeetCode - 394. 字符串解码](https://leetcode.cn/problems/decode-string/)；

  标准嵌套递归，按照`Theory`的模板填就行了；

  ```cpp
  class Solution {
  private:
      int where; // 全局记录当前解析位置
  
      string parse(const string& s) {
          string ans;
          int count = 0;
  
          while (where < s.size() && s[where] != ']') { // where停在']'，符合下方的跳过逻辑
              char c = s[where];
              if (c >= '0' && c <= '9') {
                  count = count * 10 + (c - '0');
                  where++;
              } else if (c == '[') {
                  where++;                 // 跳过'['
                  string inner = parse(s);
                  where++;                 // 跳过']'
  
                  for (int k = 0; k < count; k++) ans += inner;
  
                  count = 0; // 注意！！必须清零，因为可能有多个平行嵌套
              } else {
                  ans += c;
                  where++;
              }
          }
  
          return ans;
      }
  
  public:
      string decodeString(string s) {
          where = 0;
          return parse(s);
      }
  };
  ```

---

##### LC726

- 题目：[LeetCode - 726. 原子的数量](https://leetcode.cn/problems/number-of-atoms/)；

  
