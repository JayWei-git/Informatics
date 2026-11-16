# Unite-Find Set

> Author：玖沐 (Jura)  |  Last Updated：2026/11/05

- [左程云 - 算法056 并查集上](https://www.bilibili.com/video/BV1894y1W7Sb)；
- [左程云 - 算法057 并查集下](https://www.bilibili.com/video/BV1Ny4y1F71J)；

## Theory

##### Basic Template

- 并查集是一种逻辑上的树 / 森林结构，用于处理**不相交**集合的合并与查询问题；

  - 一棵树对应一个集合，根节点作为该集合的代表元素；

- 用一个一维数组 `parent[ ]` 维护森林结构：

  - `parent[x]` 表示节点 $x$ 的父节点；
  - 若 `parent[x] == x`，说明 $x$ 是当前树的**根节点** (也是该集合的唯一标识)；
  - **初始化**：每个元素自成一个集合，即 $parent[i] = i$；

- 两大优化：

  1. 路径压缩：在执行`find(x)`时，将路径上经过的所有节点直接挂载到根节点上；
  2. 按秩合并：记录每棵树的高度 (rank) 或节点数 (size)，合并时将秩小的树根挂到秩大的树根下方；

  - 双优化下，单次操作平摊复杂度为 $O(\alpha(N))$ (反阿克曼函数，常数级)；

- 标准实现：

  [牛客 - 并查集的实现](https://www.nowcoder.com/practice/e7ed657974934a30b2010046536a5372)；[洛谷 - P3367 [模板] 并查集](https://www.luogu.com.cn/problem/P3367)；

  ```cpp
  class UniteFindSet {
  private:
      vector<int> parent;
      vector<int> rank; // 只有根的rank值才有意义，其余都是垃圾值
      
  public:
      UniteFindSet(int n) : parent(n), rank(n, 0) {
          for (int i = 0; i < n; i++) {
              parent[i] = i;
          }
      }
      
      int find(int x) {
          return parent[x] == x ? x : parent[x] = find(parent[x]); // 递归实现路径压缩
      }
      
      bool unite(int x, int y) {
          int rootX = find(x), rootY = find(y);
          
          if (rootX == rootY) { return false; } // 已在同一集合
          
          if (rank[rootX] < rank[rootY]) { // 按秩大小合并，这里秩是树高度
              parent[rootX] = rootY;
          } else if (rank[rootX] > rank[rootY]) {
              parent[rootY] = rootX;
          } else { // 秩相等随便哪个被合并
              parent[rootX] = rootY; 
              rank[rootY]++;  // 秩递增
          }
          
          return true; 
      }
  };
  ```
  
  > 路径压缩时，树高可能会降低，所以这里的rank实际是用于比较的高度上限，不过理解为树高也没关系；

---

##### Extensions







## Practice

##### (???) LC765

- 题目：[LeetCode - 765. 情侣牵手](https://leetcode.cn/problems/couples-holding-hands/)；

  ```cpp
  class UniteFindSet {
  private:
      vector<int> parent;
      int sets; // 并查集中有多少棵树 / 多少个不相交集合
      
  public:
      UniteFindSet (int n) : parent(n), sets(n) { for (int i = 0; i < n; i++) { parent[i] = i; }}
      int find(int x) { return x == parent[x] ? x : parent[x] = find(parent[x]); }
  
      void unite(int x, int y) {
          int rootx = find(x), rooty = find(y);
          if (rootx != rooty) {
              parent[rootx] = rooty;
              sets--; // 合并后集合数量-1
          }
      }
  
      int size() { return sets; } // 提供一个公共方法查询sets
  };
  
  class Solution {
  public:
      int minSwapsCouples(vector<int>& row) {
          int n = row.size();
          UniteFindSet UFS(n / 2);
          
          // 逻辑就4行，但理解难度可不小
          for (int i = 0; i < n; i += 2) {
              UFS.unite(row[i] / 2, row[i + 1] / 2);
          }
          return n / 2 - UFS.size();
      }
  };
  ```

---

##### LC839

- 题目：[LeetCode - 839. 相似字符串组](https://leetcode.cn/problems/similar-string-groups/)；

  很显然要使用并查集了，两个单词相似就合并到同一个集合中，最后返回并查集中集合数量即可；

  找出所有相似关系必须两两比对所有单词，没招，只能两个`for`；

  然后怎么判断是否相似呢，题目中说了都是异构词，即所含字母相同只是排列不同，那逐位置遍历一下看几个索引不同不就行了；

  ```cpp
  class UniteFindSet { ... }; // 和LC765一致，新增一个sets字段，略
  
  class Solution {
  private:
      bool similar(const string& s1, const string& s2) {
          int len = s1.length();
          int diffs = 0;
          for (int i = 0; i < len; i++) {
              if (s1[i] != s2[i]) diffs++;
              if (diffs > 2) return false; // 直接剪掉，优化常数时间
          }
          return diffs == 0 || diffs == 2; // 其实这里可以直接return true，异位词不同位置数量只可能是偶数
      }
  
  public:
      int numSimilarGroups(vector<string>& strs) {
          int n = strs.size();
          UniteFindSet UFS(n);
          for (int i = 0; i < n; i++) {
              for (int j = i + 1; j < n; j++) {
                  if (UFS.find(i) == UFS.find(j)) continue; // 直接跳过已在同一集合的
                  if (similar(strs[i], strs[j])) {
                      UFS.unite(i, j);
                  }
              }
          }
          return UFS.size();
      }
  };
  ```

  > 第三个注释这里解释一下：例如 ["abc", "acb", "bac"]：
  >
  > - 比较 (0, 1)：相似，合并；
  > - 比较 (0, 2)：相似，合并；
  > - 轮到 (1, 2)：这对下标第一次出现，但 1 和 2 已经通过 0 连在同一组了，无需调用`similar`；

---

##### LC200

- 题目：[LeetCode - 200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/)；

  斜着相邻的陆地方块不算同一座岛屿；

  ```cpp
  class UniteFindSet { ... }; // 和LC765一致，新增一个sets字段，略
  
  int numIslands(vector<vector<char>>& grid) {
      int n = grid.size(), m = grid[0].size();
      UniteFindSet UFS(n * m);
      int waters = 0;
  
      for (int i = 0; i < n; i++) {
          for (int j = 0; j < m; j++) {
              if (grid[i][j] == '0') {
                  waters++;
                  continue; // 跳过水
              }
  
              int id = i * m + j;
              if (j > 0 && grid[i][j - 1] == '1')
                  UFS.unite(id, id - 1); // 与左边那块陆地合并
              if (i > 0 && grid[i - 1][j] == '1')
                  UFS.unite(id, id - m); // 与上边那块陆地合并
          }
      }
  
      return UFS.size() - waters; // 减去水的数量
  }
  ```

  > 此题需要注意的是陆地方块采取的编号是`i * m + j`那么并查集初始大小开的是`n * m`，但是这里面是包括了水方块的数量，所以最后要减去；
  >
  > 然后由于遍历方向是自上而下自左而右所以每块陆地不用看四个方向只用看左和上就行了；

  - 此题还可以洪水填充：遍历所有网格，对遇到的陆地块DFS，把与其相邻的陆地改成海洋，最后DFS的次数就是岛屿数量；


---

##### LC947

- 题目：[LeetCode - 947. 移除最多的同行或同列石头](https://leetcode.cn/problems/most-stones-removed-with-same-row-or-column/)；

  注意题意，两块石头只要同行或者同列就直接相连，无论距离多远；通过其他石头也可以间接连通；

  虽然消除石头的数量和顺序有关，不过最大消除数量总是连通块内总个数 - 1；

  例如三块石头：`A = (0,0)    B = (0,1)    C = (1,1)`，先移 B：A、C 都没法再移，只移掉 `1` 块；先移 A，再移 B：最后留 C，移掉 `2` 块；

  最终答案就是 石头总数 - 连通块数；

  合并石头时，无需双重`for`两两检查所有石头，可以分别记录每一行、每一列第一次遇到的石头作为代表；后续遇到同一行或同一列的石头，只需与对应的代表石头合并；这样只需遍历石头一次，使用哈希表时平均时间复杂度为 $O(n\alpha(n))$，空间复杂度为 $O(n)$；

  ```cpp
  class UniteFindSet { ... }; // 和LC765一致，新增一个sets字段，略
  
  int removeStones(vector<vector<int>>& stones) {
      int n = stones.size();
      UniteFindSet UFS(n);
  
      unordered_map<int, int> row_first, col_first;
  
      for (int i = 0; i < n; i++) {
          int row = stones[i][0], col = stones[i][1];
  
          auto p = row_first.find(row);
          if (p == row_first.end()) row_first[row] = i; // 记下这一行第一次遇见的石头是i
          else UFS.unite(i, p->second); // 这一行有代表石头了，将i与其合并
  
          auto p2 = col_first.find(col);
          if (p2 == col_first.end()) col_first[col] = i; // 这一列第一次遇见的石头是i
          else UFS.unite(i, p2->second); // 这一列有代表石头了，将i与其合并
      }
  
      return n - UFS.size();
  }
  ```

---

##### LC2092

- 题目：[LeetCode - 2092. 找出知晓秘密的所有专家](https://leetcode.cn/problems/find-all-people-with-secret/)；

  一开始的想法是按时间排序后逐个遍历会议即可，但是在此题这种情形下是会漏信息的，比如假设此时只有`{0, 3}`知道秘密，然后遍历会议`[2, 1, 1]`，`[2, 3, 1]`，会漏掉`1`也得知了秘密 (专家掌握了影分身技术，可以在同一时刻参加多场会议；)；

  所以应该一次性处理同一时刻发生的所有会议，先建立当前时刻的所有小团体，然后重新遍历一次，把没有分享秘密的小团体拆了，避免干扰后续；

  ```cpp
  class UniteFindSet {
  	... // 和模板一致
      void reset(int x) { parent[x] = x; } // 提供一个公共方法，用于拆除集合
  };
  
  vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) {
      // 1. 将会议按开始时间排序
      sort(meetings.begin(), meetings.end(), 
          [](const vector<int>& m1, const vector<int>& m2) {
              return m1[2] < m2[2];
      });
  
      UniteFindSet UFS(n);
      UFS.unite(0, firstPerson); // 别忘了一开始0号专家就和firstPerson分享了
  
      int i = 0; // meetings[i, j)为相同时间召开的会议
      while (i < meetings.size()) {
          // 2. 建立当前时刻的所有小团体
          UFS.unite(meetings[i][0], meetings[i][1]);
          int j = i + 1;
          while (j < meetings.size() && meetings[j][2] == meetings[i][2]) {
              UFS.unite(meetings[j][0], meetings[j][1]);
              j++;
          }
  
          // 3. 再遍历一次，没有共享秘密的小团体拆开
          for (int k = i; k < j; k++) {
              if (UFS.find(meetings[k][0]) != UFS.find(0)) { // 与会双方其一不知道秘密说明两人都不知道
                  UFS.reset(meetings[k][0]);
                  UFS.reset(meetings[k][1]);
              }
          }
  
          i = j; // 4. 推进计数器，重复上述过程
      }
  
      // 5. 遍历所有专家，和0号在一起的就是共享了信息的
      vector<int> ans;
      for (int p = 0; p < n; p++) {
          if (UFS.find(p) == UFS.find(0)) {
              ans.push_back(p);
          }
      }
  
      return ans;
  }
  ```

---

##### LC2421

- 题目：[LeetCode - 2421. 好路径的数目](https://leetcode.cn/problems/number-of-good-paths/)；
- 题目说的路径包含单节点路径，下文不考虑这种路径，只需在最后答案中加上顶点个数 $n$ 即可；

1. 两个初步认知：

   - 对于无向无环连通图，也就是树，任意两异顶点间有且仅有一条路径；

   - 对于某个连通块，其中最大值顶点数量记为 $k$，那么以该最大值开始结尾的好路径的数量为 $k*(k-1)/2$；

     其他以非最大值开始结尾的好路径数量是无法计算的，因为可能有大值顶点作桥；

   那么要计算所有好路径的数量，只能按顶点值升序处理；

2. 初步方法：

   按顶点值升序依次点亮顶点，每点亮一批值为 $v$ 的顶点，只统计包含这批顶点的连通块中值为 $v$ 的顶点数，计算出好路径数并累加到答案中；

   实现时在点亮顶点的时候为了拿到联通信息，还需要看它已点亮的邻居，这就需要建图；就不展示代码了；

4. 进阶做法：

   不点亮顶点，而是点亮边；

   首先将边排序，排序的键值为该边两端点中的较大值；

   用并查集维护顶点的联通关系，初始没有边被点亮；

   每点亮一条边，就把新增的好路径数算出来；

   首先点亮边必然导致两个连通块被合并 (因为图是树)(哪怕是只有一个顶点的连通块)，且新增路径数只会来源于跨被合并的两个连通块的路径；

   设当前点亮的边为 $(x, y)$，排序值为 $v = max(vals[x], vals[y])$；由于边按排序值升序加入，当前边两端所在的连通块中，顶点值都不大于 $v$；而这条边至少有一个端点的值恰好为 $v$；

   任何新增路径都要经过这条边，因此必经过一个值为 $v$ 的顶点；若它是好路径，两端的值就不能小于 $v$；同时，两端所在连通块中也没有大于 $v$ 的顶点；所以，新增好路径的两端必须都**等于** $v$；
   
   接下来只需比较两个待合并连通块的最大值：
   
   - 最大值不等：其中一块没有值为 $v$ 的顶点，新增 0 条；
   - 最大值相等：两块的最大值都为 $v$；若各有 $a、b$ 个最大值顶点，分别从两块选一个端点，新增 $a × b$ 条；
   
   因此，并查集要维护每个块的最大值及其出现次数；
   
   ```cpp
   class UniteFindSet {
   private:
       vector<int> parent;
       vector<int> maxv, maxn; // 分别存最大值和最大值数量，只有根的值才有意义
   
   public:
       UniteFindSet(int n, const vector<int>& vals) : parent(n), maxv(n), maxn(n) {
           for (int i = 0; i < n; i++) {
               parent[i] = i;
               maxv[i] = vals[i]; // 注意这里的初始化
               maxn[i] = 1;
           }
       }
   
       int find(int x) { return x == parent[x] ? x : parent[x] = find(parent[x]); }
   
       int unite(int x, int y) {
           int new_paths = 0;
           int rx = find(x), ry = find(y);
           if (rx != ry) {
               // 计算此次合并将产生多少新的好路径，调用者保证按键升序加边
               if (maxv[rx] == maxv[ry]) new_paths = maxn[rx] * maxn[ry];
   
               parent[rx] = ry; // 合并两个连通块
   
               // 合并后更新最大值及最大值数量
               if (maxv[rx] > maxv[ry]) {
                   maxv[ry] = maxv[rx];
                   maxn[ry] = maxn[rx];
               }
               else if (maxv[rx] == maxv[ry]) {
                   maxn[ry] += maxn[rx];
               }
           }
   
           return new_paths;
       }
   };
   
   class Solution {
   public:
       int numberOfGoodPaths(vector<int>& vals, vector<vector<int>>& edges) {
           auto value = [&vals](const vector<int>& e) { // 这里定义一个返回边的排序键值的仿函数
               return max(vals[e[0]], vals[e[1]]);
           };
   
           sort(edges.begin(), edges.end(), [&](const auto& e1, const auto& e2) { // 排序边
               return value(e1) < value(e2);
           });
   
           int n = vals.size();
           UniteFindSet UFS(n, vals);
           int ans = n; // 单节点路径也算好路径
   
           for (const auto& edge : edges) {
               ans += UFS.unite(edge[0], edge[1]);
           }
   
           return ans;
       }
   };
   ```
   
   > 难爆了，难炸了；

---

##### LC928

- 题目：[LeetCode - 928. 尽量减少恶意软件的传播 II](https://leetcode.cn/problems/minimize-malware-spread-ii/)；

  一个感染体会感染整个连通块；
  
  先合并未感染顶点为多个连通块，看每一块与多少个感染源相连：
  
  - 只连着一个感染源 (哪怕多条边连向同一个感染源)，删除该感染源后，这块得救；
  - 连着多个，没救，删哪个都会被其它感染源感染；
  - 一个也没连，那本来就不会被感染；
  
  同一个感染源 有可能有多个块分别只连着它，要累加；
  
  所以并查集要维护的额外信息：连通块中顶点数量，连通块连接了哪些感染源；
  
  ```cpp
  class UniteFindSet {
  private:
      vector<int> parent;
      vector<int> size;   // 连通块大小，只有根的值有意义
      vector<int> infect; // 连通块连了哪些感染源，-1没有，x(x>=0)连感染源x，-2连了多个不同的感染源，依然是只有根的值有意义
      
  public:
      UniteFindSet(int n) : parent(n), size(n), infect(n) {
      	for (int i = 0; i < n; i++) {
          	parent[i] = i;
              size[i] = 1;
              infect[i] = -1;
          }
  	}
      
      int find(int x) { return x == parent[x] ? x : parent[x] = find(parent[x]); }
      
      void unite(int x, int y) {
          int rx = find(x), ry = find(y);
          if (rx != ry) {
              parent[rx] = ry;
              size[ry] += size[rx];
              // unite不需要更新infect，逻辑是先合并所有未感染顶点
          }
      }
      
      void setInfect(int x, int v) {
          int rx = find(x);
          int irx = infect[rx];
          if (irx == -1) infect[rx] = v;
          // 注意这一行，因为同一块是可能有多条边连接同一个感染源，此时不能重复计算
          if (irx >= 0 && irx != v) infect[rx] = -2;
      }
      
      // 调用者要保证传入的是根
      int getInfect(int root) { return infect[root]; }
      int getSize(int root) { return size[root]; }
  };
  
  class Solution {
  public:
      int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial) {
          // 用哈希表快速查询一个顶点是不是感染源
          unordered_set<int> virus; 
          for (int x : initial) {
              virus.insert(x);
          }
          
          // 联通所有未感染顶点
          int n = graph.size();
          UniteFindSet S(n);
          for (int i = 0; i < n; i++) { // 遍历邻接矩阵上三角及对角线即可
              for (int j = i; j < n; j++) {
                  if (graph[i][j] == 1 && virus.count(i) == 0 && virus.count(j) == 0)
                      S.unite(i, j);
              }
          }
          
          // 设置每个连通块的感染源
          for (int sick : virus) {
              for (int i = 0; i < n; i++) {
                  if (graph[sick][i] == 1 && virus.count(i) == 0) {
  					S.setInfect(i, sick);
                  }
              }
          }
          
          // saved[infect]为删去感染源infect后能拯救的顶点数
          vector<int> saved(n, 0);
          for (int i = 0; i < n; i++) {
              // 不是根，或者是感染源，不用看
              if (S.find(i) != i || virus.count(i) != 0) continue;
              
              int infect = S.getInfect(i);
              if (infect >= 0) { // 这个连通块只连接了一个感染源，可以被拯救
                  saved[infect] += S.getSize(i);
              }
          }
          
          // 找能拯救最多的感染源
          sort(initial.begin(), initial.end()); // 题目要求相同返回索引较小的
          int ans = initial[0];
          for (int x : initial) {
              if (saved[x] > saved[ans]) ans = x;
          }
          return ans;
      }
  };
  ```
  
  > 注意注释中提到的细节；

---



