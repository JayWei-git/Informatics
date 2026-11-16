# String Hash

> Author：玖沐 (Jura)  |  Last Updated：2026/10/30

- [左程云 - 算法105 字符串哈希](https://www.bilibili.com/video/BV1hu4m1A7AF)；

## Theory

- 字符串哈希一般采用`多项式字符串哈希`；
  - 把字符串看作 $\text{Base}$ 进制的数；例如令 `a = 1`、`b = 2`、`c = 3`，取 $\text{Base}=131$，则 $\text{hash}(\text{"abc"})=1\times131^2+2\times131+3$；
  - **字符从1开始编码，避免开头出现“前导零”**；如果 `a = 0`，那么 `a` 和 `aa` 的哈希值都是0；
  - 使用64位无符号整数时，结果太大会自然溢出，相当于对 $2^{64}$ 取模；

  ```cpp
  using ull = unsigned long long;
  const ull Base = 131;
  
  ull hash_of_s = 0; // long long的溢出行为是未定义的
  for (char c : s) { // 假设s只由小写字母构成
  	hash_of_s = hash_of_s * Base + (c - 'a' + 1); // 前缀哈希值乘B相当于B进制下的左移进位
  }
  ```
  
- 这种哈希方式还有个好处：预处理所有前缀哈希和 $\text{Base}$ 的幂后，**可以在 $O(1)$ 时间计算子串哈希；**
  - 设 $H[k]=\text{hash}(s[0\dots k))$，下标从0开始，右端点不包含在内；
  - $\text{hash}(s[l\dots r))=H[r]-H[l]\times\text{Base}^{r-l}$；
  - 例如取 $\text{Base}=10$，`abc` 的前缀哈希有 $H[1]=1$、$H[3]=123$；子串 `bc` 对应 $[1,3)$，所以 $\text{hash}(\text{"bc"})=H[3]-H[1]\times10^2=23$；

  ```cpp
  int n = s.length();
  vector<ull> preHash(n + 1, 0);
  vector<ull> powBase(n + 1, 1); // 存放Base的所有次方，自然溢出即可
  
  for (int i = 1; i <= n; i++) {
      preHash[i] = preHash[i - 1] * Base + (s[i - 1] - 'a' + 1);
      powBase[i] = powBase[i - 1] * Base;
  }
  
  ull computeSubHash(vector<ull>& preHash, vector<ull>& powBase, int l, int r) {
      return preHash[r] - preHash[l] * powBase[r - l];
  }
  ```
  
- 不同字符串可能得到相同的哈希值，即哈希碰撞，不过概率极低；

- 字符串哈希常用于字符串的比对 特别是子串的比对 (模式匹配)；





## Practice

##### LG3370

- 题目：[洛谷 - P3370 [模板] 字符串哈希](https://www.luogu.com.cn/problem/P3370)；

  ```cpp
  const unsigned long long B = 499;
  unsigned long long stringHash(const string& s) {
      unsigned long long ans = 0;
  
      for (char c : s) {
          if ('0' <= c && c <= '9') {
              ans = ans * B + (c - '0' + 1);
          } else if ('a' <= c && c <= 'z') {
              ans = ans * B + (c - 'a' + 11);
          } else if ('A' <= c && c <= 'Z') {
              ans = ans * B + (c - 'A' + 37);
          }
      }
  
      return ans;
  }
  
  
  int main() {
      int N;
      cin >> N;
  
      vector<unsigned long long> h(N);
      for (int i = 0; i < N; i++) {
          string s;
          cin >> s;
          h[i] = stringHash(s);
      }
  
      sort(h.begin(), h.end());
  
      int ans = 0;
      for (int i = 0; i < N; i++) { // 有多少个不同的数就是多少个不同的字符串
          if (i == 0 || h[i] != h[i - 1]) {
              ans++;
          }
      }
  
      cout << ans << '\n';
      return 0;
  }
  ```

---

##### LC28

- 题目：[LeetCode - 28. 找出字符串中第一个匹配项的下标](https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/)；

  这道题用KMP做过，现在用子串哈希；

  ```cpp
  using ull = unsigned long long;
  int strStr(string haystack, string needle) {
      int n = haystack.length(), m = needle.length();
  
      const ull B = 499;
      vector<ull> ph(n + 1, 0); // pre_hash
      vector<ull> pow(n + 1, 1);
      for (int i = 1; i <= n; i++) {
          ph[i] = ph[i - 1] * B + (haystack[i - 1] - 'a' + 1);
          pow[i] = pow[i - 1] * B;
      }
  
      ull needleHash = 0;
      for (int j = 0; j < m; j++) {
          needleHash = needleHash * B + (needle[j] - 'a' + 1);
      }
  
      // k + m <= n 即 k <= n - m
      for (int k = 0; k <= n - m; k++) {
          ull hashSub = (ph[k + m] - ph[k] * pow[m]);
          if (hashSub == needleHash) return k;
      }
  
      return -1;
  }
  ```

  > KMP.md 中的**LC686**也可以这样做，就是把子串匹配换成了枚举子串哈希罢了；

---

##### LC30

- 题目：[LeetCode - 30. 串联所有单词的子串](https://leetcode.cn/problems/substring-with-concatenation-of-all-words/)；

  ```cpp
  vector<int> findSubstring(string s, vector<string>& words) {
  
  }
  ```

---

##### LG3763

- 题目：[洛谷 - P3763 DNA](https://www.luogu.com.cn/problem/P3763)；

  允许k个容错的模式匹配；

  由于求的是能配上的数量，所以需要枚举T的所有子串，然后和P比较看有多少个字符不同；

  如何快速判定两个等长字符串有多少个不同字符？用二分法不断找最长相同前缀；

  期间要多次比对子串是否先相同，用字符串哈希；

  ```cpp
  // 用一个类来管理字符串哈希
  class StringHash {
  private:
      vector<ull> hash, pow;
      ull hashCode(char c) {
          switch(c) {
              case 'A' : return 1ll;
              case 'T' : return 2ll;
              case 'C' : return 3ll;
              case 'G' : return 4ll;
          }
      }
  
  public:
      StringHash(const string& s) {
          // 注意要开成s.length() + 1，因为采取的是左闭右开
          hash.assign(s.length() + 1, 0);
          pow.assign(s.length() + 1, 1);
  
          for (int i = 1; i <= s.length(); i++) {
              hash[i] = hash[i - 1] * B + hashCode(s[i - 1]);
              pow[i] = pow[i - 1] * B;
          }
      }
  
      ull get(int l, int r) {
          return hash[r] - hash[l] * pow[r - l];
      }
  };
  ```
  
  - 然后是`main`函数：
  
  ```cpp
  int main() {
      int cases, k = 3; // 此题默认k == 3
      cin >> cases;
      while (cases--) {
          string text, pattern;
          cin >> text >> pattern;
          StringHash textHash(text), patternHash(pattern);
          int n = text.length(), m = pattern.length();
          
          int ans = 0;
          for (int i = 0; i <= n - m; i++) { // 枚举text的所有子串
              int matched_len = 0, diffs = 0;
              while (matched_len < m && diffs <= k) { // 开始多次二分找不相同的字符，注意while条件
                  // 计数器based on pattern
                  int left = matched_len, right = m;
                  while (left < right) {
                      int mid = left + (right - left) / 2;
                      // 注意这里的二分边界！！！！小心空区间陷阱
                      if (textHash.get(i + left, i + mid + 1) == patternHash.get(left, mid + 1)) {
                          left = mid + 1;
                      } else {
                          right = mid;
                      }
                  }
                  // 当前二分结束，left/right停留在matched_len后首个不相同字符(全相同则停在结尾)
                  if (left == m) {
                      matched_len = m;
                  } else {
                      matched_len = left + 1;
                      diffs++;
                  }
              }
              if (diffs <= k) ans++; // 所有二分结束，看不同字符是否满足条件
          }
          cout << ans << "\n"; // 给出当前case的ans
      }
      return 0;
  }
  ```
  
  > 此题的二分区间边界问题卡了我很久，仔细理解，尤其是空区间陷阱；
