# Flood Fill

> Author：玖沐 (Jura)  |  Last Updated：2026/11/07

- [左程云 - 算法058 洪水填充](https://www.bilibili.com/video/BV1VF411S7RH)；

## Theory

- 洪水填充是在网格上，从一个起点出发，用 DFS 或 BFS 找到与它连通的所有格子；

- 遍历到格子时立刻标记，避免重复访问；标记可以直接写回网格，也可以单独用 `visited`；

- 从哪里出发，决定找什么：遍历全图可以找所有连通块（LC200、LC827）；从边界出发可以找与边界连通的格子（LC130、LC803）；

- 填充时还可以顺便统计连通块大小，或给每个连通块编号；每个格子只处理一次时，时间复杂度为 $O(n\times m)$；





## Practice

##### LC200

- 题目：[LeetCode - 200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/)；

  并查集中也见过这个题；

  洪水填充：遍历所有网格，对遇到的陆地块DFS，把与其相邻的陆地改成海洋，最后DFS的次数就是岛屿数量；

  ```cpp
  class Solution {
  private:
      void dfs(int i, int j, vector<vector<char>>& grid) {
          if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size()) return;
      	if (grid[i][j] == '0') return;
          
          grid[i][j] = '0'; // 先把自己弄成海洋，避免邻居又反复访问自己
          
          // 泛洪4个邻居
          dfs(i + 1, j, grid);
          dfs(i - 1, j, grid);
          dfs(i, j + 1, grid);
          dfs(i, j - 1, grid);
      }
      
  public:
      // 如果不想破坏现场那么复制一份grid就行了
      int numIslands(vector<vector<char>>& grid) {
          int n = grid.size(), m = grid[0].size();
          
          int ans = 0;
          for (int i = 0; i < n; i++) {
              for (int j = 0; j < m; j++) {
                  if (grid[i][j] == '1') {
                      dfs(i, j, grid);
                      ans++;
                  }
              }
          }
          
          return ans;
      }
  };
  ```

  - 时间复杂度 $O(n\times m)$；

---

##### LC130

- 题目：[LeetCode - 130. 被围绕的区域](https://leetcode.cn/problems/surrounded-regions/)；

  把不能被围住的O标记出来 (比如标记成N)，然后遍历棋盘，所有O改X，所有N换回O；

  标记不能被围住的O就用洪水填充；

  ```cpp
  class Solution {
  private:
      // 和前一个题一模一样
      void dfs(int i, int j, vector<vector<char>>& board) {
          if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size()) return;
          if (board[i][j] != 'O') return;
          
          board[i][j] = 'N';
          
          dfs(i + 1, j, board);
          dfs(i - 1, j, board);
          dfs(i, j + 1, board);
          dfs(i, j - 1, board);
      }
      
  public:
      void solve(vector<vector<char>>& board) {
  		int n = board.size(), m = board[0].size();
          
          // 从边上的O出发，才能标记不能被围住的O
          // 4个角会被访问两次，不过没影响
          for (int j = 0; j < m; ++j) {
              dfs(0, j, board);       // 上边
              dfs(n - 1, j, board);   // 下边
          }
  
          for (int i = 0; i < n; ++i) {
              dfs(i, 0, board);       // 左边
              dfs(i, m - 1, board);   // 右边
          }
          
          // 重新遍历棋盘
          for (int i = 0; i < n; i++) {
              for (int j = 0; j < m; j++) {
                  if (board[i][j] == 'O') board[i][j] = 'X';
                  if (board[i][j] == 'N') board[i][j] = 'O';
              }
          }
      }
  };
  ```

  - 时间复杂度也是 $O(n\times m)$；

---

##### LC827

- 题目：[LeetCode - 827. 最大人工岛](https://leetcode.cn/problems/making-a-large-island/)；

  预处理初始地图，统计每个岛屿的大小，然后逐个遍历翻转海洋，看哪个最大即可；

  一开始想的是直接把grid的格子值改成自身所在岛屿大小，不过翻转一个 `0` 时，四个邻格可能有两个属于同一座岛，也可能属于两座恰好一样大的岛；只看大小无法区分；

  所以可以在 DFS 时给每座岛一个唯一编号，让岛上的格子都存这个编号，并记录 `size[编号]`；

  ```cpp
  class Solution {
  private:
      void dfs(int i, int j, int index, int& size, vector<vector<int>>& grid) {
          if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size()) return;
          if (grid[i][j] != 1) return;
          
          grid[i][j] = index;
          size++;
          
          dfs(i + 1, j, index, size, grid);
          dfs(i - 1, j, index, size, grid);
          dfs(i, j + 1, index, size, grid);
          dfs(i, j - 1, index, size, grid);
      }
      
  public:
      int largestIsland(vector<vector<int>>& grid) {
          int n = grid.size(), m = grid[0].size();
          
          int island_index = 2; // 从2开始编号
          vector<int> island_size(2, -1); // 开头两个给原网格值
          
          // 把各个岛的网格值改成其index并把大小存到index里
          for (int i = 0; i < n; i++) {
              for (int j = 0; j < m; j++) {
                  if (grid[i][j] == 1) {
                      int size = 0;
                      dfs(i, j, island_index++, size, grid);
                      island_size.push_back(size);
                  }
              }
          }
          
          // 计算答案，注意去重海洋周边的岛屿
          int ans = 0;
          const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
          for (int i = 0; i < n; i++) {
  			for (int j = 0; j < m; j++) {
                  if (grid[i][j] == 0) {
                      unordered_set<int> visited;
                      int area = 1; // 别忘了自己被翻转成陆地了
                      
                      for (int k = 0; k < 4; k++) {
                          int x = i + dirs[k][0], y = j + dirs[k][1];
                          if (x < 0 || x >= n || y < 0 || y >= m) continue;
                          
                          int id = grid[x][y];
                          // insert(id).second 为 true 表示这个编号第一次出现
                          if (id >=2 && visited.insert(id).second) {
  							area += island_size[id];
                          }
                      }
                      
                      ans = max(ans, area);
                  }
              }
          }
          
          return ans == 0 ? n * m : ans; // 有个特殊情况别忘了，就是全是陆地，没有海洋让你翻转，ans保持为0
      }
  };
  ```
  
  - 实测发现常数时间不好，估计是反复创建哈希表的开销；重写了一下去重逻辑；
  
  ```cpp
  // 计算答案，注意去重海洋周边的岛屿
  int ans = 0;
  const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
  for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
          if (grid[i][j] == 0) {
              int visited[4];
              int count = 0; // 已经在四周遇见了几个岛屿的计数
              int area = 1;  // 别忘了自己被翻转成陆地了
  
              for (int k = 0; k < 4; k++) {
                  int x = i + dirs[k][0], y = j + dirs[k][1];
                  if (x < 0 || x >= n || y < 0 || y >= m) continue;
  
                  int id = grid[x][y];
                  if (id < 2) continue; // 不是岛屿
  
                  bool seen = false;
                  for (int t = 0; t < count; t++) {
                      if (visited[t] == id) { // 这个岛屿见过了
                          seen = true;
                          break;
                      }
                  }
  
                  if (!seen) { // 没见过的岛屿的大小累加进去
                      visited[count++] = id;
                      area += island_size[id];
                  }
              }
  
              ans = max(ans, area);
          }
      }
  }
  ```
  
  > 看起来代码多，但实际很简单；时间复杂度也是 $O(n\times m)$；
  >
  > 然后这个题也可以用并查集，格子编号用`i * m + j`，给每个连通块挂个`size`字段，区分岛屿就看根的编号，逻辑差不多；

---

##### LC803

- 题目：[LeetCode - 803. 打砖块](https://leetcode.cn/problems/bricks-falling-when-hit/)；

  此题正向处理非常难，但是题目一次性给出了所有炮弹的坐标，所以可以像离线查询那样，不必老老实实依次处理；
  
  可以反过来，看每发炮弹能把多少砖块“黏连”回去；过程为：
  
  首先初始状态`0`为空白，`1`为砖块；
  
  第一步，把所有炮弹击打的格子减一；
  
  第二步，从顶部DFS，把所有遍历到的砖块改为`2`，表示这些砖是稳定的；
  
  第三步，逆序处理炮弹：
  
  - 如果对应格子是`-1`，说明打到了空白，答案就是零；
  - 如果是`0`，说明打到了砖，<u>先把砖块放回改为`1`</u>，如果在顶行或者周围有`2`，从它出发DFS，把新联通的砖标记为`2`；
  
  对画线处的解释：必须要把砖块放回，比如：
  
  初始： 1 0   ，正向先敲 (1,0)，再敲 (1,1)
  	    1 1  
  
  正向敲掉(1, 0)后，(1, 1)会掉落，如果逆序时不把(1, 1)放回去，就会漏掉；
  
  ```cpp
  class Solution {
  private:
      int n, m;
      
      // 从1出发，把连通的所有1变成2，并返回感染的个数
      int dfs(int i, int j, vector<vector<int>>& grid) {
     		if (i < 0 || i >= n || j < 0 || j >= m) return 0;
          if (grid[i][j] != 1) return 0;
          
          grid[i][j] = 2;
          
          return 1 + dfs(i + 1, j, grid) + dfs(i - 1, j, grid)
                   + dfs(i, j + 1, grid) + dfs(i, j - 1, grid);
      }
      
      // 逆序处理炮弹时，要不要从某个砖块DFS
      bool worth(int i, int j, vector<vector<int>>& grid) {
          if (i == 0) return true; // 本来就在顶部
          
          const int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
          for (int k = 0; k < 4; k++) {
              int x = i + dirs[k][0], y = j + dirs[k][1];
              if (x < 0 || x >= n || y < 0 || y >= m) continue;
              if (grid[x][y] == 2) return true; // 周围有2
          }
          
          return false;
      }
      
  public:
      vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
          n = grid.size();
          m = grid[0].size();
          
          for (const auto& h : hits) {
  			grid[h[0]][h[1]]--;
          }
          
          for (int j = 0; j < m; j++) {
              if (grid[0][j] == 1) dfs(0, j, grid); // 先把顶部那些从没被击落的砖块标记了
          }
          
          int hs = hits.size();
          vector<int> ans(hs);
          for (int k = hs - 1; k >= 0; k--) {
              int x = hits[k][0], y = hits[k][1];
              if (grid[x][y] == -1) {
                  ans[k] = 0; // 打到空白了
                  continue;
              } else if (grid[x][y] == 0) {
                  grid[x][y] = 1; // 上面说的放回砖块
                  if (worth(x, y, grid)) ans[k] = dfs(x, y, grid) - 1; // 被炮弹击碎的砖块不算
              }
          }
          
          return ans;
      }
  };
  ```

  设炮弹数为 $h$：预处理和逆序遍历各花 $O(h)$；DFS 虽然会调用多次，但每块砖最多从 `1` 标记为 `2` 一次，所以所有 DFS 合起来是 $O(n\times m)$；总计 $O(n\times m+h)$；题目保证敲击位置互不相同，因此 $h\le n\times m$，可简写为 $O(n\times m)$；
