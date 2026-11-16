# Greedy Algorithm

> Author：玖沐 (Jura)  |  Last Updated：2026/11/10

- [左程云 - 算法089 贪心专题1](https://www.bilibili.com/video/BV1be41167yw)；

- [左程云 - 算法090 贪心专题2](https://www.bilibili.com/video/BV1Rp4y1973N)；

- [左程云 - 算法091 贪心专题3](https://www.bilibili.com/video/BV1ST4y1s7XT)；

- [左程云 - 算法092 贪心专题4](https://www.bilibili.com/video/BV1MG411z7Yz)；

- [左程云 - 算法093 贪心专题5](https://www.bilibili.com/video/BV16Q4y1L7s9)；

- [左程云 - 算法094 贪心专题6](https://www.bilibili.com/video/BV1r5411q7UV)；

## Theory

- **Greedy Algorithm** :x: ​；**Guess Algorithm** :white_check_mark:；





## Practice

##### LC179

- 题目：[LeetCode - 179. 最大数](https://leetcode.cn/problems/largest-number/)；

  原型是拼接所有字符串使结果字典序最小；

  策略：排序所有字符串然后依次拼接，但是排序规则不是直接比较而是拼接后比较；

  ```cpp
  string largestNumber(vector<int>& nums) {
  	vector<string> parts;
      for (int num : nums) {
  		parts.push_back(to_string(num));
      }
      
      sort(parts.begin(), parts.end(), 
          [](const string& a, const string& b) {
              return a + b > b + a;
          });
      
      if (parts[0] == "0") return "0"; // 有前导零
      
      string ans;
      for (const string& part : parts) {
          ans += part;
      }
      return ans;
  }
  ```

---

##### LC1029

- 题目：[LeetCode - 1029. 两地调度](https://leetcode.cn/problems/two-city-scheduling/)；

  策略：先假设所有2n个人都去A地，然后收集每个人从A改成B的差值，将差值最小的前n个人换成B即可；

  ```cpp
  int twoCitySchedCost(vector<vector<int>>& costs) {
      vector<int> diff;
      diff.reserve(costs.size());
      int ans = 0;
  	for (const auto& cost : costs) {
          ans += cost[0]; // 所有人先去A
          diff.push_back(cost[1] - cost[0]);
      }
      
      sort(diff.begin(), diff.end());
      
      for (int i = 0; i < diff.size() / 2; i++) { // 差值最小前n个人换B
          ans += diff[i];
      }
      
      return ans;
  }
  ```

---

##### LC1553

- 题目：[LeetCode - 1553. 吃掉 N 个橘子的最少天数](https://leetcode.cn/problems/minimum-number-of-days-to-eat-n-oranges/)；

  首先想到的肯定是递归DFS搜索；

  ```cpp
  int minDays(int n) {
      if (n <= 1) return n;
  	int ans1 = minDays(n - 1) + 1;
      int ans2 = INT_MAX;
      if (n % 2 == 0) ans2 = minDays(n / 2) + 1;
      int ans3 = INT_MAX;
      if (n % 3 == 0) ans3 = minDays(n / 3) + 1;
      
      return min(min(ans1, ans2), ans3);
  }
  ```
  
  但这肯定是会超时的；不能贪心地认为能按比例吃就一定要立刻按比例吃，比如 $n=10$ 时，先吃一个走到 $9$ 再按比例吃只需 $4$ 天，直接吃掉一半则需要 $5$ 天；
  
  画个图可以发现能把连续吃一个的操作合并到下一次按比例吃的操作中；第一条路径先用 $n\bmod 2$ 天吃到偶数，再用一天吃掉一半，剩余 $n/2$ 个；第二条路径先用 $n\bmod 3$ 天吃到 $3$ 的倍数，再用一天吃掉三分之二，剩余 $n/3$ 个；这里的除法都是向下取整；
  
  ```cpp
  int minDays(int n) {
      if (n <= 1) return n;
      return 1 + min(n % 2 + minDays(n / 2), n % 3 + minDays(n / 3)); // 整数除法有截断
  }
  ```
  
  按这个递推式写成自底向上的DP就是：
  
  ```cpp
  int minDays(int n) {
      if (n <= 1) return n;
      vector<int> dp(n + 1);
      dp[0] = 0;
      dp[1] = 1;
      for (int i = 2; i <= n; i++) {
  		dp[i] = 1 + min(i % 2 + dp[i / 2], i % 3 + dp[i / 3]);
      }
      return dp[n];
  }
  ```
  
  但这需要 $O(n)$ 的时间和空间；$n$ 很大时，遍历会超时，数组也可能因内存不足而无法创建；
  
  那怎么办？只算需要的状态；比如求 $dp[10]$，只会用到 $dp[0]$、$dp[1]$、$dp[2]$、$dp[3]$、$dp[5]$ 和 $dp[10]$，其余状态是冗余的；虽然 $10$ 到 $3$ 的实际过程经过 $9$，但吃掉一个所需的一天已经计入 $10\bmod 3$，不需要计算 $dp[9]$；
  
  那么稀疏化记忆数组即可；
  
  ```cpp
  unordered_map<int, int> dp;
  int minDays(int n) {
      if (n <= 1) return n;
      if (dp.count(n)) return dp[n]; // Cache Hit
      return dp[n] = 1 + min(n % 2 + minDays(n / 2), n % 3 + minDays(n / 3)); // Cache Fresh   
  }
  ```
  
  稀疏化的自底向上DP不太好写，就不管了；

---

##### LC630

- 题目：[LeetCode - 630. 课程表 III](https://leetcode.cn/problems/course-schedule-iii/)；

  贪心策略：按截止日期从早到晚处理课程；一旦总学习时间超过当前截止日期，就放弃已选课程中耗时最长的那门；
  
  ```cpp
  int scheduleCourse(vector<vector<int>>& courses) {
      sort(courses.begin(), courses.end(),  // 按截止日期升序排列
          [](const vector<int>& a, const vector<int>& b) {
              return a[1] < b[1];
          });
  
      priority_queue<int> chosen;
      int total_days = 0;
      for (const auto& course : courses) {
          chosen.push(course[0]);
          total_days += course[0];
  
          if (total_days > course[1]) {
              total_days -= chosen.top(); // 放弃耗时最长的
              chosen.pop();
          }
      }
  
      return chosen.size();
  }
  ```

---

##### LC(LCR)132

- 题目：[LeetCode - LCR 132. 砍竹子 II](https://leetcode.cn/problems/jian-sheng-zi-ii-lcof/)

  没办法尝试，乘出来的数太大了，中间取余会丢失大小信息；

  直接记策略：尽量拆3，但是如果`n % 3 == 1`，最后一个(3 + 1)要拆成(2 + 2)；

  - `n % 3 == 0`，可拆成全3，此时认为tail == 1；
  - `n % 3 == 2`，tail == 2；
  - `n % 3 == 1`，tail == 2 * 2；

  边界要单独处理，n = 2返回1，n = 3返回2；

  然后这个题要用到乘法快速幂，忘了点[链接](https://www.bilibili.com/video/BV1Rp4y1973N?t=35.4)；

  ```cpp
  class Solution {
  private:
      static constexpr long long MOD = 1000000007LL;
      long long power(long long x, int n) { // 乘法用long long避免乘的过程中溢出
          long long ans = 1;
          while (n != 0) {
              if (n & 1) ans = ans * x % MOD;
              x = x * x % MOD;
              n >>= 1;
          }
          return ans;
      }
  
  public:
      int cuttingBamboo(int bamboo_len) {
          int n = bamboo_len;
          if (n == 2) return 1;
          if (n == 3) return 2;
  
          int tail = n % 3 == 0 ? 1 : (n % 3 == 1 ? 4 : 2);
          int p = (tail == 1 ? n : n - tail) / 3;
          return static_cast<int>(tail * power(3, p) % MOD);
      }
  };
  ```

---

##### 分成 k 份的最大乘积

- 题目：给定 $n$ 和 $k$，把正整数 $n$ 恰好分成 $k$ 个正整数，求乘积的最大值，对 $10^9+7$ 取模；来源为笔试题，无在线测试；

---

##### LC435

- 题目：[LeetCode - 435. 无重叠区间](https://leetcode.cn/problems/non-overlapping-intervals/)；

---

##### LG1803

- 题目：[洛谷 - P1803. 凌乱的 yyy / 线段覆盖](https://www.luogu.com.cn/problem/P1803)；

---

##### LC1353

- 题目：[LeetCode - 1353. 最多可以参加的会议数目](https://leetcode.cn/problems/maximum-number-of-events-that-can-be-attended/)；

---

##### LC502

- 题目：[LeetCode - 502. IPO](https://leetcode.cn/problems/ipo/)；

---

##### 加入差值绝对值直到长度固定

- 题目：给定非负数组，反复计算任意两个元素差的绝对值；新值尚不存在时只加入一份，直到数组不再增长，求最终长度；来源为笔试题，无在线测试；

---
