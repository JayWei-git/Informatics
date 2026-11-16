# Two Pointers

> Author：玖沐 (Jura)  |  Last Updated：2026/11/06

- [左程云 - 算法050 双指针](https://www.bilibili.com/video/BV1V841167Rg)；

## Theory

- 双指针是一种简单而又灵活的技巧和思想，没有特定的算法；
- 通常是根据单调性对暴力解法的优化；







## Practice

##### LC922

- 题目：[LeetCode - 922. 按奇偶排序数组 II](https://leetcode.cn/problems/sort-array-by-parity-ii/)；

  奇数偶数各一半；不能使用额外空间；

  一个指针找放在奇数索引上的偶数，一个指针找放在偶数索引上的奇数，找到了交换，然后继续，直到有一个指针越界；

  ```cpp
  vector<int> sortArrayByParityII(vector<int>& nums) {
      int n = nums.size();
      int p0 = 0, p1 = 1;
      while (p0 < n && p1 < n) {
          while (p0 < n && nums[p0] % 2 == 0) p0 += 2;
          while (p1 < n && nums[p1] % 2 != 0) p1 += 2;
          if (p1 < n && p0 < n) swap(nums[p1], nums[p0]); // 不要忘了判断越界，上面while退出条件之一就是越界
      }
      return nums;
  }
  ```

---

##### LC287

- 题目：[LeetCode - 287. 寻找重复数](https://leetcode.cn/problems/find-the-duplicate-number/)；

  `nums` 含有 `n + 1` 个整数，每个数都在 `[1, n]` 内，且只有一个不同的数重复出现；找出这个重复的数；要求不修改数组，并且只使用 $O(1)$ 额外空间；

  建模方法：将`nums`视为链表，节点编号为`i`指向第`nums[i]`节点；由于存在两个`nums[i] == nums[j]`，即这个逻辑链表存在环；

  显然这是一个单链表，那么成环位置只能在尾部，相当于找环入口；那不就是 **LC142** 吗？链表.md中收录了这个题；

  ```cpp
  int findDuplicate(vector<int>& nums) {
  	int fast = 0, slow = 0;
  	while (true) {
          fast = nums[nums[fast]];
          slow = nums[slow];
          
          if (fast == slow) {
              int p = 0;
              while (p != slow) {
  				p = nums[p];
                  slow = nums[slow];
              }
              return p; // 题目保证有环
          }
      }
  }
  ```

---

##### LC42

- 题目：[LeetCode - 42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/)；

  设`left_max[i]`代表`[0, i)`上的最大值，`right_max[i]`代表`(i, n - 1]`范围上的最大值；
  
  则第`i`根柱子上的存水量为`max(0, min(left_max[i], right_max[i]) - height[i])`；
  
  `left_max`与`right_max`的生成过程有点像DP；
  
  ```cpp
  int trap(vector<int>& height) {
      int n = height.size();
      vector<int> left_max(n, -1), right_max(n, -1); // left_max[0] == right_max[n - 1] == -1
      for (int i = 1; i < n; i++) {
          left_max[i] = max(left_max[i - 1], height[i - 1]);
          right_max[n - i - 1] = max(right_max[n - i], height[n - i]);
      }
  
      int ans = 0;
      for (int i = 0; i < n; i++) {
          ans += max(0, min(left_max[i], right_max[i]) - height[i]);
      }
  
      return ans;
  }
  ```
  
  但是真的需要将`left_max`和`right_max`全部计算出来才能累加`ans`吗？
  
  设双指针 $l=1$、$r=n-2$，用 $lmax$ 记录 $[0,l)$ 范围内的最大高度，用 $rmax$ 记录 $(r,n)$ 范围内的最大高度；初始时 $lmax=height[0]$、$rmax=height[n-1]$；
  
  例如 $height[0]=100$、$height[n-1]=80$，此时 $lmax>rmax$；虽然不知道 $r$ 左侧真正的最大高度，但它至少为 $100$，而 $r$ 右侧目前只有末尾那根高度为 $80$ 的柱子；所以 $r=n-2$ 处的水量已经由右侧的 $80$ 决定，可以计算后将 $r$ 左移；单调性出来了；
  
  一般地，当 $lmax\le rmax$ 时，$l$ 处的左侧边界较矮，计算该处水量后将 $l$ 右移；当 $lmax>rmax$ 时，$r$ 处的右侧边界较矮，计算该处水量后将 $r$ 左移；每次只需更新移动方向上的最大高度，不必保存所有位置的 $left\_max$ 和 $right\_max$；
  
  ```cpp
  int trap(vector<int>& height) {
      int n = height.size();
      int l = 1, r = n - 2;
      int lmax = height[0], rmax = height[n - 1];
      
      int ans = 0;
      // 在进入循环前，[l, r]是没有被计算的柱子
      // 所以只要区间不空，就要继续进入循环计算，即循环条件应写为 r >= l
      // 这种边界问题要仔细把握：循环做什么？什么时候继续？区间语义是什么？
      while (r >= l) {
  		if (lmax <= rmax) {
              ans += max(0, lmax - height[l]);
              lmax = max(lmax, height[l]);
              l++;
          } else {
              ans += max(0, rmax - height[r]);
              rmax = max(rmax, height[r]);
              r--;
          }
      }
      return ans;
  }
  ```

---

##### LC881

- 题目：[LeetCode - 881. 救生艇](https://leetcode.cn/problems/boats-to-save-people/)；

  先排序一下数组，然后双指针尽量把体重大的和小的组合起来；

  ```cpp
  int numRescueBoats(vector<int>& people, int limit) {
      // 1. 先对体重进行排序
      sort(people.begin(), people.end());
  
      int left = 0;                  // 指向最轻的人
      int right = people.size() - 1; // 指向最重的人
      int boats = 0;                 // 记录船的数量
  
      // 2. 双指针贪心匹配
      // [left, right]是待处理区间
      // 另外有个边界情况是left==right时这个人的体重会被视为两倍，不过不影响
      while (left <= right) {
          // 如果最重的人和最轻的人能坐同一艘船
          if (people[left] + people[right] <= limit) {
              left++; // 最轻的人成功上船，指针右移
          }
          // 最重的人无论是否与最轻的人同乘，都会占据一艘船
          right--; // 最重的人已安排，指针左移
          boats++; // 船的数量加 1
      }
  
      return boats;
  }
  ```

---

##### LC11

- 题目：[LeetCode - 11. 盛最多水的容器](https://leetcode.cn/problems/container-with-most-water/)；

  先选最左和最右的柱子，当前水量是 `min(height[l], height[r]) × (r - l)`；然后**移动较矮的那一侧**，每次计算新水量，记录最大值；

  为什么移动矮的？假设左边高度是 `2`，右边高度是 `5`；如果保留左边、只把右边往里移，宽度变小，而水位仍不可能超过 `2`，面积不可能更大；所以要尝试换一根更高的左边柱子；两边等高时，移动任意一边都可以；

  这样只扫描一遍，时间 `O(n)`，额外空间 `O(1)`；

  反证法证明不会错过最优解：[链接](https://www.bilibili.com/video/BV1V841167Rg?t=4172.1)；

  ```cpp
  int maxArea(vector<int>& height) {
  	int left = 0, right = height.size() - 1;
      int ans = 0;
      // [left, right]是待处理区间，进入循环条件是区间长度>=2
      while (right > left) {
  		ans = max(ans, min(height[left], height[right]) * (right - left));
          if (height[left] <= height[right]) // 移动较短的柱子
              left++;
          else 
              right--;
      }
      return ans;
  }
  ```

---

##### LC475

- 题目：[LeetCode - 475. 供暖器](https://leetcode.cn/problems/heaters/)；

  针对每个房屋，找出最近供暖器的距离；这些距离中的最大值，就是覆盖所有房屋所需的最小半径；

  每个房屋求最近供暖器可不要 $O(n^2)$ 循环，有单调性的；

  将房屋和供暖器分别排序；房屋从左往右遍历时，最近供暖器的位置不会回退；若下一个供暖器距离更近或相等，就把供暖器指针右移，否则当前供暖器已是最近的；
  
  ```cpp
  int findRadius(vector<int>& houses, vector<int>& heaters) {
      sort(houses.begin(), houses.end());
      sort(heaters.begin(), heaters.end());
  
      int cur_heater = 0; // 排过序了，不会回退
      int ans = 0;
      for (int i = 0; i < houses.size(); i++) { // 每个房屋都找出距离其最近的供暖器
          while (cur_heater < heaters.size() - 1) {
              int cur_dis = abs(heaters[cur_heater] - houses[i]);
              int next_dis = abs(heaters[cur_heater + 1] - houses[i]);
              if (next_dis <= cur_dis) // 不要写成next_dis < cur_dis，原因见下
                  cur_heater++;
              else
                  break;
          }
          ans = max(ans, abs(heaters[cur_heater] - houses[i])); // 更新所有房屋最近距离的最大值
      }
      return ans;
  }
  ```
  
  > 距离相等时也要右移；例如房屋在 $2$，供暖器位于 $[1,1,2]$，若只在下一个供暖器更近时右移，就会停在第一个 $1$，漏掉距离为 $0$ 的供暖器；

---

##### LC41

- 题目：[LeetCode - 41. 缺失的第一个正数](https://leetcode.cn/problems/first-missing-positive/)；

  要求时间复杂度为 $O(n)$，额外空间复杂度为 $O(1)$；

  ```cpp
  ```


---

##### LC283

- 题目：[LeetCode - 283. 移动零](https://leetcode.cn/problems/move-zeroes/)；

   非常基础的双指针，一个指针做计数器，一个指针维护区间信息；

  ```cpp
  void moveZeroes(vector<int>& nums) {
      int n = nums.size();
  
      // [0, p1)放好的非零数，[p1, p2)扫过的零，[p2, n)没有处理
  	int p1 = 0;
      for (int p2 = 0; p2 < n; p2++) {
          if (nums[p2] != 0) {
              swap(nums[p1], nums[p2]);
              p1++;
          }
      }
  }
  ```

---

