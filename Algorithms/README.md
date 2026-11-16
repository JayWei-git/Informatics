# README

- 以[左程云老师](https://space.bilibili.com/8888480)的B站算法系列为主线进行的算法学习之旅；同时参考[OI WiKi](https://oi-wiki.org/)；
- 如没有特殊说明，LGXXX一般来自洛谷主题库，即P编号的题；



## Agent 更新 README 流程

1. 先检查本目录的笔记、分类前缀、`assets/` 和本文件中的视频清单，再修改对应的统计与路径；保留与本次更新无关的内容；
2. 笔记统计范围为本目录及 `未处理/` 下的 Markdown 笔记，排除 `README.md` 和 `classzuo/`；去除空白后计字符，汉字按 Unicode 区间 U+4E00—U+9FFF 统计；`cpp` 代码块单独统计块数、代码行数和非空白代码行数，不计围栏行；
3. 已完成题目数按可核对的笔记记录计算：以二级至六级标题划分小节，只计有题目链接、同一小节有 `cpp` 解法代码、标题未标“不会”的题目；同一题的链接去掉查询参数和末尾斜杠后跨笔记去重；这项数字不代表在线评测平台的 AC 数；
4. 根据视频清单的勾选状态和每项时长重算已看、未看、合计及进度；检查图片相对路径和插图表中的所属笔记；核对统计日期、分类篇数与实际文件，文档句末使用全角中文分号；



## 笔记组织

已分类笔记直接放在本目录，文件名采用 `编号英文分类_主题.md`，例如 `05Structure_并查集.md`；尚未分类的笔记放在 `未处理/`；`Books/`、`classzuo/` 和 `assets/` 分别存放书籍、参考代码和插图；

| 原分类 | 文件名前缀 | 当前笔记数 |
| --- | --- | ---: |
| 00 算法基础 | `00Basic` | 4 |
| 01 搜索 | `01Search` | 1 |
| 02 动态规划 | `02DP` | 1 |
| 03 字符串 | `03String` | 2 |
| 04 数学 | `04Math` | 0 |
| 05 数据结构 | `05Structure` | 6 |
| 06 图论 | `06Graph` | 0 |
| 07 计算几何 | `07Geometry` | 0 |
| 08 杂项 | `08Misc` | 2 |
| 09 专题 | `09Topic` | 0 |



## 笔记统计

截至 2026-11-08，递归统计本目录下的 Markdown 笔记，排除 `classzuo/` 和本文件 `README.md`，共 25 篇，其中根目录 16 篇、`未处理/` 9 篇；按文件原文统计，去除空白后有 **205,774 个字符**，其中 **36,596 个汉字**；字符数包含代码和 Markdown 标记，PDF 与 SVG 文件未计入；

同一范围内共有 276 个 `cpp` 代码块，代码共 **7,981 行**（含空行），其中 **6,744 行非空白代码**；统计包含注释，不含代码围栏标记和未标注为 `cpp` 的树形示意块；

按上述口径，笔记中**已完成题目数为 163 题**；



## Assets

由于小项较多，Algorithms 下的插图统一放在根目录 `assets/` 中管理；根目录笔记使用 `assets/文件名` 引用，`未处理/` 中的笔记使用 `../assets/文件名` 引用；SVG 文件名保留主题前缀，便于识别所属内容；

| 所属笔记 | SVG 文件名 |
| --- | --- |
| `未处理/前缀树.md` | [trie_insert_ac.svg](assets/trie_insert_ac.svg) |
| `03String_KMP.md` | [kmp_ababa_lps_equals_3.svg](assets/kmp_ababa_lps_equals_3.svg) |
| `03String_KMP.md` | [kmp_mismatch_shift_and_skip_matched_text.svg](assets/kmp_mismatch_shift_and_skip_matched_text.svg) |
| `03String_KMP.md` | [kmp_next_extend_longest_border.svg](assets/kmp_next_extend_longest_border.svg) |
| `03String_KMP.md` | [kmp_next_fallback_to_shorter_border.svg](assets/kmp_next_fallback_to_shorter_border.svg) |
| `03String_KMP.md` | [kmp_p4824_stack_restore.svg](assets/kmp_p4824_stack_restore.svg) |
| `05Structure_有序表.md` | [avl_cases.svg](assets/avl_cases.svg) |
| `05Structure_有序表.md` | [avl_ll_rr.svg](assets/avl_ll_rr.svg) |
| `05Structure_有序表.md` | [avl_lr_rl.svg](assets/avl_lr_rl.svg) |
| `05Structure_有序表.md` | [avl_rotations_transparent.svg](assets/avl_rotations_transparent.svg) |
| `05Structure_霍夫曼树.md` | [huffman_construction.svg](assets/huffman_construction.svg) |
| `05Structure_霍夫曼树.md` | [huffman_wpl_example.svg](assets/huffman_wpl_example.svg) |



## Video

| 状态 | 期数 | 总时长 |
| --- | ---: | ---: |
| 已看 | 55 | 65:03:00 |
| 未看 | 152 | 306:52:26 |
| 合计 | 207 | 371:55:26 |

**时长进度：17.49%**；期数进度：26.57%；

- [x] [左程云 - 算法001[入门] 学习算法的语言问题以及如何开通gpt4](https://www.bilibili.com/video/BV1eu4y117KQ)（00:15:34）；
- [x] [左程云 - 算法002[入门] 从社会实验到入门提醒](https://www.bilibili.com/video/BV1Q14y1B7DH)（00:18:22）；
- [x] [左程云 - 算法003[入门] 二进制和位运算](https://www.bilibili.com/video/BV1494y1C72o)（01:06:03）；
- [x] [左程云 - 算法004[入门] 选择、冒泡、插入排序](https://www.bilibili.com/video/BV12P41147to)（00:24:57）；
- [x] [左程云 - 算法005[入门] 对数器-验证的重要手段](https://www.bilibili.com/video/BV1mM4y1H7fz)（00:23:16）；
- [x] [左程云 - 算法006[入门] 二分搜索](https://www.bilibili.com/video/BV1bX4y177uT)（00:53:15）；
- [x] [左程云 - 算法007[入门] 时间复杂度和空间复杂度](https://www.bilibili.com/video/BV1JV411G7De)（00:52:01）；
- [x] [左程云 - 算法008[入门] 算法和数据结构简介](https://www.bilibili.com/video/BV1zN411h7Kx)（00:10:14）；
- [x] [左程云 - 算法009[入门] 单双链表及其反转-堆栈诠释](https://www.bilibili.com/video/BV1N94y1C7az)（00:28:47）；
- [x] [左程云 - 算法010[入门] 链表入门题目-合并两个有序链表](https://www.bilibili.com/video/BV1nm4y1W75L)（00:14:48）；
- [x] [左程云 - 算法011[入门] 链表入门题目-两个链表相加](https://www.bilibili.com/video/BV1bh4y1r75d)（00:12:58）；
- [x] [左程云 - 算法012[入门] 链表入门题目-划分链表](https://www.bilibili.com/video/BV1SV4y1i76r)（00:12:29）；
- [x] [左程云 - 算法013[入门] 队列和栈-链表、数组实现](https://www.bilibili.com/video/BV18X4y177vA)（00:37:56）；
- [x] [左程云 - 算法014[入门] 队列和栈入门题目-栈和队列相互实现](https://www.bilibili.com/video/BV1E14y1B7j4)（00:16:29）；
- [x] [左程云 - 算法015[入门] 栈的入门题目-最小栈](https://www.bilibili.com/video/BV15X4y177cM)（00:12:33）；
- [x] [左程云 - 算法016[入门] 双端队列-双链表和固定数组实现](https://www.bilibili.com/video/BV1PM4y1p7N5)（00:33:31）；
- [x] [左程云 - 算法017[入门] 二叉树及其三种序的递归实现](https://www.bilibili.com/video/BV12p4y1V728)（00:26:05）；
- [x] [左程云 - 算法018[入门] 二叉树遍历的非递归实现和复杂度分析](https://www.bilibili.com/video/BV15P411t7e2)（00:44:14）；
- [x] [左程云 - 算法019[必备] 算法笔试中处理输入和输出](https://www.bilibili.com/video/BV1sh4y1w7uv)（00:37:55）；
- [x] [左程云 - 算法020[必备] 递归和master公式](https://www.bilibili.com/video/BV1kV411G7wP)（00:47:46）；
- [x] [左程云 - 算法021[必备] 归并排序](https://www.bilibili.com/video/BV1wu411p7r7)（00:52:52）；
- [x] [左程云 - 算法022[必备] 归并分治](https://www.bilibili.com/video/BV1L14y1B7ef)（01:00:31）；
- [x] [左程云 - 算法023[必备] 随机快速排序](https://www.bilibili.com/video/BV1cc411F7Y6)（01:08:46）；
- [x] [左程云 - 算法024[必备] 随机选择算法](https://www.bilibili.com/video/BV1mN411b71K)（00:19:34）；
- [x] [左程云 - 算法025[必备] 堆结构和堆排序](https://www.bilibili.com/video/BV1fu4y1q77y)（01:27:36）；
- [x] [左程云 - 算法026[必备] 哈希表、有序表和比较器的用法](https://www.bilibili.com/video/BV13m4y1p7VB)（00:50:32）；
- [x] [左程云 - 算法027[必备] 堆结构常见题](https://www.bilibili.com/video/BV1Gm4y1p7UE)（01:06:52）；
- [x] [左程云 - 算法028[必备] 基数排序](https://www.bilibili.com/video/BV1Lh4y1c7Aw)（00:37:42）；
- [x] [左程云 - 算法029[必备] 重要排序算法的总结](https://www.bilibili.com/video/BV1Kh4y1c7Pu)（00:15:18）；
- [x] [左程云 - 算法030[必备] 异或运算的骚操作](https://www.bilibili.com/video/BV1LN411z7nu)（01:15:54）；
- [x] [左程云 - 算法031[必备] 位运算的骚操作](https://www.bilibili.com/video/BV1ch4y1Q7vd)（00:57:45）；
- [x] [左程云 - 算法032[必备] 位图](https://www.bilibili.com/video/BV1yr4y1Z7jb)（01:01:00）；
- [ ] [左程云 - 算法033[必备] 位运算实现加减乘除](https://www.bilibili.com/video/BV1up4y1g7d8)（00:54:36）；
- [x] [左程云 - 算法034[必备] 链表高频题目和必备技巧](https://www.bilibili.com/video/BV1A14y1i7GY)（01:17:47）；
- [ ] [左程云 - 算法035[必备] 数据结构设计高频题](https://www.bilibili.com/video/BV1nF411y7rD)（01:31:08）；
- [x] [左程云 - 算法036[必备] 二叉树高频题目-上-不含树型dp](https://www.bilibili.com/video/BV1Rp4y1g7ys)（02:00:28）；
- [x] [左程云 - 算法037[必备] 二叉树高频题目-下-不含树型dp](https://www.bilibili.com/video/BV1194y16727)（01:28:39）；
- [x] [左程云 - 算法038[必备] 常见经典递归过程解析](https://www.bilibili.com/video/BV19m4y1n7mo)（01:57:40）；
- [ ] [左程云 - 算法039[必备] 嵌套类问题的递归解题套路](https://www.bilibili.com/video/BV1JP411p7KG)（01:14:30）；
- [ ] [左程云 - 算法040[必备] N皇后问题-重点是位运算的版本](https://www.bilibili.com/video/BV1gr4y1R79Z)（01:05:15）；
- [ ] [左程云 - 算法041[必备] 最大公约数、同余原理](https://www.bilibili.com/video/BV1Wu4y1i7m3)（00:42:55）；
- [ ] [左程云 - 算法042[必备] 对数器打表找规律的技巧](https://www.bilibili.com/video/BV11u4y1Q7FD)（00:44:24）；
- [ ] [左程云 - 算法043[必备] 根据数据量猜解法的技巧-天字第一号重要技巧](https://www.bilibili.com/video/BV1Cm4y1M72N)（01:08:03）；
- [ ] [左程云 - 算法044[必备] 前缀树原理和代码详解](https://www.bilibili.com/video/BV1Yu4y1Q7vR)（01:13:56）；
- [ ] [左程云 - 算法045[必备] 前缀树的相关题目](https://www.bilibili.com/video/BV16h4y1K7Sp)（01:30:21）；
- [ ] [左程云 - 算法046[必备] 构建前缀信息的技巧-解决子数组相关问题](https://www.bilibili.com/video/BV1Sj411q7fi)（01:15:22）；
- [ ] [左程云 - 算法047[必备] 一维差分与等差数列差分](https://www.bilibili.com/video/BV1Gp4y1E7Jp)（00:55:29）；
- [ ] [左程云 - 算法048[必备] 二维前缀和、二维差分、离散化技巧](https://www.bilibili.com/video/BV1Wz4y1K74C)（01:59:00）；
- [ ] [左程云 - 算法049[必备] 滑动窗口技巧与相关题目](https://www.bilibili.com/video/BV1DG411d7fh)（02:08:13）；
- [x] [左程云 - 算法050[必备] 双指针技巧与相关题目](https://www.bilibili.com/video/BV1V841167Rg)（02:01:08）；
- [ ] [左程云 - 算法051[必备] 二分答案法与相关题目](https://www.bilibili.com/video/BV1Mh4y1P7qE)（02:44:03）；
- [ ] [左程云 - 算法052[必备] 单调栈-上](https://www.bilibili.com/video/BV1HH4y1X7T9)（01:48:36）；
- [ ] [左程云 - 算法053[必备] 单调栈-下](https://www.bilibili.com/video/BV1GH4y1D7TB)（01:38:16）；
- [ ] [左程云 - 算法054[必备] 单调队列-上](https://www.bilibili.com/video/BV11h4y1w7Bu)（01:29:53）；
- [ ] [左程云 - 算法055[必备] 单调队列-下](https://www.bilibili.com/video/BV1y8411i7cY)（01:35:23）；
- [x] [左程云 - 算法056[必备] 并查集-上](https://www.bilibili.com/video/BV1894y1W7Sb)（01:29:48）；
- [x] [左程云 - 算法057[必备] 并查集-下](https://www.bilibili.com/video/BV1Ny4y1F71J)（01:57:13）；
- [x] [左程云 - 算法058[必备] 洪水填充](https://www.bilibili.com/video/BV1VF411S7RH)（00:56:34）；
- [x] [左程云 - 算法059[必备] 建图、链式前向星、拓扑排序](https://www.bilibili.com/video/BV1rj411k7tS)（02:10:15）；
- [x] [左程云 - 算法060[必备] 拓扑排序的扩展技巧](https://www.bilibili.com/video/BV12y4y1F79q)（01:15:05）；
- [x] [左程云 - 算法061[必备] 最小生成树](https://www.bilibili.com/video/BV1sK4y1F7LH)（01:55:24）；
- [x] [左程云 - 算法062[必备] 宽度优先遍历及其扩展](https://www.bilibili.com/video/BV1Dw411w7P5)（02:54:27）；
- [x] [左程云 - 算法063[必备] 双向广搜](https://www.bilibili.com/video/BV11w411y79P)（01:12:19）；
- [x] [左程云 - 算法064[必备] Dijkstra算法、分层图最短路](https://www.bilibili.com/video/BV1Cm4y1g77W)（02:29:47）；
- [ ] [左程云 - 算法065[必备] A星、Floyd、Bellman-Ford与SPFA](https://www.bilibili.com/video/BV1t94y187zW)（02:04:28）；
- [x] [左程云 - 算法066[必备] 从递归入手一维动态规划](https://www.bilibili.com/video/BV1Ww41167Ac)（03:53:04）；
- [x] [左程云 - 算法067[必备] 从递归入手二维动态规划](https://www.bilibili.com/video/BV1WQ4y1W7d1)（02:53:30）；
- [x] [左程云 - 算法068[必备] 见识更多二维动态规划题目](https://www.bilibili.com/video/BV1cg4y1o719)（01:35:00）；
- [x] [左程云 - 算法069[必备] 从递归入手三维动态规划](https://www.bilibili.com/video/BV1gM41197rM)（02:00:12）；
- [x] [左程云 - 算法070[必备] 子数组最大累加和问题与扩展-上](https://www.bilibili.com/video/BV1pw411M7Du)（01:33:12）；
- [x] [左程云 - 算法071[必备] 子数组最大累加和问题与扩展-下](https://www.bilibili.com/video/BV1qu4y187Xn)（02:06:48）；
- [ ] [左程云 - 算法072[必备] 最长递增子序列问题与扩展](https://www.bilibili.com/video/BV1ne411D7CQ)（01:43:11）；
- [ ] [左程云 - 算法073[必备] 背包dp-01背包、有依赖的背包](https://www.bilibili.com/video/BV1NN41137jF)（02:23:13）；
- [ ] [左程云 - 算法074[必备] 背包dp-分组背包、完全背包](https://www.bilibili.com/video/BV1UM411f7YL)（01:53:52）；
- [ ] [左程云 - 算法075[必备] 背包dp-多重背包、混合背包](https://www.bilibili.com/video/BV1Nz4y1c71M)（01:47:08）；
- [ ] [左程云 - 算法076[必备] 区间dp-上](https://www.bilibili.com/video/BV1NQ4y1b7Uo)（02:01:30）；
- [ ] [左程云 - 算法077[必备] 区间dp-下](https://www.bilibili.com/video/BV1du4y1L7gy)（02:22:46）；
- [ ] [左程云 - 算法078[必备] 树型dp-上](https://www.bilibili.com/video/BV1194y1J7ud)（02:14:17）；
- [ ] [左程云 - 算法079[必备] 树型dp-下](https://www.bilibili.com/video/BV1ae411f7AC)（02:21:56）；
- [ ] [左程云 - 算法080[必备] 状压dp-上](https://www.bilibili.com/video/BV15a4y1o7NA)（01:42:29）；
- [ ] [左程云 - 算法081[必备] 状压dp-下](https://www.bilibili.com/video/BV1Tu4y1g7GU)（02:20:25）；
- [ ] [左程云 - 算法082[必备] 动态规划中用观察优化枚举的技巧-上](https://www.bilibili.com/video/BV1PN411j7aG)（02:20:41）；
- [ ] [左程云 - 算法083[必备] 动态规划中用观察优化枚举的技巧-下](https://www.bilibili.com/video/BV1pN41157uX)（01:53:34）；
- [ ] [左程云 - 算法084[必备] 数位dp-上](https://www.bilibili.com/video/BV1cC4y1Q7r3)（01:29:01）；
- [ ] [左程云 - 算法085[必备] 数位dp-下](https://www.bilibili.com/video/BV1uC4y1Q72c)（01:46:11）；
- [ ] [左程云 - 算法086[必备] 动态规划中得到具体决策方案的技巧](https://www.bilibili.com/video/BV1L94y1w72w)（01:41:10）；
- [ ] [左程云 - 算法087[必备] 动态规划中根据数据量猜解法的技巧](https://www.bilibili.com/video/BV1ba4y1673e)（01:36:14）；
- [ ] [左程云 - 算法088[必备] 动态规划专题总结与预告](https://www.bilibili.com/video/BV1YQ4y1u7Tz)（00:13:41）；
- [x] [左程云 - 算法089[必备] 贪心经典题目专题1](https://www.bilibili.com/video/BV1be41167yw)（01:22:53）；
- [ ] [左程云 - 算法090[必备] 贪心经典题目专题2](https://www.bilibili.com/video/BV1Rp4y1973N)（01:17:31）；
- [ ] [左程云 - 算法091[必备] 贪心经典题目专题3](https://www.bilibili.com/video/BV1ST4y1s7XT)（01:19:03）；
- [ ] [左程云 - 算法092[必备] 贪心经典题目专题4](https://www.bilibili.com/video/BV1MG411z7Yz)（01:14:10）；
- [ ] [左程云 - 算法093[必备] 贪心经典题目专题5](https://www.bilibili.com/video/BV16Q4y1L7s9)（01:18:12）；
- [ ] [左程云 - 算法094[必备] 贪心经典题目专题6](https://www.bilibili.com/video/BV1r5411q7UV)（01:13:29）；
- [ ] [左程云 - 算法095[必备] 博弈类问题必备内容详解-上](https://www.bilibili.com/video/BV1T5411i7Gg)（01:23:24）；
- [ ] [左程云 - 算法096[必备] 博弈类问题必备内容详解-下](https://www.bilibili.com/video/BV1N94y1T7Mb)（02:21:42）；
- [ ] [左程云 - 算法097[必备] 质数判断、质因子分解、质数筛](https://www.bilibili.com/video/BV1oK4y1z7ML)（01:20:51）；
- [ ] [左程云 - 算法098[必备] 快速幂、矩阵快速幂与两类问题](https://www.bilibili.com/video/BV13k4y1D7Dn)（02:08:10）；
- [ ] [左程云 - 算法099[扩展] 逆元和除法同余、容斥原理](https://www.bilibili.com/video/BV1SW4y1F7Tq)（02:00:56）；
- [x] [左程云 - 算法100[扩展] KMP算法原理和代码详解](https://www.bilibili.com/video/BV19Q4y1c7ko)（01:36:01）；
- [ ] [左程云 - 算法101[扩展] KMP算法相关题目](https://www.bilibili.com/video/BV13m41197tS)（01:39:31）；
- [ ] [左程云 - 算法102[扩展] AC自动机原理、优化、代码详解](https://www.bilibili.com/video/BV1Sy421a78C)（02:44:32）；
- [ ] [左程云 - 算法103[扩展] Manacher算法、扩展KMP](https://www.bilibili.com/video/BV1YH4y1h7MU)（01:47:37）；
- [ ] [左程云 - 算法104[扩展] Manacher算法相关题目](https://www.bilibili.com/video/BV1LC41147Lh)（01:03:40）；
- [ ] [左程云 - 算法105[扩展] 字符串哈希原理、代码、题目详解](https://www.bilibili.com/video/BV1hu4m1A7AF)（02:02:39）；
- [ ] [左程云 - 算法106[扩展] 哈希函数、哈希表、布隆过滤器、一致性哈希](https://www.bilibili.com/video/BV1FJ4m1e7Sz)（01:45:57）；
- [ ] [左程云 - 算法107[扩展] 大厂面试中经常漫聊的有趣算法问题](https://www.bilibili.com/video/BV1bK421b789)（02:12:20）；
- [ ] [左程云 - 算法108[扩展] 树状数组原理、扩展、代码详解](https://www.bilibili.com/video/BV1ZZ421a7uH)（02:01:26）；
- [ ] [左程云 - 算法109[扩展] 树状数组相关题目](https://www.bilibili.com/video/BV1HH4y1s7fC)（03:03:26）；
- [ ] [左程云 - 算法110[扩展] 线段树专题1-线段树原理和代码详解](https://www.bilibili.com/video/BV12i421X76h)（03:09:58）；
- [ ] [左程云 - 算法111[扩展] 线段树专题2-线段树的离散化、二分搜索、特别修改](https://www.bilibili.com/video/BV1qm411k7yX)（02:00:34）；
- [ ] [左程云 - 算法112[扩展] 线段树专题3-线段树维护更多类型的信息](https://www.bilibili.com/video/BV1Qf421Z7uA)（01:13:23）；
- [ ] [左程云 - 算法113[扩展] 线段树专题4-线段树解决区间合并的问题](https://www.bilibili.com/video/BV1Gz421y7YX)（01:54:49）；
- [ ] [左程云 - 算法114[扩展] 线段树专题5-开点线段树、区间最值和历史最值](https://www.bilibili.com/video/BV1HK421a75t)（02:43:04）；
- [ ] [左程云 - 算法115[扩展] 线段树专题6-线段树与扫描线结合的题目](https://www.bilibili.com/video/BV16x4y1a7Ro)（01:56:14）；
- [ ] [左程云 - 算法116[扩展] 摩尔投票大加强，线段树里捉海王](https://www.bilibili.com/video/BV1Br421G7hB)（01:04:29）；
- [ ] [左程云 - 算法117[扩展] 倍增算法和ST表](https://www.bilibili.com/video/BV1At421A7UP)（01:52:35）；
- [ ] [左程云 - 算法118[扩展] 树上问题专题1-树上倍增和LCA-上](https://www.bilibili.com/video/BV1DZ421J7Uc)（01:27:12）；
- [ ] [左程云 - 算法119[扩展] 树上问题专题2-树上倍增和LCA-下](https://www.bilibili.com/video/BV1Yi421k7cL)（01:57:40）；
- [ ] [左程云 - 算法120[扩展] 树上问题专题3-树的重心](https://www.bilibili.com/video/BV1Pf421m7Ts)（00:53:45）；
- [ ] [左程云 - 算法121[扩展] 树上问题专题4-树的直径](https://www.bilibili.com/video/BV1ws421N76M)（02:15:53）；
- [ ] [左程云 - 算法122[扩展] 树上问题专题5-树上差分](https://www.bilibili.com/video/BV1WZ42147Gc)（01:49:31）；
- [ ] [左程云 - 算法123[扩展] 树上问题专题6-换根dp](https://www.bilibili.com/video/BV1wt421u7Eo)（02:28:07）；
- [ ] [左程云 - 算法124[扩展] Morris遍历](https://www.bilibili.com/video/BV1Yt421K7Yu)（01:45:23）；
- [ ] [左程云 - 算法125[扩展] 轮廓线dp](https://www.bilibili.com/video/BV1vU411d7s7)（01:57:00）；
- [ ] [左程云 - 算法126[扩展] 三进制状压dp](https://www.bilibili.com/video/BV1Zs421u7Q5)（01:24:33）；
- [ ] [左程云 - 算法127[扩展] 动态规划优化专题1-优化尝试和状态设计-上](https://www.bilibili.com/video/BV1VS411K7bk)（02:13:00）；
- [ ] [左程云 - 算法128[扩展] 动态规划优化专题2-优化尝试和状态设计-下](https://www.bilibili.com/video/BV1vT421Y7Dr)（02:02:47）；
- [ ] [左程云 - 算法129[扩展] 动态规划优化专题3-倍增优化](https://www.bilibili.com/video/BV1Z6421f7jN)（02:23:16）；
- [ ] [左程云 - 算法130[扩展] 动态规划优化专题4-单调队列和单调栈优化](https://www.bilibili.com/video/BV18z42187JH)（03:18:17）；
- [ ] [左程云 - 算法131[扩展] 动态规划优化专题5-树状数组和线段树优化](https://www.bilibili.com/video/BV1eT421a7uE)（03:05:00）；
- [ ] [左程云 - 算法132[扩展] 动态规划优化专题6-预处理数组优化](https://www.bilibili.com/video/BV1B4421D77S)（02:21:25）；
- [ ] [左程云 - 算法133[扩展] 高斯消元专题1-解决加法方程组](https://www.bilibili.com/video/BV1gT421k728)（01:32:25）；
- [ ] [左程云 - 算法134[扩展] 高斯消元专题2-解决异或方程组](https://www.bilibili.com/video/BV1Bb421J7sN)（01:39:23）；
- [ ] [左程云 - 算法135[扩展] 高斯消元专题3-解决同余方程组](https://www.bilibili.com/video/BV12H4y1c7HH)（01:19:34）；
- [ ] [左程云 - 算法136[扩展] 线性基-上](https://www.bilibili.com/video/BV1iz421i7eW)（01:30:16）；
- [ ] [左程云 - 算法137[扩展] 线性基-下](https://www.bilibili.com/video/BV1yS421X7n1)（01:19:05）；
- [ ] [左程云 - 算法138[扩展] 01分数规划](https://www.bilibili.com/video/BV1Hz421i7fc)（01:33:03）；
- [ ] [左程云 - 算法139[扩展] 裴蜀定理和扩展欧几里得算法](https://www.bilibili.com/video/BV14gWxevEW4)（01:15:10）；
- [ ] [左程云 - 算法140[扩展] 扩展欧几里得和二元一次不定方程](https://www.bilibili.com/video/BV1rJW5efEix)（01:37:30）；
- [ ] [左程云 - 算法141[扩展] 中国剩余定理及其扩展](https://www.bilibili.com/video/BV17JHEebEGC)（01:30:12）；
- [ ] [左程云 - 算法142[扩展] 负环和差分约束](https://www.bilibili.com/video/BV1iNH9eREG3)（01:58:30）；
- [ ] [左程云 - 算法143[扩展] 同余最短路](https://www.bilibili.com/video/BV18htge6EuU)（02:06:57）；
- [ ] [左程云 - 算法144[扩展] 二项式定理证明和相关题目](https://www.bilibili.com/video/BV1V7s9ebEtQ)（01:16:13）；
- [ ] [左程云 - 算法145[扩展] 二项式反演的四种形式、证明、相关题目](https://www.bilibili.com/video/BV1bcx1ecEvK)（03:14:33）；
- [ ] [左程云 - 算法146[扩展] 康托展开、约瑟夫环、完美洗牌](https://www.bilibili.com/video/BV1Dz2eYTE7T)（02:15:15）；
- [ ] [左程云 - 算法147[扩展] 卡特兰数题型详解和取模处理](https://www.bilibili.com/video/BV1BACUY7EzN)（02:08:37）；
- [x] [左程云 - 算法148[扩展] 有序表专题1-AVL树](https://www.bilibili.com/video/BV1h5yXYJE6t)（02:12:11）；
- [ ] [左程云 - 算法149[扩展] 有序表专题2-跳表](https://www.bilibili.com/video/BV1HZ1jYMEt3)（01:23:15）；
- [ ] [左程云 - 算法150[扩展] 有序表专题3-替罪羊树](https://www.bilibili.com/video/BV1GqDSYbEPR)（01:40:03）；
- [ ] [左程云 - 算法151[扩展] 有序表专题4-笛卡尔树、Treap树](https://www.bilibili.com/video/BV16eUFY8EXH)（02:57:54）；
- [ ] [左程云 - 算法152[扩展] 有序表专题5-FHQ Treap](https://www.bilibili.com/video/BV1rYBiY6E87)（02:45:51）；
- [ ] [左程云 - 算法153[扩展] 有序表专题6-Splay树](https://www.bilibili.com/video/BV1xPioYPEXZ)（03:18:47）；
- [ ] [左程云 - 算法154[挺难] 左偏树的原理、代码、相关题目](https://www.bilibili.com/video/BV11HBFYGEH6)（02:07:43）；
- [ ] [左程云 - 算法155[挺难] 左偏树和懒更新、可持久化左偏树、k短路问题](https://www.bilibili.com/video/BV1BCCAYpEmH)（03:29:13）；
- [ ] [左程云 - 算法156[挺难] 带权并查集的原理和扩展题目](https://www.bilibili.com/video/BV1S8r7YrEHd)（02:33:51）；
- [ ] [左程云 - 算法157[挺难] 可持久化线段树和标记永久化](https://www.bilibili.com/video/BV1bSc6eREi4)（01:42:09）；
- [ ] [左程云 - 算法158[挺难] 可持久化线段树的相关题目](https://www.bilibili.com/video/BV1puw7emEVu)（02:31:57）；
- [ ] [左程云 - 算法159[挺难] 可持久化前缀树和相关题目](https://www.bilibili.com/video/BV1HnFuenEYC)（03:26:28）；
- [ ] [左程云 - 算法160[挺难] 树套树的原理、优化、相关题目](https://www.bilibili.com/video/BV1ZuPAeME8n)（02:44:52）；
- [ ] [左程云 - 算法161[挺难] 树链剖分-上](https://www.bilibili.com/video/BV11w9nY6EKq)（02:20:04）；
- [ ] [左程云 - 算法162[挺难] 树链剖分-下](https://www.bilibili.com/video/BV1UcQgYCEtj)（03:08:10）；
- [ ] [左程云 - 算法163[挺难] 树上启发式合并的原理和相关题目](https://www.bilibili.com/video/BV1ujo6YCEjt)（02:30:26）；
- [ ] [左程云 - 算法164[挺难] Kruskal重构树的原理和相关题目](https://www.bilibili.com/video/BV1xVZhYmEti)（02:15:27）；
- [ ] [左程云 - 算法165[挺难] 可持久化并查集、可撤销并查集](https://www.bilibili.com/video/BV1rrdSYWED7)（01:47:20）；
- [ ] [左程云 - 算法166[挺难] 线段树分治-上](https://www.bilibili.com/video/BV1RPL3zVEdo)（02:15:20）；
- [ ] [左程云 - 算法167[挺难] 线段树分治-下](https://www.bilibili.com/video/BV1WXVpzpEKS)（03:00:34）；
- [ ] [左程云 - 算法168[挺难] 整体二分-上](https://www.bilibili.com/video/BV1FXEbzLEZH)（01:24:19）；
- [ ] [左程云 - 算法169[挺难] 整体二分-下](https://www.bilibili.com/video/BV11hjHzbEuM)（03:48:09）；
- [ ] [左程云 - 算法170[挺难] CDQ分治-上](https://www.bilibili.com/video/BV1sa7zz1EVx)（01:51:01）；
- [ ] [左程云 - 算法171[挺难] CDQ分治-下](https://www.bilibili.com/video/BV1RsTiz4EAw)（02:23:30）；
- [ ] [左程云 - 算法172[挺难] 分块专题1-块状数组、块状链表](https://www.bilibili.com/video/BV1nXNvzaEvG)（01:48:22）；
- [ ] [左程云 - 算法173[挺难] 分块专题2-分块的几道好题、树上分块](https://www.bilibili.com/video/BV18V38zLEDa)（02:49:20）；
- [ ] [左程云 - 算法174[挺难] 分块专题3-双层分块、分块结合并查集](https://www.bilibili.com/video/BV1aiu8zkE38)（03:56:18）；
- [ ] [左程云 - 算法175[挺难] 分块专题4-根号分治](https://www.bilibili.com/video/BV1kftBzeEtz)（02:01:39）；
- [ ] [左程云 - 算法176[挺难] 莫队专题1-普通莫队、带修莫队](https://www.bilibili.com/video/BV1XxYQzAEJ8)（02:15:00）；
- [ ] [左程云 - 算法177[挺难] 莫队专题2-回滚莫队、树上莫队](https://www.bilibili.com/video/BV1Kvapz6EdZ)（02:48:15）；
- [ ] [左程云 - 算法178[挺难] 莫队专题3-莫队二次离线](https://www.bilibili.com/video/BV1Emp7z9E9S)（02:36:27）；
- [ ] [左程云 - 算法179[挺难] 莫队专题4-莫队综合应用](https://www.bilibili.com/video/BV1tQHGzQE7M)（02:48:16）；
- [ ] [左程云 - 算法180[挺难] 虚树的原理和相关题目](https://www.bilibili.com/video/BV1yYWYzGE38)（02:41:21）；
- [ ] [左程云 - 算法181[挺难] 线段树的合并与分裂-上](https://www.bilibili.com/video/BV1zRspz3EMF)（01:23:21）；
- [ ] [左程云 - 算法182[挺难] 线段树的合并与分裂-下](https://www.bilibili.com/video/BV18L1WB8Eyh)（02:02:13）；
- [ ] [左程云 - 算法183[挺难] 静态点分治-上](https://www.bilibili.com/video/BV1uhy5BeEW6)（02:13:20）；
- [ ] [左程云 - 算法184[挺难] 静态点分治-下](https://www.bilibili.com/video/BV1EhSvBXE4r)（03:02:01）；
- [ ] [左程云 - 算法185[挺难] 点分树和动态点分治](https://www.bilibili.com/video/BV1VmmsB5Ezz)（03:21:47）；
- [ ] [左程云 - 算法186[挺难] 欧拉序求LCA、dfn序求LCA](https://www.bilibili.com/video/BV1P3iGBcEPQ)（00:34:43）；
- [ ] [左程云 - 算法187[挺难] 边分治和边分树](https://www.bilibili.com/video/BV1rjiGBoEak)（03:00:02）；
- [ ] [左程云 - 算法188[挺难] 欧拉路径与相关题目](https://www.bilibili.com/video/BV1zFryBxEVJ)（03:18:08）；
- [ ] [左程云 - 算法189[挺难] 强连通分量和缩点-上](https://www.bilibili.com/video/BV18u6FBmEXc)（01:48:33）；
- [ ] [左程云 - 算法190[挺难] 强连通分量和缩点-下](https://www.bilibili.com/video/BV1wc6hBsEBw)（01:38:52）；
- [ ] [左程云 - 算法191[挺难] 割边、边双连通分量和缩点](https://www.bilibili.com/video/BV1PPZdB3Epn)（01:07:55）；
- [ ] [左程云 - 算法192[挺难] 边双连通分量缩点的更多题目](https://www.bilibili.com/video/BV1bcf3BtES5)（01:29:35）；
- [ ] [左程云 - 算法193[挺难] 割点和点双连通分量](https://www.bilibili.com/video/BV1vbPkzME1b)（01:32:07）；
- [ ] [左程云 - 算法194[挺难] 圆方树的原理和相关题目](https://www.bilibili.com/video/BV16UwczxENb)（02:13:35）；
- [ ] [左程云 - 算法195[挺难] 优化建图的技巧和题目](https://www.bilibili.com/video/BV1Bs9cBYEMe)（02:36:39）；
- [ ] [左程云 - 算法196[挺难] 2-SAT与前缀优化建图](https://www.bilibili.com/video/BV1qNdvBwEGa)（02:22:02）；
- [ ] [左程云 - 算法197[挺难] 2-SAT与优化建图的更多题目](https://www.bilibili.com/video/BV18bRcB9EH4)（02:13:10）；
- [ ] [左程云 - 算法198[挺难] 树状数组优化建图、主席树优化建图、CDQ优化建图](https://www.bilibili.com/video/BV15U5x6iECd)（02:07:20）；
- [ ] [左程云 - 算法199[挺难] 基环树和相关题目](https://www.bilibili.com/video/BV1tdVP6tEwZ)（02:33:04）；
- [ ] [左程云 - 算法200[挺难] 仙人掌图和相关题目](https://www.bilibili.com/video/BV1EzJs6HE6f)（03:40:24）；
- [ ] [左程云 - 算法201[挺难] LCT专题1-LCT的原理和模版题目](https://www.bilibili.com/video/BV1HL7V6HEsP)（01:59:23）；
- [ ] [左程云 - 算法202[挺难] LCT专题2-LCT维护路径、边双连通分量的题目](https://www.bilibili.com/video/BV1KZN86CEKK)（02:02:58）；
- [ ] [左程云 - 算法203[挺难] LCT专题3-LCT维护边权、生成树的题目](https://www.bilibili.com/video/BV17J346eEup)（01:48:12）；
- [ ] [左程云 - 算法204[挺难] LCT专题4-LCT维护子树、图结构的题目](https://www.bilibili.com/video/BV1f8gK6GEde)（02:39:58）；
- [ ] [左程云 - 算法205[挺难] K-D树的原理、代码、常见题目](https://www.bilibili.com/video/BV1u5Yt6VEwB)（02:39:23）；
- [ ] [左程云 - 算法206[挺难] K-D树的删除操作、树套树、优化建图](https://www.bilibili.com/video/BV1TXeu6tEtW)（01:53:59）；
- [ ] [左程云 - 算法207[挺难] Johnson全源最短路、传递闭包](https://www.bilibili.com/video/BV1fca56DETs)（01:09:57）；
