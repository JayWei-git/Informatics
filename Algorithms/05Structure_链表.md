# Linked List

> Author：玖沐 (Jura)  |  Last Updated：2026/11/07

- [左程云 - 算法034 链表高频题目](https://www.bilibili.com/video/BV1A14y1i7GY)；

## Theory

- 链表题没有特定算法，难点集中于：特定做法，边界条件与细节，code和注释须细细阅读；
- 尽量不要使用哑节点 / 虚拟头节点；





## Practice

##### LC160

- 题目：[LeetCode - 160. 相交链表](https://leetcode.cn/problems/intersection-of-two-linked-lists)；

  - 快慢指针法：

  ```cpp
  ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
      int len1 = 0, len2 = 0;
      for (ListNode *p = headA; p; p = p->next) len1++;
      for (ListNode *p = headB; p; p = p->next) len2++;
      
      ListNode *p1 = headA, *p2 = headB;
      // 让 p1 始终指向较长链表，p2 指向较短链表
      int diff = abs(len1 - len2);
      if (len1 < len2) { swap(p1, p2); }
      
      while (diff--) { p1 = p1->next; } // 快指针先走差值步
      
      // 同步向后查找相交点
      // 即使不想交，p1 p2也会同时变成nullptr
      while (p1 != p2) {
          p1 = p1->next;
          p2 = p2->next;
      }
      
      return p1;
  }
  ```
  

---

##### LC25

- 题目：[LeetCode - 25. K个一组翻转链表](https://leetcode.cn/problems/reverse-nodes-in-k-group)；

  注意如果节点总数不是 `k` 的整数倍，那么最后剩余节点保持原有顺序；题目保证 $n\ge k$；

  需要 $4$ 个变量：`pre_left`，`cur_left`，`cur_right`和`next_left`；
  
  1. 翻转当前组后要让`cur_left->next = next_left`，这样如果后续节点数不足`k`个直接break就行；
  2. 只要不是第 $0$ 组，就要让`pre_left->next = cur_right`；
  
  `cur_right`会从`cur_left`开始往后移`k - 1`个节点进行更新，`next_left`会在`cur_right`更新后设置为其`next`，所以每轮末尾更新`pre_left`和`cur_left`即可；
  
  ```cpp
  ListNode *reverseKGroup(ListNode *head, int k) {
  	ListNode *ans;
  
  	ListNode *pre_left = nullptr; // 第0组没有pre
  	ListNode *cur_left = head;
  	ListNode *next_left;
  
  	while (true) {
          // 1. cur_right从cur_left开始往后移动k-1个节点 (因为是闭区间)
  		ListNode *cur_right = cur_left;
  		for (int i = 1; i < k && cur_right; ++i) {
  			cur_right = cur_right->next;
  		}
  		if (!cur_right) break; // 处理上一组时已经让cur_left->next = next_left，所以下一组不足k个直接不处理即可
  
  		next_left = cur_right->next; // 更新next_left
  
          // 2.翻转当前组，并让cur_left->next = next_left
  		ListNode *cur = cur_left, *pre = next_left; // 注意pre的设置
  		while (cur != next_left) {
  			ListNode *temp = cur->next;
  			cur->next = pre;
  			pre = cur;
  			cur = temp;
  		}
  
  		if (pre_left) { 
  			pre_left->next = cur_right; // 只要不是第0组，更新pre_left->next，重新让其指向cur_right
  		} else {
  			ans = cur_right; // 第0组还要多一个步骤，记录一下ans
  		}
  
          // 3. 滚动到下一组
  		pre_left = cur_left;
  		cur_left = next_left;
  	}
  
  	return ans;
  }
  ```
  


---

##### LC138

- 题目：[LeetCode - 138. 随机链表的复制](https://leetcode.cn/problems/copy-list-with-random-pointer/)；

  难点在于如何快速赋值新链表的`random`字段，比如发现`3->random == 5`，所以要让`3' ->random`指向`5'`，怎么找到`5’`？难道从头开始遍历吗？不行；
  
  第一次循环，在旧节点之间插入复制节点，比如把`3->5->7->null`变成`3->3'->5->5'->7->7'->null`，复制节点的`random`先置空；
  
  第二次循环，更新复制节点的`random`，显然`3’->random = 3->random->next`；
  
  第三次循环，把两个链表拆开即可；
  
  ```cpp
  Node* copyRandomList(Node* head) {
      if (head == nullptr) return nullptr;
      
      for (Node* old = head; old != nullptr; old = old->next->next) {
          Node* copied = new Node(old->val);
          Node* temp = old->next;
          old->next = copied;
          copied->next = temp;
      }
  
  	for (Node* old = head; old != nullptr; old = old->next->next) {
          Node* copied = old->next;
          copied->random = (old->random == nullptr ? nullptr : old->random->next);
      }
  
  	Node* ans = head->next;
      for (Node* old = head; old != nullptr; old = old->next) {
          Node* copied = old->next;
          old->next = copied->next;
          copied->next = (old->next == nullptr ? nullptr : old->next->next);
      }
  
      return ans;
  }
  ```
  
  > 注意几个细节：
  >
  > 第一次第二次循环都已经插入了`copied`节点，所以`old = old->nex-t>next`更新两次；
  >
  > 而第三次循环已经拆开了，所以`old = old->next`更新一次；
  >
  > 另外，解引用任何指针 (除非确信不为空) 都必须判空！！！

---

##### LC234

- 题目：[LeetCode - 234. 回文链表](https://leetcode.cn/problems/palindrome-linked-list/)；

  用栈非常简单，先把整个链表压栈，然后逐个弹出比对即可；如何在 $O(n)$ 时间复杂度和 $O(1)$ 空间复杂度内解决该问题？
  
  首先快慢指针找中点 (偶数就是中点左边)，慢指针走一步快指针走两步；
  
  然后翻转右半部分 并 断开两边，比如 $1\to2\to3\to3\to2\to1$ 改成 $1\to2\to3\to null；null\gets3\gets2\gets1$；然后判断是不是回文；

  ```cpp
  bool isPalindrome(ListNode* head) {
  	// 1. 快慢指针找中点
      if (!head || !head->next) return true; // fast->next&&fast->next->next有短路保护，这个if是防止head为空时访问fast->next，并顺便处理单节点链表
      ListNode *slow = head, *fast = head;
      while (fast->next && fast->next->next) {
          fast = fast->next->next;
          slow = slow->next;
      }
      
      // 2. 翻转右半部分
      ListNode *pre = nullptr, *cur = slow->next;
      slow->next = nullptr; // 断开两部分方便处理
      while (cur) {
          ListNode* temp = cur->next;
          cur->next = pre;
          pre = cur;
          cur = temp;
      } // 结束后pre指向最后一个节点
      
      // 3. 验证回文
      bool ans = true;
      ListNode *left = head, *right = pre;
      while (left && right) {
          if (left->val != right->val) {
              ans = false;
              break;
          }
          
          left = left->next;
          right = right->next;
      }
      
      // 4. 工程好习惯，还原链表
      cur = pre;
      pre = nullptr;
      while (cur) {
          ListNode *temp = cur->next;
          cur->next = pre;
          pre = cur;
          cur = temp;
      } // 结束后pre指向右半部分第一个节点
      
      slow->next = pre; // 这一步别忘了，不然链表是断开的
      return ans;
  }
  ```
  

---

##### LC143

- 题目：[LeetCode - 143. 重排链表](https://leetcode.cn/problems/reorder-list/)；

  和 **LC234** 处理方法一致；

  ```cpp
  void reorderList(ListNode* head) {
      // 1. 找中点
      if (!head || !head->next) return;
  	ListNode *fast = head, *slow = head;
      while (fast->next && fast->next->next) {
  		slow = slow->next;
          fast = fast->next->next;
      }
      
      // 2. 翻转右半部分
      ListNode *pre = nullptr, *cur = slow->next;
      slow->next = nullptr; // 断开两边，否则交替连接后可能成环
      while (cur) {
          ListNode *temp = cur->next;
          cur->next = pre;
          pre = cur;
          cur = temp;
      } // 结束后pre指向最后一个节点
      
      // 3. 交替连接
      ListNode *left = head, *right = pre;
      while (left && right) {
          ListNode *left_next = left->next, *right_next = right->next;
          left->next = right;
          right->next = left_next; // 如果之前没有断开两边，最中间节点就会在这成环(可画图体会)
          left = left_next;
          right = right_next;
      }
  }
  ```

---

##### LC142

- 题目：[LeetCode - 142. 环形链表 II](https://leetcode.cn/problems/linked-list-cycle-ii/)；

  首先提醒一句，由于链表节点只有一个`next`指针，所以只能在尾部成环，且成环就不会分叉；

  用哈希表的做法就略了，太简单；快慢指针的做法不好想，直接记忆；

  ```cpp
  ListNode* detectCycle(ListNode* head) {
      ListNode *slow = head, *fast = head;
  
      while (fast && fast->next) {
          slow = slow->next;
          fast = fast->next->next;
  
          if (slow == fast) {
              ListNode* p = head;
              while (p != slow) {
                  p = p->next;
                  slow = slow->next;
              }
              return p; // 环入口
          }
      }
  
      return nullptr; // 无环
  }
  ```
  
  > 注：这道题的前一道题也就是 **LC141 环形链表**，不要求返回环入口只判断有无环，那就快慢指针不停走就行了；

---

##### LC148

- 题目：[LeetCode - 148. 排序链表](https://leetcode.cn/problems/sort-list/)；

  要求时间复杂度 $O(n\times\log n)$，空间复杂度 $O(1)$，且排序稳定；

  显然只能不用递归进行归并了；

  ```cpp
  ListNode* sortList(ListNode* head) {
      int n = 0;
      for (ListNode* p = head; p; p = p->next) { n++; }
      
      for (int size = 1; size < n; size <<= 1) { // size是每次归并的长度
      	ListNode *cur = head, *newhead = nullptr, *tail = nullptr;
          
          // split的语义是够size就切，不够size就返回空，所以即使剩余节点不够也能工作良好
          while (cur) {
              ListNode* left = cur;
              ListNode* right = split(left, size);
              
              cur = split(right, size); // 还得切一次，让left和right变成两个单独小链表才能merge
              
              ListNode* merged = merge(left, right); // 归并切出来的left和right
              
              // 经典去哑做法
              if (!newhead) { // 头空说明在开头，初始化 
              	newhead = merged;
                  tail = merged;
              } else { // 头不空说明在中间，接到tail后面 
              	tail->next = merged;
              }
              
              while (tail->next) { tail = tail->next; } // 别忘了tail要去找新的尾
          }
          
          head = newhead; // 重置head
      }
      
      return head;
  }
  
  // 合并两个有序链表
  ListNode* merge(ListNode* p1, ListNode* p2) {
      ListNode *head = nullptr, *tail = nullptr;
      
      while (p1 && p2) {
          ListNode* node;
         	// node选出较小节点
          if (p1->val <= p2->val) {
              node = p1;
              p1 = p1->next;
          } else {
              node = p2;
              p2 = p2->next;
          }
          
          // 依然是去哑做法
          // [head, tail]，head为空接在head后面，head不为空接在tail后面
          if (!head) { head = node; }
          else { tail->next = node; }
          
          tail = node; // tail初始化 / tail后移
          // 这里和上面不太一致是因为每次只接一个node到tail后面
      }
      
      if (!head) return p1 ? p1 : p2; // 至少一个链表是空的
      
      tail->next = p1 ? p1 : p2; // 剩余部分别忘了
      return head;
  }
  
  // 切出size个节点，返回后半段，不足返回空
  ListNode* split(ListNode* head, int size) {
      if (!head) return nullptr;
      
      for (int i = 1; i < size && head->next; i++) {
          head = head->next;
      }
      
      ListNode* next = head->next;
      head->next = nullptr;
      return next;
  }
  ```
  
  > 真难啊.... (吐血)

---

##### LC19

- 题目：[LeetCode - 19. 删除链表的倒数第 N 个结点](https://leetcode.cn/problems/remove-nth-node-from-end-of-list/)；

  仍然是快慢指针，快指针先走 $n$ 步然后二者一起走，当快指针走到尾节点时慢指针指向待删除节点的前驱；

  实测发现如果待删除节点是头节点会出错，所以单独处理；

  ```cpp
  ListNode* removeNthFromEnd(ListNode* head, int n) {
  	ListNode *fast = head;
      for (int i = 0; i < n; i++) {
          fast = fast->next; // 题目保证链表至少有n个节点
      }
      
      if (!fast) { // fast指空了，说明要删除头节点
          ListNode* next = head->next;
          delete head;
          return next; 
      }
      
      ListNode* slow = head;
      while (fast->next) { // 让快指针走到尾节点
          fast = fast->next;
          slow = slow->next;
      }
      
      ListNode* target = slow->next;
      slow->next = target->next;
      delete target;
      return head;
  }
  ```

---

##### LC24

- 题目：[LeetCode - 24. 两两交换链表中的节点](https://leetcode.cn/problems/swap-nodes-in-pairs/)；

  ```cpp
  ListNode* swapPairs(ListNode* head) {
  	if (!head || !head->next) return head; // 空或单
      
      ListNode* newhead = head->next;
      ListNode* prev = nullptr; // 上一组交换后的尾
      ListNode* cur = head; // 待交换的头
      
      while (cur && cur->next) {
          ListNode* first = cur;
          ListNode* second = cur->next;
          ListNode* next = second->next;
          
          first->next = next;
          second->next = first;
          
          if (prev) prev->next = second; // 处理的是中间的，正常连接
          
          prev = first; // prev初始化 / prev后移
          cur = next;
      }
      
      return newhead;
  }
  ```

  > 最开始我是这样处理的`prev`：
  >
  > ```cpp
  > if (!prev) prev = first; // 处理的是开头两个，初始化prev
  > else prev->next = second; // 处理的是中间的，正常连接
  > 
  > prev = first; // 后移
  > cur = next;
  > ```
  >
  > 然后我发现逻辑其实可以合并；

---

##### LC61

- 题目：[LeetCode - 61. 旋转链表](https://leetcode.cn/problems/rotate-list/)；

  $k$ 可以大于链表长度，不过对链表长度取个模就行了；然后快慢指针就行了；
  
  ```cpp
  ListNode* rotateRight(ListNode* head, int k) {
      if (!head || !head->next) return head; // 下文n要作为除数，链表不能为空，顺便处理单节点链表
  
      int n = 0;
      for (ListNode* p = head; p; p = p->next) { n++; }
  
      int new_k = k % n; // new_k 范围是[0, n-1]
      if (new_k == 0) return head; // 如果k是n的倍数也就是new_k==0，下文也是工作良好的，但没必要
      
      ListNode *slow = head, *fast = head;
      for (int i = 0; i < new_k; i++) { fast = fast->next; }
      while (fast && fast->next) { // 让fast停在尾节点
          fast = fast->next;
          slow = slow->next;
      }
  
      fast->next = head;
      head = slow->next;
      slow->next = nullptr;
  
      return head;
  }
  ```
  
  - 需要遍历两边链表，复杂度是 $O(n)$ 的；
  
  还有一种写法是不统计链表长度，直接让链表成环，让 `fast` 沿环走 `k` 步，再让 `fast` 和 `slow` 同时走，直到 `fast` 到达原尾节点，此时 `slow` 就是新尾节点；复杂度是 $O(n+k)$ 的，`k`较小时这种写法更好，`k`较大时上面更好

---

##### LC83

- 题目：[LeetCode - 83. 删除排序链表中的重复元素](https://leetcode.cn/problems/remove-duplicates-from-sorted-list/)；

  ```cpp
  ListNode* deleteDuplicates(ListNode* head) {
      ListNode* cur = head;
      while (cur) { // 这里已判断head不为空，下一行放心解引用
          ListNode* next = cur->next;
          // next要么指空要么指向与cur值不同节点
          while (next && next->val == cur->val) {
              next = next->next;
          }
          
          // 把cur和next之间的全部删了
          // 即使cur->next==next也正确
          ListNode* removed = cur->next;
          while (removed != next) {
              ListNode* temp = removed->next;
              delete removed;
              removed = temp;
          }
          
          cur->next = next; // 别忘了连过去
          cur = next; // 滚动
      }
      return head;
  }
  ```

---

##### LC82

- 题目：[LeetCode - 82. 删除排序链表中的重复元素 II](https://leetcode.cn/problems/remove-duplicates-from-sorted-list-ii/)；

  相较于 **LC83** 就是重复节点全删而不是留一个，比如 $1\to2\to2\to3$ 变为 $1\to3$；引入`prev`即可；

  ```cpp
  ListNode* deleteDuplicates(ListNode* head) {
      ListNode* prev = nullptr;
      ListNode* cur = head;
  
      while (cur) {
          ListNode* next = cur->next;
          // next要么指空要么指到与cur值不同节点
          while (next && next->val == cur->val) {
              next = next->next;
          }
  
          if (cur->next == next) {  // 当前值只出现一次
              prev = cur;
              cur = next;
          } else {                  // 删除当前值的整段节点
              if (prev) prev->next = next;
              else head = next;
  
              while (cur != next) {
                  ListNode* removed = cur;
                  cur = cur->next;
                  delete removed;
              }
          }
      }
  
      return head;
  }
  ```
  

---

##### LC86

- 题目：[LeetCode - 86. 分隔链表](https://leetcode.cn/problems/partition-list/)；

  ```cpp
  ListNode* partition(ListNode* head, int x) {
  	ListNode *lessHead = nullptr, *lessTail = nullptr;
      ListNode *otherHead = nullptr, *otherTail = nullptr;
      
      while (head) {
          ListNode* node = head;
          head = haed->next;
          node->next = nullptr; // 其实不用显式断开node，不过这样更直观
          
          if (node->val < x) {
              if (lessHead) lessTail->next = node;
              else lessHead = node; // 没有就初始化一下
              lessTail = node;
          } else {
              if (otherHead) otherTail->next = node;
              else otherHead = node;
              otherTail = node;
          }
      }
      
      if (lessHead) lessTail->next = otherHead; // 不要盲目拼接，记得判空
      else return otherHead;
      
      return lessHead;
  }
  ```

---

##### LC92

- 题目：[LeetCode - 92. 反转链表 II](https://leetcode.cn/problems/reverse-linked-list-ii/)；

  题目保证$1\le$`left`$\le$`right`$\le n$；且要求只用一趟扫描；

  没啥难的，多用临时变量保存要用到的节点，理清循环次数与指针最终停留节点 间的关系 就行了；
  
  ```cpp 
  ListNode* reverseBetween(ListNode* head, int left, int right) {
      ListNode* before = nullptr;
      ListNode* cur = head;
      for (int i = 1; i < left; i++) {
          before = cur;
  		cur = cur->next;
      }
      
      // 此时cur找到了left
      // 翻转后，left会成为这一小段的tail
      ListNode* tail = cur;
      
      // 翻转[left, right]内的节点
      ListNode* prev = nullptr;
      for (int i = left; i <= right; i++) {
  		ListNode* temp = cur->next;
          cur->next = prev;
          prev = cur;
          cur = temp;
      }
      
      // 此时prev找到了right
      // cur是right->next
      tail->next = cur;
      if (before) before->next = prev; // 注意若left是head，那么before为空
      else head = prev;
      
      return head;
  }
  ```
  

---

##### LC147

- 题目：[LeetCode - 147. 对链表进行插入排序](https://leetcode.cn/problems/insertion-sort-list/)；

  ```cpp
  ListNode* insertionSortList(ListNode* head) {
  	
  }
  ```

  
