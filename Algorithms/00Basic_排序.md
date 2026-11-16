# Sort Algorithms

> Author：玖沐 (Jura)  |  Last Updated：2026/10/25

- [左程云 - 算法021 归并排序](https://www.bilibili.com/video/BV1wu411p7r7)；
- [左程云 - 算法022 归并分治](https://www.bilibili.com/video/BV1L14y1B7ef)；
- [左程云 - 算法023 随机快速排序](https://www.bilibili.com/video/BV1cc411F7Y6)；
- [左程云 - 算法024 随机选择算法](https://www.bilibili.com/video/BV1mN411b71K)；
- [左程云 - 算法025 堆结构和堆排序](https://www.bilibili.com/video/BV1fu4y1q77y)；
- [左程云 - 算法027 堆结构常见题](https://www.bilibili.com/video/BV1Gm4y1p7UE)；
- [左程云 - 算法028 基数排序](https://www.bilibili.com/video/BV1Lh4y1c7Aw)；
- [左程云 - 算法029 重要排序算法的总结](https://www.bilibili.com/video/BV1Kh4y1c7Pu)；

## Theory

- 三傻排序略；

##### Merge Sort

- 左边排好序，右边也排好序，然后归并`merge`左右两边；

  ```cpp
  // 引用传递help可避免多次递归开辟临时数组的开销
  void mergeSort(vector<int>& arr, vector<int>& help, int left, int right) {
  	if (right - left <= 1) return;
  
  	int mid = left + ((right - left) >> 1); // 左闭右开[left, mid) [mid, right)
  	mergeSort(arr, help, left, mid);  // 左边排好序
  	mergeSort(arr, help, mid, right); // 右边排好序
  
  	// merge左右两边
  	int i = left, j = mid;
  	int k = left;
  	while (i < mid && j < right) { // 归并
  		if (arr[i] <= arr[j]) help[k++] = arr[i++]; // 相等选左边，保证稳定性
  		else help[k++] = arr[j++];
  	}
  	// 处理剩余部分
  	while (i < mid) help[k++] = arr[i++];
  	while (j < right) help[k++] = arr[j++];
  
  	for (int p = left; p < right; p++) { arr[p] = help[p]; }
  }
  ```

  > 开头递归终止条件别忘了；记住区间语义为左闭右开；
  
  - 再来看看非递归版，没啥好说的，自底向上不断`merge`就行了
  
  ```cpp
  void mergeSort(vector<int>& arr) {
      int n = arr.size();
      vector<int> help(n);
      
      for (int ml = 1; ml < n; ml <<= 1) { // merge_length
          int left = 0; // ml翻倍，left重置，从数组最左边开始
          while (left < n) {
              int mid = min(left + ml, n); // 防止越界
              int right = min(mid + ml, n);
              
              // 归并[left, mid)和[mid, right)，和递归版的一致
              int i = left, j = mid, k = left;
              while (i < mid && j < right) {
  				if (arr[i] <= arr[j]) help[k++] = arr[i++]; // 记得相等选左边
                  else help[k++] = arr[j++];
              }
              while (i < mid) help[k++] = arr[i++];
              while (j < right) help[k++] = arr[j++];
              for (int p = left; p < right; p++) { arr[p] = help[p]; }
              
              // 去下一组
              left = right;
          }
      }
  }
  ```
  
  > 归并排序本身就是一种分治算法，其核心是处理左处理右再处理跨左右，且在左右被处理后不影响处理跨左右 (甚至更容易)；
  >
  > 有的问题可以用这种分治思路解决，称为归并分治；

---

##### Quick Sort

- 首先看最常用的三路划分快排：随机选取一个基准值 / 枢轴`pivot`，$<$`pivot`的放左边，$>$`pivot`的放右边，中间 $==$`pivot`不递归处理；

  ```cpp
  void quickSort(vector<int>& arr, int left, int right) {
  	if (right - left <= 1) return;
  
  	int pivot = arr[left + (rand() % (right - left))];
      // [left, smaller) < pivot, [bigger, right) > pivot
      // 计数器都是based on整个arr的，然后注意初始值，两边一开始是空区间，i从left开始
  	int smaller = left, bigger = right;
  	int i = left;
  
  	while (i < bigger) {
  		if (arr[i] == pivot) {
  			i++; // 相等直接跳过，不加入任何一边
  		} else if (arr[i] > pivot) {
              // i不要递增，右边换过来的元素没有检查
  			swap(arr[i], arr[--bigger]); // 先--bigger再交换，因为bigger是左闭，smaller才是右开
  		} else {
  			swap(arr[i++], arr[smaller++]); // 左边换过来的元素检查过了
  		}
  	}
      
  	quickSort(arr, left, smaller);
  	quickSort(arr, bigger, right);
  }
  ```

  > 三路划分一个很重要的特点是：`[smaller, bigger)`之间`== pivot`的元素放好了就永远不动了，`[smaller, bigger)`就是它们最终排序结果的索引；

- 然后应试中可能会出现二路划分快排，$\le$`pivot`的放左边，$>$`pivot`的放右边，然后只把**一个**`pivot`移到最终正确位置，接着递归处理左右两边；

  - 所以当数组所有元素全部相等时，二路划分会退化到 $O(n^2)$；

  ```cpp
  void quickSort(vector<int>& arr, int left, int right) {
      if (right - left <= 1) return;
      int pivot = arr[left + (rand() % (right - left))];
      int bound = left; // [left, bound) <=pivot, [bound, right) >pivot
      int pivot_index;  // pivot的索引，随便记一个就行
      
      for (int i = left; i < right; i++) {
          if (arr[i] <= pivot) {
              swap(arr[bound], arr[i]);
              if (arr[bound] == pivot) pivot_index = bound; // 已经swap了
              bound++; // 扩充<=pivot的区间
          }
      }
      
      swap(arr[pivot_index], arr[bound - 1]); // 只把一个pivot移动正确位置
      
      quickSort(arr, left, bound - 1);
      quickSort(arr, bound, right);
  }
  ```

---

##### Heap Sort

- 利用 二叉堆 这一数据结构实现的排序算法；本质是一种选择排序，利用堆加快了选择这个过程；

- 注：二叉堆，堆底层是一颗被存储为数组的完全二叉树；0-based索引下：

  - 父节点索引为 $p$，则左孩子为 $2*p+1$，右孩子为 $2*p+2$；

  - 最后一个非叶节点索引 可由 最后一个左孩子索引 推出；

  ```cpp
  void heapSort(vector<int>& arr) {
      // 为了实现O(1)的额外空间，一般原地建堆
      // 将arr视为大根堆，叶节点符合要求，从最后的非叶节点开始调整(下沉)
      int n = arr.size();
      for (int i = n / 2 - 1; i >= 0; --i) {
  		shiftDown(arr, i, n);
      }
  
      // 堆建好了，堆顶是最大值
      // 不断把堆顶换到末尾的有序区，新堆顶重新下沉即可
      int sorted = n; // 有序区为[sorted, n)，[0, sorted)是还在处理的堆
      while (sorted > 1) { // 堆中还有两个元素时再处理一次就只剩一个元素，就不用处理了
          swap(arr[0], arr[--sorted]); // 注意左闭边界是先减再使用
          shiftDown(arr, 0, sorted); // 下沉新的堆顶
      }
  }
  
  // 堆下沉操作
  // 这里的思路比较有意思，我们并没有在选出better child后就交换arr[i]和arr[better]
  // 而是先保存最初的arr[i]，然后将i位置视为空位，让better child不断上浮，空位不断下沉
  // 从而省去了swap开销，有的工程元素swap开销比较大，这样是更合理的
  void shiftDown(vector<int>& arr, int i, int heapSize) {
  	int n = heapSize;
      int val = arr[i];
  
      while (2 * i + 1 < n) { // 没有left child说明到叶了，下沉结束
          // 选出更大的child(大根堆)
          int left = i * 2 + 1, right = left + 1;
          int better = left;
          if (right < n && arr[right] > arr[left]) {
              better = right;
          }
  
          if (val >= arr[better]) break; // 空位下沉到了合适位置
  
          arr[i] = arr[better]; // better child上移
          i = better; // 空位下沉来到better child
      }
  
      arr[i] = val; // 把原值赋回空位
  }
  ```

  > 注意建堆从最后一个非叶子开始下沉 (不是必需的)；

---

##### Radix Sort

- 不基于比较的排序，只适用于整数间排序；具体原理难以叙述清晰，推荐重温讲解视频；

  ```cpp
  void radixSort(vector<int>& arr, int base = 10) {
      if (arr.empty() || base < 2) return; // 0进制和1进制是什么鬼
      int n = arr.size();
      
      int max_val = arr[0];
      for (int i = 1; i < n; i++) {
          max_val = max(max_val, arr[i]);
      }
      
      // offset表示此轮循环在按照第几位数字进行排序
  	// 只要 最大值 / offset 不为0，说明还得继续
      for (int offset = 1; max_val / offset > 0; offset *= base) {
          countSort(arr, offset, base);
      }
  }
  
  // 计数排序
  // 按第offset位排序，通过 前缀和 + 逆序遍历 保证稳定性
  void countSort(vector<int>& arr, int offset, int base) {
      int n = arr.size();
      vector<int> output(n);
      vector<int> count(base, 0);
  	
      // 1. 统计频率并计算前缀和
      for (int i = 0; i < n; i++) {
          int digit = (arr[i] / offset) % base; // base进制下，arr[i]在第offset位上的数字
          count[digit]++;
      }
      
      for (int i = 1; i < base; i++) {
          count[i] += count[i - 1];
      }
      
      // 2. 依据前缀和，逆序构建output，并将结果写回
      for (int i = n - 1; i >= 0; i--) {
          int digit = (arr[i] / offset) % base;
          output[--count[digit]] = arr[i];
      }
      
      for (int i = 0; i < n; i++) {
          arr[i] = output[i];
      }
  }
  ```

  - 然后如果有负数，需要整体平移一下：

  ```cpp
  vector<int> sortArray(vector<int>& nums) {
      if (nums.empty()) return nums;
      int min_val = *min_element(nums.begin(), nums.end());
  
      if (min_val < 0) { // 最小值是负的，需要平移
        for (int& num : nums) num -= min_val; // 注意是减，别加一个负数
      }
  
      radixSort(nums);
  
      if (min_val < 0) { // 最小值是负的，排完序需要还原
        for (int& num : nums) num += min_val;   
      }
  
      return nums;
  }
  ```







## Practice

##### Template

- [牛客 - 快速排序](https://www.nowcoder.com/practice/3385982ae71d4a1ca8bf3d03614c0325)；
- [牛客 - 堆排序](https://www.nowcoder.com/practice/ed8308a5f0f744fc936bdba78c15f810)；
- [LeetCode - 912. 排序数组](https://leetcode.cn/problems/sort-an-array)；
-  [洛谷 - P1177 [模板] 排序](https://www.luogu.com.cn/problem/P1177)；

---

##### NC - 数组小和

- 题目：[牛客 - 计算数组的小和](https://www.nowcoder.com/practice/edfe05a1d45c4ea89101d936cac32469)；

- 归并分治：

  - 整个范围上的`ans` = 左侧`ans` + 右侧`ans` + 跨越左右的`ans`；
  - 左右两侧各自有序 能让 计算跨越左右的`ans` 更容易；

   ```cpp
   // 设smallSum(left, right)的语义是返回[left, right)上的小和并排好序，据此编写递归
   // [left, mid)和[mid, right)内部顺序不改变crossSum，相反排好序后还更容易计算crossSum
   long long smallSum(vector<int>& arr, vector<int>& help, int left, int right) { // 实测发现int会溢出
       if (right - left <= 1) return 0; // 小和为0
   
       int mid = left + ((right - left) >> 1);
       long long ans = smallSum(arr, help, left, mid) + smallSum(arr, help, mid, right);
   
       // merge并计算跨左右的crossSum
       int i = left, j = mid, k = left;
       long long crossSum = 0;
       while (i < mid && j < right) {
           if (arr[i] <= arr[j]) {
               // arr[i]<=arr[j]意味着arr[i]<=所有[j,right)，一次计算其贡献
               crossSum += 1LL * arr[i] * (right - j);
               help[k++] = arr[i++];
           } else {
               help[k++] = arr[j++];
           }
       }
       while (i < mid) help[k++] = arr[i++];
       while (j < right) help[k++] = arr[j++];
       for (int p = left; p < right; p++) { arr[p] = help[p]; }
   
       return ans + crossSum;
   }
   // main读入数据直接call smallSum即可，略
   ```

---

##### LC493

- 题目：[LeetCode - 493. 翻转对](https://leetcode.cn/problems/reverse-pairs)；

   符合归并分治；
   
   ```cpp
   // 设mergeDivide的语义是返回[left,right)上翻转对的数量并排序数组，编写递归即可
   // [left, mid)和[mid, right)内部顺序不改变crossAns，相反排好序后还更容易计算crossAns
   int mergeDivide(vector<int>& arr, vector<int>& help, int left, int right) {
       if (right - left <= 1) return 0;
   
       int mid = left + ((right - left) >> 1);
       int ans = mergeDivide(arr, help, left, mid) + mergeDivide(arr, help, mid, right);
   
       int p2 = mid, crossAns = 0;
       for (int p1 = left; p1 < mid; p1++) {
           // [mid, p2)是满足当前arr[p1]>2*arr[p2]的区间
           // 两边都有序，所以p1递增arr[p1]增加时，p2是不回退的
           // 实测发现用int乘2会溢出，所以用LL
           while (p2 < right && arr[p1] > 2LL * arr[p2]) p2++;
           crossAns += p2 - mid;
       }
   
       // 正常merge即可
       int i = left, j = mid, k = left;
       while (i < mid && j < right) {
           if (arr[i] <= arr[j]) help[k++] = arr[i++];
           else help[k++] = arr[j++];
       }
       while (i < mid) help[k++] = arr[i++];
       while (j < right) help[k++] = arr[j++];
       for (int p = left; p < right; p++) { arr[p] = help[p]; }
   
       return ans + crossAns;
   }
   // public方法int reversePairs(vector<int>& nums)直接call mergeDivide即可，略
   ```

---

##### LC215

- 题目：[LeetCode - 215. 数组中第K个最大元素](https://leetcode.cn/problems/kth-largest-element-in-an-array)；

  注：需要找的是数组排序后的第`k`大的元素，而不是第 `k` 个不同的元素；

  - 比如`[1,2,2,3,4,5,5,5,6]`中第3大的元素是`5`；

  快排具体性质见上方`Theory`部分； 

  ```cpp
  int randomSelect(vector<int>& arr, int left, int right, int k) {
      if (right - left <= 1) return arr[left];
      
      int pivot = arr[left + (rand() % (right - left))];
      int s = left, b = right, i = left;
      while (i < b) {
          if (arr[i] < pivot) {
              swap(arr[s++], arr[i++]);
          } else if (arr[i] > pivot) {
              swap(arr[--b], arr[i]);
          } else {
              i++;
          }
      }
  
      if (k < s) { // 随机选大了
          return randomSelect(arr, left, s, k);
      } else if (k >= b) { // 随机选小了
          return randomSelect(arr, b, right, k);
      } else { // 刚好
          return pivot;
      }
  }
  
  int findKthLargest(vector<int>& nums, int k) {
      // nums.size() - k把“第 k 大”换成升序排列后的零起始下标
      return randomSelect(nums, 0, nums.size(), nums.size() - k);
  }
  ```

  - 随机选择的期望时间复杂度为 $O(n)$；

---

##### LC23 / NC - 合并链表

- 题目：[LeetCode - 23. 合并K个升序链表](https://leetcode.cn/problems/merge-k-sorted-lists)；[牛客 - 合并K个已排序的链表](https://www.nowcoder.com/practice/65cfde9e5b9b4cf2b6bafa5f3ef33fa6)；

  每次要从`K`个链表头中选出最小的，这不是堆是什么？

  ```cpp
  class minHeap {
  private:
      vector<ListNode*> heap;
      int hsize;
  
      void shiftUp (int i) {
          ListNode* temp = heap[i];
          while (i > 0) {
              int parent = (i - 1) / 2;
              if (heap[parent]->val > temp->val) {
                  heap[i] = heap[parent];
              } else {
                  break;
              }
              i = parent;
          }
          heap[i] = temp;
      }
  
      void shiftDown(int i) {
          ListNode* temp = heap[i];
          while (i * 2 + 1 < hsize) {
              int left = i * 2 + 1;
              int right = left + 1, smaller = left;
              if (right < hsize && heap[right]->val < heap[left]->val) {
                  smaller = right;
              }
  
              if (temp->val > heap[smaller]->val) {
                  heap[i] = heap[smaller];
              } else {
                  break;
              }
              
              i = smaller;
          }
          heap[i] = temp;
      }
  
  public:
      minHeap(int K) : heap(K), hsize(0) {} // 堆中最多K个元素
  
      bool empty() { return hsize == 0; }
  
      void push(ListNode* node) {
          heap[hsize] = node;
          shiftUp(hsize);
          hsize++;
      }
  
      
      ListNode* pop() {
          ListNode* temp = heap[0];
          --hsize;
          if (hsize > 0) {
              heap[0] = heap[hsize];
              shiftDown(0);
          }
          return temp;
      }
  };
  
  ListNode* mergeKLists(vector<ListNode*>& lists) {
      // 特别强调：所有指针入堆都必须判空，否则破坏语义
      int K = lists.size();
      minHeap h(K);
      for (ListNode* list : lists) {
          if (list) h.push(list); // 判空
      }
      if (h.empty()) return nullptr; // 堆空直接返回
  
      ListNode* ans = h.pop(); // 先取一个节点做头，不用哑节点
      if (ans->next) h.push(ans->next); // 判空
  
      ListNode* cur = ans;
      while (!h.empty()) {
          ListNode* node = h.pop();
  
          cur->next = node;
          cur = cur->next;
  
          if (node->next) h.push(node->next); // 判空
      }
  
      return ans;
  }
  ```


---

##### NC - 线段重合

- 题目：[牛客 - 线段重合](https://www.nowcoder.com/practice/1ae8d0b6bb4e4bcdbf64ec491f63fc37)；注意不考虑端点处的重合；

  维护一个当前所有 有重合线段的集合；
  
  - 先把线段按`start`升序排序；
  
  - 逐个遍历排序后的线段，设为`cur`，从集合中逐出`end`$\le$`cur.start`的旧线段 (它们与`cur`不重合)；
  
    所以：按`strat`升序排序的原因就是保证被逐出的线段不会再次进入集合；
  
  - 遍历过程中，集合中线段数量最大值就是最多重合线段数；
  
  显然这个集合要用堆实现；
  
  ```cpp
  // 手写堆见LC23，这里就用标准库了
  struct Line {
      int start, end;
  };
  
  bool compareLines(const Line& a, const Line& b) {
      return a.start < b.start;
  }
  
  int maxOverlapLines(vector<Line>& lines) {
      sort(lines.begin(), lines.end(), compareLines);
      priority_queue<int, vector<int>, greater<>> minHeap;
      int maxOverlap = 0;
  
      for (const auto& line : lines) {
          while (!minHeap.empty() && minHeap.top() <= line.start) { // 清理不可能与当前线段发生重合的线段
              minHeap.pop();
          }
  
          minHeap.push(line.end);
  
          maxOverlap = max(maxOverlap, (int)minHeap.size());
      }
  
      return maxOverlap;
  }
  // main读入数据后call maxOeverlapLines即可，略
  ```

---

##### LC2208

- 题目：[LeetCode - 2208. 将数组和减半最少操作次数](https://leetcode.cn/problems/minimum-operations-to-halve-array-sum/)；

  每次都选最大的减半，显然使用大根堆；其实也是一种贪心；

  ```cpp
  int halveArray(vector<int>& nums) {
  	// 手写堆见LC23，这里使用标准库直接原地建堆了
      // 其实STL的建堆非常高效
      priority_queue<double> heap(nums.begin(), nums.end());
      double sum = 0;
      for (int num : nums) { sum += num; }
             
      double goal = sum / 2.0;
      int method = 0;
  
      while (sum > goal) {
          double current_max = heap.top();
          heap.pop();
  
          double half = current_max / 2.0;
          sum -= half;
          heap.push(half); // 别忘了减半之后还要压入堆中
  
          method++;
      }
  
      return method;
  }
  ```








## Summary

| SortName | 时间 | 额外空间 | 稳定性 |
| --- | --- | --- | --- |
| SelectionSort | $O(N^2)$ | $O(1)$ | 不稳定 |
| BubbleSort | $O(N^2)$ | $O(1)$ | 稳定 |
| InsertionSort | $O(N^2)$ | $O(1)$ | 稳定 |
| MergeSort | $O(N\log N)$ | $O(N)$ | 稳定 |
| QuickSort | 期望 $O(N\log N)$，最坏 $O(N^2)$ | 期望 $O(\log N)$，最坏 $O(N)$ | 不稳定 |
| HeapSort | $O(N\log N)$ | $O(1)$ | 不稳定 |
| CountSort | $O(N+M)$ | 稳定实现需 $O(N+M)$ | 可稳定 |
| RadixSort | $O(d(N+B))$ | $O(N+B)$ | 稳定 |

其中，$M$ 是计数排序的值域大小，$d$ 是基数排序的位数，$B$ 是基数；随机快排的平均时间按期望估计；
