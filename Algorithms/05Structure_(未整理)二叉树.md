# Binary Tree

> Author：玖沐 (Jura)  |  Last Updated：2026/10/30

- [左程云 - 算法036 二叉树题目上](https://www.bilibili.com/video/BV1Rp4y1g7ys)；
- [左程云 - 算法037 二叉树题目下](https://www.bilibili.com/video/BV1194y16727)；

## Theory

- 二叉树具有自相似性：每棵子树仍是二叉树，适合定义递归函数处理左右子树，再合并结果；
- 二叉树也是树，可以按节点间的连接关系进行搜索，如用队列做 BFS（层序遍历），或用栈、递归做 DFS；

递归与搜索并不互斥；递归遍历二叉树就是 DFS 的一种实现；







## Practice

##### 二叉树的四种遍历

- 题目：[LeetCode - 144. 二叉树的前序遍历](https://leetcode.cn/problems/binary-tree-preorder-traversal/)；

- 题目：[LeetCode - 94.   二叉树的中序遍历](https://leetcode.cn/problems/binary-tree-inorder-traversal/)；

- 题目：[LeetCode - 145. 二叉树的后序遍历](https://leetcode.cn/problems/binary-tree-postorder-traversal/)；

- 题目：[LeetCode - 102. 二叉树的层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/)；

  1. 递归实现三种遍历：
  
  ```cpp
  void preorder(TreeNode* root) {
      if (root == nullptr) return;
      // 访问 / 操作 当前root指向的节点
      preorder(root->left, res);
      preorder(root->right, res);
  }
  
  void inorder(TreeNode* root) {
      if (root == nullptr) return;
      inorder(root->left);
      // 访问 / 操作 当前root指向的节点
      inorder(root->right);
  }
  
  void postorder(TreeNode* root) {
      if (root == nullptr) return;
      postorder(root->left);
      postorder(root->right);
      // 访问 / 操作 当前root指向的节点
  }
  ```
  
  2. 模拟系统栈改写三种递归：
  
     栈帧需要保存局部变量`root`和当前`pc`作为返回地址；规定：
  
     pc = 0：刚进入函数，接下来调用左子树
     pc = 1：左子树调用返回，接下来调用右子树
     pc = 2：右子树调用返回，当前函数返回
  
     三种遍历只是访问节点的位置不同；
  
  ```cpp
  struct StackFrame {
      TreeNode* root;
      int pc;
  };
  
  void preorder(TreeNode* root) {
      stack<StackFrame> s;
      s.push({root, 0});
  
      while (!s.empty()) {
          StackFrame& f = s.top(); // 引用，要修改其pc
  
          if (f.root == nullptr) { // 递归基
              s.pop(); // 当前调用返回
              continue;
          }
  
          switch (f.pc) {
              case 0:
                  // 访问当前节点，例如 cout << f.root->val << ' ';
  				// 前序遍历要在访问左子树前访问root，另外两种同理
                  f.pc = 1;                   // 保存返回位置
                  s.push({f.root->left, 0});  // 调用左子树
                  break;
  
              case 1:
                  f.pc = 2;                    // 保存返回位置
                  s.push({f.root->right, 0});  // 调用右子树
                  break;
  
              case 2:
                  s.pop(); // 当前调用返回
                  break;
          }
      }
  }
  ```
  
  3. 队列实现层序遍历：
  
  ```cpp
  // 层序遍历，使用队列逐层遍历即可
  // 由于这道题要求返回一个嵌套vector，所以需要vector<int> cur_level
  vector<vector<int>> levelOrder(TreeNode* root) {
      if (!root) return {};
  
      vector<vector<int>> ans;
      queue<TreeNode*> q;
      q.push(root);
  
      while (!q.empty()) {
          int level_size = q.size();
          vector<int> cur_level(level_size);
  
          for (int i = 0; i < level_size; i++) {
              TreeNode* cur = q.front();
              q.pop();
  
              cur_level[i] = cur->val;
  
              if (cur->left) q.push(cur->left);
              if (cur->right) q.push(cur->right);
          }
  
          ans.push_back(cur_level);
      }
  
      return ans;
  }
  ```

---

##### LC103

- 题目：[LeetCode - 103. 二叉树的锯齿形层序遍历](https://leetcode.cn/problems/binary-tree-zigzag-level-order-traversal/)；

  正常层序遍历，但是往每一层的vector里面塞的时候看看是正塞还是反塞就行了

  ```cpp
  vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
      if (!root) return {};
  
      vector<vector<int>> ans;
      queue<TreeNode*> q;
      q.push(root);
  
      bool flag = true; // true表示正塞
      
      while (!q.empty()) {
          int level_size = q.size();
          vector<int> cur_level(level_size);
  
          for (int i = 0; i < level_size; i++) {
              TreeNode* cur = q.front();
              q.pop();
  
              int index = flag ? i : level_size - i - 1;
              cur_level[index] = cur->val;
  
              if (cur->left) q.push(cur->left);
              if (cur->right) q.push(cur->right);
          }
  
          ans.push_back(cur_level);
          flag = !flag; // 换向
      }
  
      return ans;
  }
  ```

---

##### LC662

- 题目：[LeetCode - 662. 二叉树最大宽度](https://leetcode.cn/problems/maximum-width-of-binary-tree)；

  ```cpp
  int widthOfBinaryTree(TreeNode* root) {
      return -1;
  }
  ```
  

---

##### LC104

- 题目：[LeetCode - 104. 二叉树最大深度](https://leetcode.cn/problems/maximum-depth-of-binary-tree/description/)；

  递归即可；

  ```cpp
  int maxDepth(TreeNode* root) {
  	if (root == nullptr) return 0;
      return max(maxDepth(root->left) + 1, maxDepth(root->right) + 1);
  }
  ```

---

##### LC111

- 题目：[LeetCode - 111. 二叉树最小深度](https://leetcode.cn/problems/minimum-depth-of-binary-tree/description/)；

  注意最小深度不是较浅的孩子的高度+1，所以不能写为：`return min(minDepth(root->left) + 1, minDepth(root->right) + 1);`；比如一颗一直向右的链，无论多长，上面的逻辑都会返回`1`，显然错的；

  讨论一下子树为不为空就行了；

  ``` cpp
  int minDepth(TreeNode* root) {
      if (root == nullptr) return 0;
      
      // 情况 1: 左子树为空，右子树不为空
      // 最小深度取决于右子树
      if (root->left == nullptr && root->right != nullptr) {
          return minDepth(root->right) + 1;
      }
      
      // 情况 2: 右子树为空，左子树不为空
      // 最小深度取决于左子树
      if (root->right == nullptr && root->left != nullptr) {
          return minDepth(root->left) + 1;
      }
      
      // 情况 3: 左右子树都不为空，或者都为空（叶节点）
      // 此时整棵树的最小深度才是：左右子树较浅的那一棵的深度 + 1
      return min(minDepth(root->left) + 1, minDepth(root->right) + 1);
  }
  ```

---

##### LC958

- 题目：[LeetCode - 958. 二叉树的完全性检验](https://leetcode.cn/problems/check-completeness-of-a-binary-tree/description/)；

  ```
            1
         /     \
        2       3			
      /   \   /   \
     4     5 6    空
    / \   / \
   空  空 空  空 
  ```

  队列BFS逐层遍历，遇见空节点就不能遇见有效节点了；

  ```cpp
  // 题目保证了树不空
  bool isCompleteTree(TreeNode* root) {
  	queue<TreeNode*> q;
      q.push(root);
      
      bool flag = false; // 标记一下遇见过空节点没有
      while (!q.empty()) {
          TreeNode* cur = q.front();
          q.pop();
          
          if (!cur) flag = true; // 遇见空节点了
          else {
          	if (flag) return false; // 之前遇见过空节点，当前节点还不空
              q.push(cur->left);
              q.push(cur->right);
          }
      }
      
      return true; // 全部跑完没有违规才能下结论
  }
  ```

---

##### LC222

- 题目：[LeetCode - 222. 完全二叉树的节点个数](https://leetcode.cn/problems/count-complete-tree-nodes/description/)；不能使用遍历树的 $O(n)$ 做法；

  观察发现，完全二叉树的两个子树，总有一个是满二叉树，另一个是完全二叉树；对于满二叉子树，直接使用公式计算节点个数即可，对于完全二叉子树，递归该过程即可；

  如何判断两棵子树谁是完全谁是满？首先计算左右子树的极左高度，分别设为 `left_lh` 和 `right_lh`，由于是完全二叉树，所以 `left_lh` $\ge$ `right_lh`；

  - 如果 `left_lh == right_lh`，说明最后一层的节点已经延伸到了右子树，则此时左子树是满的，右子树是完全的；
  - 如果 `left_lh > right_lh`，说明最后一层的节点还没有填满左子树，或者刚刚填到左子树；则此时右子树是满的，左子树是完全的；

  ```cpp
  int getLeftHeight(TreeNode* node) { // 计算极左高度
      int height = 0;
      while (node) {
  		height++;
          node = node->left;
      }
     	return height;
  }
  
  int countNodes(TreeNode* root) {
      if (root == nullptr) return 0;
      
  	int left_h = getLeftHeight(root->left);
      int right_h = getLeftHeight(root->right);
      
      if (left_h == right_h) {
  		// 情况 A：左子树是满二叉树
          // 左子树节点数 = (1 << left_h) - 1，加上根节点 1，等于 (1 << left_h)
          // 然后递归计算右子树
          return (1 << left_h) + countNodes(root->right);
      } else {
  		// 情况 B：右子树是满二叉树
          // 右子树节点数 = (1 << right_h) - 1，加上根节点 1，等于 (1 << right_h)
          // 然后递归计算左子树
          return (1 << right_h) + countNodes(root->left);
      }
  }
  ```

  复杂度分析：递归深度为树高也就是 $O(\log n)$，每层递归都要调用 `getLeftHeight`，其复杂度为 $O(\log n)$；所以总时间复杂度为 $O(\log^2n)$；

---

##### LC110

- 题目：[LeetCode - 110. 平衡二叉树](https://leetcode.cn/problems/balanced-binary-tree/description/)；

  如果按照常规递归三步，一棵树是平衡二叉树 $\iff$ 左子树平衡 $\land$ 右子树平衡 $\land$ 左右子树高度差 $\le1$；

  ```cpp
  bool isBalanced(TreeNode* root) {
  	if (root == nullptr) return true;
      return isBalanced(root->left) && isBalanced(root->right) && heightDiff_of_subTrees_lessThanOne(root);
  }
  ```

  但是 `heightDiff_of_subTrees_lessThanOne(root)` 实现难度较大；

  转变思路：求树高的同时判断树是不是平衡的；

  设函数 `int getHeight(TreeNode* root)` 的行为是：如果 `root` 树平衡，返回其高度；如果 `root` 树不平衡，返回一个标记，比如 `-1`；

  根据此行为，可写出具体的递归逻辑：

  ```cpp
  int getHeight(TreeNode* root) {
      if (root == nullptr) return 0; // 边界情况
      
      int leftHeight = getHeight(root->left); 
      if (leftHeight == -1) return -1; // 左子树不平衡，整棵树就不平衡
      
      int rightHeight = getHeight(root->right); 
      if (rightHeight == -1) return -1; // 右子树不平衡，整棵树也不平衡
      
      if (abs(leftHeight - rightHeight) > 1) return -1; // 左右子树都平衡，但是高度差大于1，整棵树还是不平衡
      
      return max(leftHeight, rightHeight) + 1; // 整棵树平衡，返回其高度
  }
  ```

  原接口函数直接调用即可

  ```cpp
  bool isBalanced(TreeNode* root) {
  	return getHeight(root) != -1;
  }
  ```

---

##### LC112

- 题目：[LeetCode - 112. 路径总和](https://leetcode.cn/problems/path-sum/)；

  简单地一比；节点值有负值，不要提前剪枝；

  ```cpp
  class Solution {
  private:
      bool dfs(TreeNode* cur, int cur_sum, int target) {
          if (!cur) return false;
  
          cur_sum += cur->val;
          if (cur_sum == target && !cur->left && !cur->right) {
              return true;
          }
  
          return dfs(cur->right, cur_sum, target) || dfs(cur->left, cur_sum, target);
      }
  public:
      bool hasPathSum(TreeNode* root, int targetSum) {
          return dfs(root, 0, targetSum);
      }
  };
  ```

---

##### LC113

- 题目：[LeetCode - 113. 路径总和II](https://leetcode.cn/problems/path-sum-ii/description/)；

  回溯算法，注意此题节点值可正可负，无法提前剪枝，只能遍历所有路径；

  ```cpp
  class Solution {
  private:
      vector<vector<int>> ans;
      void dfs(TreeNode* cur, int target, int cur_sum, vector<int>& path) {
          if (!cur) return;
          
          cur_sum += cur->val;
          path.push_back(cur->val);
          
          if (cur_sum == target && !cur->left && !cur->right) { // 到叶子了
  			ans.push_back(path);
          }
          
          dfs(cur->left, target, cur_sum, path);
          dfs(cur->right, target, cur_sum, path);
          
          path.pop_back();
      }
  public:
      vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
          vector<int> path;
          dfs(root, targetSum, 0, path);
          return ans;
      }
  };
  ```

---

##### LC669

- 题目：[LeetCode - 669. 修剪搜索二叉树](https://leetcode.cn/problems/trim-a-binary-search-tree/)；

  递归，把握好函数行为即可；

  ```cpp
  // trimBST返回已经修剪好的树
  TreeNode* trimBST(TreeNode* root, int low, int high) {
      if (root == nullptr) return root; // 空结点肯定是已经修剪好的
  	
      if (root->val > high) return trimBST(root->left, low, high);  // 根和右子树全部不要，去修剪左子树
      if (root->val < low)  return trimBST(root->right, low, high); // 同理
      
      // 根节点在范围中，分别修剪左右子树即可
      root->left = trimBST(root->left, low, high);
      root->right = trimBST(root->right, low, high);
      return root;
  }
  ```

---

##### LC109

- 题目：[LeetCode - 109. 有序链表转换二叉搜索树](https://leetcode.cn/problems/convert-sorted-list-to-binary-search-tree/)；

  注意不光搜索还平衡；

  方法一：递归函数语义为：以head整个链表建立平衡搜索树，先快慢指针找中点，就是根，然后递归建立左右子树即可；

  ```cpp
  TreeNode* sortedListToBST(ListNode* head) {
      if (!head) return nullptr;
      if (!head->next) return new TreeNode(head->val);
  
      ListNode *slow = head, *fast = head, *slow_prev = nullptr;
      while (fast && fast->next) {
          slow_prev = slow;
          slow = slow->next; // slow会停在根节点
          fast = fast->next->next;
      }
  
      slow_prev->next = nullptr; // 将用于构建左子树的链表断开，以符合递归语义
  
      TreeNode* root = new TreeNode(slow->val);
      root->left = sortedListToBST(head);
      root->right = sortedListToBST(slow->next);
      return root;
  }
  ```

  每次递归都要找中点，复杂度 $O(n\log n)$；

  方法二：不用每次都双指针找中点，所有递归函数共享同一个计数器，先建左子树，建好后计数器来到根，再建右子树，计数器只顺着走一边链表即可，也不用拆链表，$O(n)$；

  ```cpp
  // 设build语义是将cur及后面的n个节点转为搜索树(不破坏链表)，并且最终cur指向end(右开，建了左子树cur就指向root)
  // 不需要理清所有递归细节，规定好函数语义直接调就行了
  TreeNode* build(ListNode*& cur, int n) {
      if (n == 0) return nullptr;
      
      TreeNode* left = build(cur, n / 2); // 建好左子树
      
      TreeNode* root = new TreeNode(cur->val); // cur来到root
      root->left = left;
      
      cur = cur->next; // 让cur走一步，指向右子树开头，保持语义
      
      root->right = build(cur, n - n / 2 - 1); // 建右子树
      return root;
  }
  
  TreeNode* sortedListToBST(ListNode* head) {
  	int n = 0;
      for (ListNode* p = head; p != nullptr; p = p->next) n++;
      
      ListNode* cur = head;
      return build(cur, n);
  }
  ```

---













## 未整理

##### LC114

- 题目：[LeetCode - 114. 二叉树展开为链表](https://leetcode.cn/problems/flatten-binary-tree-to-linked-list/)；

##### LC117

---

##### LC236 - LCA没懂

- 题目：[LeetCode - 236. 普通二叉树的最近公共祖先](https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/description/)；

  Lowest Common Ancestor，LCA；

  - 首先定义函数行为：设函数 `TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)` 返回以 `root` 为根的树中 `p` 和 `q` 的最近公共祖先；

    行为A. 如果 `p` 和 `q` 都在以 `root` 为根的树中，返回它们的最近公共祖先；

    行为B. 如果 `p` 和 `q` 都不在以 `root` 为根的树中，返回 `nullptr`；

    行为C. 如果 `p` 和 `q` 只有一个在以 `root` 为根的树中，返回那个存在的节点；

  - 然后根据函数行为推导递归逻辑：

    ```cpp
    TreeNode* left_result = lowestCommonAncestor(root->left, p, q);
    TreeNode* right_result = lowestCommonAncestor(root->right, p, q);
    ```

    情况1：如果 `left_result` 和 `right_result` 都不为空，说明 `p` 和 `q` 分别在左右子树中，满足行为 A. 函数返回 `root`；

    情况2：如果 `left_result` 和 `right_result` 都为空，说明 `p` 和 `q` 都不在以 `root` 为根的树中，满足行为 B. 函数返回 `nullptr`；

    情况3：如果 `left_result` 和 `right_result` 只有一个为空，不空的那个返回值，也是通过层层递归返回上来的，要么是已经计算好的最近公共祖先，要么是单独存在的 `p` 或 `q` ；无论是行为 A. 还是行为 C. 都直接返回这个不空的result即可；

  - 最后根据递归逻辑确定边界情况：

    如果 `root == nullptr` ，`p` 和 `q` 肯定不在以 `root` 为根的树中，返回 `nullptr`；

    如果 `root == q || root == p`，那么无论是行为 A. 还是 C. 都直接返回 `root` 即可；

  ```cpp
  TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  	if (root == nullptr || root == q || root == p) return root;
      
      TreeNode* left_result = lowestCommonAncestor(root->left, p, q);
      TreeNode* right_result = lowestCommonAncestor(root->right, p, q);
      
      // 都不为空，返回root
      if (left_result != nullptr && right_result != nullptr) {
          return root;
      }
      
      // 谁不空就返回谁，都空就会返回nullptr
      return left_result != nullptr ? left_result : right_result;
  }
  ```

- 题目2：[LeetCode - 235. 二叉搜索树的最近公共祖先](https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-search-tree/)；

  由于二叉搜索树的排序性质，这道题就比较简单了；

  如果 `p` 和 `q` 的值都小于 `root` 的值，说明它们都在左子树，向左子树移动：`root = root.left`；

  如果 `p` 和 `q` 的值都大于 `root` 的值，说明它们都在右子树，向右子树移动：`root = root.right`；

  如果不满足上述两种情况（即 `root` 的值介于 `p` 和 `q` 之间，或者 `root` 恰好等于其中一个的值），那么当前的 `root` 就是它们的最近公共祖先，直接返回即可；

  不过需要注意不存在公共祖先的情况；

  ```cpp
  TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
      while (root) { // 如果root都跑到最下面的空指针去了都还找不到公共祖先，说明没有
          if (p->val < root->val && q->val < root->val) {
              // p 和 q 都在左子树
              root = root->left;
          } else if (p->val > root->val && q->val > root->val) {
              // p 和 q 都在右子树
              root = root->right;
          } else {
              // 找到分岔点，或者 root 就是 p/q 之一
              return root;
          }
      }
      return nullptr;
  }
  ```

  也可按照题目1用递归去求解，但没必要；

---

##### Problem 13

- 题目：[LeetCode - 98. 验证二叉搜索树](https://leetcode.cn/problems/validate-binary-search-tree/description/)；

- 方法1：是二叉搜索树$\iff$中序遍历的结果严格单增；直接替换中序遍历中对结点的操作即可；

  ```cpp
  bool isValidBST(TreeNode* root) {
      stack<TreeNode*> s;
      TreeNode* prev = nullptr; // 记录中序遍历的前一个节点
  
      while (root != nullptr || !s.empty()) {
          while (root != nullptr) {
              s.push(root);
              root = root->left;
          }
          root = s.top();
          s.pop();
  
          // 如果当前节点值小于等于上一个节点值，说明不是严格递增
          if (prev != nullptr && root->val <= prev->val) {
              return false;
          }
          prev = root; // 更新前驱节点
  
          root = root->right;
      }
      return true;
  }
  ```

- 方法2：递归，任意根节点 > 左子树最大值，< 右子树最小值；

  ```cpp
  // 不考虑结点恰为INT_MAX 或 INT_MIN
  int minOf(TreeNode* root) {
      if (!root) return INT_MAX;
      return min(root->val, min(minOf(root->left), minOf(root->right)));
  }
  
  int maxOf(TreeNode* root) {
      if (!root) return INT_MIN;
      return max(root->val, max(maxOf(root->left), maxOf(root->right)));
  }
  
  bool isValidBST(TreeNode* root) {
      if (!root) return true;
      bool isCurrentValid = (root->val >= maxOf(root->left)) && (root->val <= minOf(root->right));
      return isCurrentValid && isValidBST(root->left) && isValidBST(root->right);
  }
  // 可记忆化，略
  ```

  ```cpp
  // 设isVlidBST返回时将全局变量min和max分别设为以root为根树中的最大最小值
  // 不考虑结点恰为INT_MAX 或 INT_MIN
  int minOfRoot, maxOfRoot;
  bool isValidBST(TreeNode* root) {
      // 边界
      if (!root) {
          minOfRoot = INT_MAX;
          maxOfRoot = INT_MIN;
          return true;
      }
  
      bool isLeftBST = isValidBST(root->left);
      // 赶紧存左子树的最大最小值
      int maxOfLeft = maxOfRoot;
      int minOfLeft = minOfRoot;
  
      bool isRightBST = isValidBST(root->right);
      // 赶紧存右子树的最大最小值
      int maxOfRight = maxOfRoot;
      int minOfRight = minOfRoot;
  
      // 保证递归语义完整
      minOfRoot = min(minOfLeft, min(minOfRight, root->val));
      maxOfRoot = max(maxOfLeft, max(maxOfRight, root->val));
  
      return isLeftBST && isRightBST && (maxOfLeft < root->val && root->val < minOfRight);
  }
  ```

- 方法3：区间界限法

  ```cpp
  class Solution {
      bool helper(TreeNode* node, long long lower, long long upper) {
          if (!node) return true;
          
          if (node->val <= lower || node->val >= upper) {
              return false;
          }
  
          return helper(node->left, lower, node->val) && 
                 helper(node->right, node->val, upper);
      }
  
  public:
      bool isValidBST(TreeNode* root) {
          return helper(root, LLONG_MIN, LLONG_MAX);
      }
  };
  ```



---

##### Problem 15

- 题目：[LeetCode - 337. 打家劫舍 Ⅲ](https://leetcode.cn/problems/house-robber-iii/)；

  ```cpp
  class Solution {
      // yes为偷了root的情况下能偷到的最大值
      // no为不偷root的情况下能偷到的最大值
      int yes, no;
      
      // helper返回时，会将设置yes和no
      void helper(TreeNode* root) {
          if (!root) { // 注意边界
              yes = 0;
              no = 0;
              return;
          }
  
          helper(root->left);
          int lyes = yes;
          int lno = no;
  
          helper(root->right);
          int ryes = yes;
          int rno = no;
  
          yes = root->val + lno + rno; // 偷了本树的root，两边子树就不能偷头结点
          no = max(lyes, lno) + max(ryes, rno); // 不偷本树root，两边子树可偷可不偷头，看哪个大
          // 注意两边子树是独立的，因为两颗子树不可能有结点相邻
      }
      
  public:
      int rob(TreeNode* root) {
          helper(root);
          return max(yes, no);
      }
  };
  ```

  注：ChatGPT给的代码

  ```cpp
  pair<int,int> dfs(TreeNode* root) {
      if (!root) return {0, 0};
  
      auto [lyes, lno] = dfs(root->left);
      auto [ryes, rno] = dfs(root->right);
  
      int yes = root->val + lno + rno;
      int no = max(lyes, lno) + max(ryes, rno);
  
      return {yes, no};
  }
  ```





---

##### (?) Problem 6

- 题目：[LeetCode - 297. 二叉树的序列化与反序列化](https://leetcode.cn/problems/serialize-and-deserialize-binary-tree/description/)；

  - 序列化：将一个二叉树数据结构转换为符串或字节流；
  - 反序列化：从字符串或字节流中恢复出原始的二叉树结构；

- 前序遍历 配合 空节点标记 序列化二叉树：

  ```
  	1
     / \
    2   3			前序序列化结果为：1,2,#,#,3,4,#,#,5,#,#,
       / \
      4   5
  ```

  前序序列化和反序列化都是递归过程，套路处理即可 (定义递归函数行为，写出递归方程，确定边界条件)；

  - 序列化：设 `string serializeDFS(TreeNode* root)` 接受一个节点指针，返回以该节点为根的二叉树前序序列化结果；

    ```cpp
    string serializeDFS(TreeNode* root) {
        if (root == nullptr) return "#,";
        return to_string(root->val) + "," + serializeDFS(root->left) + serializeDFS(root->right);
    }
    ```

  - 反序列化：设 `TreeNode* deserializeDFS(queue<string>& nodes)` 接受一个 `string` 队列，返回以队列第一个元素为根节点的二叉树；

    ```cpp
    TreeNode* deserializeDFS(queue<string>& nodes) {
        if (nodes.empty()) return nullptr;
        
        string current = nodes.front();
        nodes.pop();
        
        if (current == "#") return nullptr;
     
        TreeNode* root = new TreeNode(stoi(current));
        root->left = deserializeDFS(nodes);
        root->right = deserializeDFS(nodes);
    
        return root;
    }
    ```

  - 注：将一整个 `string` 按逗号分割为一个队列

    ```cpp
    // 使用 stringstream 和 getline 按照 ',' 切割字符串
    stringstream ss(data); // data就是带逗号的整个string
    queue<string> nodes;
    
    string str;
    while (getline(ss, str, ',')) {
        if (!str.empty()) {
            nodes.push(str);
        }
    }
    ```

  - 完整题解代码：

    ```cpp
    class Codec {
    private:
        // 前序序列化二叉树
        string serializeDFS(TreeNode* root) {
            if (root == nullptr) return "#,";
            return to_string(root->val) + "," + serializeDFS(root->left) + serializeDFS(root->right);
        }
    
        // 前序反序列化二叉树
        TreeNode* deserializeDFS(queue<string>& nodes) {
            if (nodes.empty()) return nullptr;
            
            string current = nodes.front();
            nodes.pop();
            
            if (current == "#") return nullptr;
        
            TreeNode* root = new TreeNode(stoi(current));
            root->left = deserializeDFS(nodes);
            root->right = deserializeDFS(nodes);
    
            return root;
        }
    
    public:
        string serialize(TreeNode* root) { return serializeDFS(root); }
    
        TreeNode* deserialize(string data) {
            stringstream ss(data);
            queue<string> nodes;
    
            string str;
            while (getline(ss, str, ',')) {
                if (!str.empty()) {
                    nodes.push(str);
                }
            }
    
            return deserializeDFS(nodes);
        }
    };
    ```

- 层序遍历序列化和反序列化；

  利用先前的按层BFS，各层依次序列化；

  ```
  	1
     / \
    2   3			序列化结果为：1,2,3,#,#,4,5,#,#,#,#,
       / \
      4   5
  ```

  反序列也是一层一层来就行了

  ```cpp
  string serialize(TreeNode *root) {
      if (root == nullptr) return "#,";
  
      queue<TreeNode *> q;
      string ans = "";
  
      q.push(root);
  
      while (!q.empty()) {
          TreeNode *current = q.front();
          q.pop();
  
          // 为什么不需要 level_size 的 for 循环了？
          // 因为不需要关心层与层之间的边界，只要逻辑上是按层序列化就行了
          if (current == nullptr) {
              ans += "#,"; // 如果是空指针可不要去push它的左右孩子
          } else {
              ans += to_string(current->val) + ",";
              q.push(current->left);
              q.push(current->right);
          }
      }
  
      return ans;
  }
  ```

  ```cpp
  TreeNode* deserialize(string data) {
      stringstream ss(data);
      queue<string> nodes;
  
      string str;
      while (getline(ss, str, ',')) {
          if (!str.empty()) {
              nodes.push(str);
          }
      }
  
      if (nodes.empty() || nodes.front() == "#") { return nullptr; }
  
      // 创建根节点并让它入队
      TreeNode* root = new TreeNode(stoi(nodes.front()));
      nodes.pop();
  
      queue<TreeNode*> q; // 专门用来记录当前层节点的队列
      q.push(root);
  
      while (!q.empty()) {
          TreeNode* parent = q.front();
          q.pop();
  
          // 1. 处理左子节点
          if (!nodes.empty()) {
              string left_val = nodes.front();
              nodes.pop();
              if (left_val != "#") {
                  parent->left = new TreeNode(stoi(left_val));
                  q.push(parent->left); // 生成了有效节点，入队以便后续处理它的子节点
              }
          }
  
          // 2. 处理右子节点
          if (!nodes.empty()) {
              string right_val = nodes.front();
              nodes.pop();
              if (right_val != "#") {
                  parent->right = new TreeNode(stoi(right_val));
                  q.push(parent->right); // 生成了有效节点，入队
              }
          }
      }
  
      return root;
  }
  ```

---

##### Problem 7

- 题目：[LeetCode - 105. 从前序与中序遍历序列构造二叉树](https://leetcode.cn/problems/construct-binary-tree-from-preorder-and-inorder-traversal/)；

- 典型的递归设计；

  - 首先定义函数行为：

    设 `TreeNode* buildTreeHelper(const vector<int>& preorder, int pre_start, int pre_end, const vector<int>& inorder, int in_start, int in_end)` 返回根据前序序列中 `[pre_start, pre_end]` 和中序序列中 `[in_start, in_end]` 构建出来的二叉树的根节点指针；

    > 显然要求 `[pre_start, pre_end]` 和 `[in_start, in_end]` 对应的是同一棵二叉树；

  - 然后给出递归方程 / 递归逻辑：

    前序序列为：`[ 根节点 | 左子树的前序遍历 | 右子树的前序遍历 ]`；因此，`pre_start` 对应的节点必然是该二叉树的根节点；中序序列为：`[ 左子树的中序遍历 | 根节点 | 右子树的中序遍历 ]`；因此，根节点左边就是该二叉树左子树的中序遍历；

    由此，就可分别确定左右子树的前序序列和中序序列；递归逻辑也就出来了；

    > 这里也可看出为什么题目限定没有重复元素；

  - 最后给出边界情况：

    由于这是典型的不断缩小区间的递归，所以边界条件一般都可从区间非法时考虑；

    此题中，如果区间非法，说明当前子树为空，直接返回空指针即可；

  - 逻辑清晰后，code就不难了；

  ```cpp
  class Solution {
  private:
      unordered_map<int, int> in_map; // 便于在中序序列中找到根节点的索引
      
      TreeNode* buidTreeHelper(const vector<int>& preorder, int pre_start, int pre_end, const vector<int>& inorder, int in_start, int in_end) {    
          if (pre_start > pre_end || in_start > in_end) return nullptr;
          
          int root_val = preorder[pre_start]; // 根节点值
          TreeNode* root = new TreeNode(root_val);
          
          int in_root_index = in_map[root_val]; // 根节点在中序序列中的索引
          int left_tree_size = in_root_index - in_start; // 左子树序列长度
          
          root->left = buildTreeHelper(preorder, pre_start + 1, pre_start + left_tree_size, inorder, in_start, in_root_index - 1); // 构建左子树；
          root->right = buildTreeHelper(preorder, pre_start + left_tree_size + 1, pre_end, inorder, in_root_index + 1, in_end); // 构建右子树
          
          return root;
      }
      
  
  public:
      TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
          in_map.clear();
          for (int i = 0; i < inorder.size(); i++) {
              in_map[inorder[i]] = i;
          }
          
          return buildTreeHelper(preorder, 0, preorder.size() - 1, inorder, 0, inorder.size() - 1);
      }
  };
  ```

  复杂度显然是 $O(N)$ 的；

---

##### Problem 8

- 

