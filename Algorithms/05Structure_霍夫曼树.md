# Huffman Tree

> Author：玖沐 (Jura)  |  Last Updated：2026/11/02

## Theory

##### WPL

- 树的带权路径长度 (Weighted Path Length of Tree, WPL)；

  - 设有 $n$ 个带权叶子，$w_i$ 为第 $i$ 个叶子的权值，$l_i$ 为从根到第 $i$ 个叶子的路径长，则：$WPL = \sum_{i=1}^{n} w_i l_i$；

  ![带权路径长度示例](assets/huffman_wpl_example.svg)
  
- 如果给定一组带权叶子，那么可构造很多二叉树，其中WPL最小的二叉树称为Huffman Tree；

  - 此处设边无权 或 等权；
  - 显然的是，叶子权值越小离根越远，反之权值越大越靠近根；
  - 另外在Huffman Tree中，只有叶子的度为 $0$，其余节点度均为 $2$；

---

##### Huffman Algorithm

- 初始化：建立一个二叉树集合 $F$，并将给定的叶子视为 $n$ 棵单节点二叉树；

- 选取与合并：从 $F$ 中挑出**根权值最小的两颗树**，并将它们连接到新根上，新根权值为二者之和；

- 重复，直到集合中只剩下一棵二叉树，即为Huffman Tree；

  ![霍夫曼树的构造过程](assets/huffman_construction.svg)

  > 给定同一组带权叶子，Huffman Tree 可能不唯一；上述算法每次运行只构造其中一棵；
  
  > 所有非叶节点 (图中方形节点) 的权值累加和就是最小的WPL；
  >
  > 实现时每次从堆中取最小的两个值，记为 $a,b$，并记 $s = a+b$，将 $s$ 累加进WPL，然后将 $s$ 重新放回堆即可；

---

##### Application : Huffman Code

- 前缀编码：任一字符的编码都不是其他字符编码的前缀，如 $0$、$10$、$11$，因此连续读取不会产生歧义；

- 前缀编码树：字符位于叶子，左、右边分别记为 $0$、$1$；从根到叶子的 $0/1$ 序列就是字符编码；

- **若叶子权值为字符出现次数，则路径长就是编码位数，WPL 就是总编码位数**；

- Huffman Tree 的 WPL 最小，因此得到总位数最少的前缀编码；







## Practice

##### LG1090

- [洛谷 - P1090 合并果子](https://www.luogu.com.cn/problem/P1090)；

  没啥好说的，最多手搓一下最小堆；

  ```cpp
  int main() {
      int N;
      cin >> N;
      MinHeap h(N); // 手搓最小堆实现略
  
      for (int i = 0; i < N; i++) {
          int val;
          cin >> val;
          h.push(val);
      }
  
      int WPL = 0;
      while (h.size() > 1) { // 只要堆中元素个数比1多
          int min1 = h.pop();
          int min2 = h.pop();
  
          WPL += (min1 + min2); // 自底向上求WPL
  
          h.push(min1 + min2);
      }
  
      cout << WPL << "\n";
      
      return 0;
  }
  ```

---

