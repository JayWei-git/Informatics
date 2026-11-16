## The XV6 Kernel

[这样好__的个人空间-这样好__个人主页-哔哩哔哩视频](https://space.bilibili.com/673424721?spm_id_from=333.788.upinfo.head.click)

xv6(riscv)类unix操作系统；

多核心单物理内存，没有缓存，内存大小为128MB(硬编码)；

每个核心只有一个硬件线程；(?)

设备：

- **UART**（Universal Asynchronous Receiver/Transmitter，通用异步收发传输器）负责串行通信；
- DISK，一个磁盘设备， 用宿主机上的文件模拟；
- 每个核心都有自己的定时器中断；
- PLIC：
- CLINT：

Memory Management: 

Page Size = 4096 bytes = 4KB

空闲页表被组织为链表；

三级页表

内核页表映射所有物理页，被所有核心共享

Scheduler: 进程可用时间片大小相同；核心共享单个就绪队列(实际上是数组)，核心循环遍历这个数组直到碰见第一个就绪进程；



启动：QEMU模拟RISC-V处理器然后启动内核，QEMU直接将内核加载到物理内存的固定起始位置中，无BIOS；



param.h中硬编码了很多限制，进程数(数组实现的循环队列)，打开的文件数等等；



User Address Space：0到MAXVA，从0开始，首先是Data and Code，也就是ELF可执行文件被Loaded的地方，然后是保护页Guard Page，然后一个页大小的栈，然后Heap开始向高地址增长；

这就是XV6和实际Unix的区别，实际Unix栈应该在高地址向低地址生长；

虚拟地址空间VAS最高处有两个页面，用于陷阱和异常处理，用户模式下不可访问；最上面的Trampoline Page是包含Code的，Kernel Mode下可执行；下面的页Trap Frame，也就是中段帧，用于保存Registers(所有进程的Trap Frame的VA相同，但是被映射到不同的PP)

实际上XV6采用的是RISC-V的Sv39架构(VA为39位)，但是XV6只是用低38位，也就是MAXVA为0x40 0000 0000，VPS为0到0x3F FFFF FFFF，也就是256GB

 首先看main.c (见源代码)

types.h：就是一些typedef；

param.h：硬编码的一些限制，最多线程数，最多打开文件数，最多核心数等；

defs.h：将kernel目录下所有文件的所有函数的接口声明，最后有一条宏，\#define NELEM(x) (sizeof(x)/sizeof((x)[0]))，就是计算元素数量

Spin Locks：自旋锁，如果在检查锁和申请锁之间被中断、调度，那么可能多个线程持有锁，RISCV提供了一个AMOSWAP的原子操作用于避免

spinlock.h给出了struct spinlock的定义；

spinlock.c给出了Initlock等的函数

自旋锁不应该持有过长时间，因为别的进程申请锁时是处于自旋的；

自旋锁用途举例：比如用键盘打字时，一个中断处理程序会被唤醒，然后将键盘来的电信号转换为字符存储在一个共享缓冲区中，然后有需求的线程就会从这个缓冲区中读；

具体就是敲键盘会触发一个外部硬件中断，然后被trap捕获，禁用中断，获取锁，向input buffer中写入键盘敲击的内容，释放锁，恢复中断，返回被中断的指令处；需要从buffer读取内容的thread也是禁止中断，获取锁，读取，释放锁，开启中断(为什么要禁止中断，试想如果刚好获取锁时，外部硬件中断发生了，也想要获取自旋锁，而且是忙等待，而且中断处理程序不可被调度，那么死锁了)

不过还存在一个问题，acquire时disable中断，release时enable中断可以避免deadlock，看起来完美了，但是同时申请多个spinlock时呢？第一个release就会开启中断，就麻烦了；

所以做法是引入嵌套计数器noff，acquire时noff++，release时noff--，noff为0时才开启中断；具体到代码就是spinlock.c中的两个函数push_off和pop_off

但是还有问题，如果在第一个acquire一个锁之前中断就是关着的呢，然后release这个锁后也不希望中断被打开，所以不能简单地在noff为0时直接开启中断；具体见代码注释；

接下来介绍memory management，具体就是kalloc和kfree两个函数，前面说过页面大小为4096bytes也就是4KB，空闲页面被组织为链表FreeList，注意是隐式空闲链表；

首先看结构体kmem，就是一个自旋锁+空闲链表头指针；

然后是kinit和freerange函数，初始化锁并将从end到PHYSTOP的所有内存分割成页再添加到隐式链表中，end就是在kernel.ld中定义的，内核代码后第一个可用内存地址，PHYSTOP就是最大user可用地址，在memlayout.h中定义；PGROUNDUP和PGSIZE宏在riscv.h中定义，一个用于地址对齐，一个规定了页大小；kinit在main中被调用了；

然后就是kfree和kalloc，具体见代码注释

这里重点讲一下struct run，仅从目前来看，这个结构体没有存在意义，只需记录隐式链表起始地址，然后将每个空闲页首字节解释为下一个页面起始地址就行了，为何还设计了一个run结构体？

这是为了可拓展性和类型安全，可能后面添加功能时，空闲页面头部不止一个指向下一个页面的指针，还需要其他字段时，就可以在struct run里面添加；

值得一提的是，由于struct run只有一个指针成员，所有这个struct大小就是一个指针大小，64位下就是8字节，所以把这个struct和其它指针互相装换是没有问题的；就是相当于新增了一种类型的指针罢了；

还有一个值得注意的细节，kalloc函数是根据freelist指针是否为空来判断空闲链表是否为空，这就要求最后一个空页面的next必须为空；注意kfree中的将页面加入链表逻辑，由于是头插法，所以必须将freelist的值赋给待插入页面的next，所以只需保证最初freelist为空，那么最后一个页面的next指针必为空，如何保证freelist值为空呢？这就涉及链接与符号相关的知识了；

最后再说一句memset函数来自于string.c，也是一个相当基本的文件，应该开头就提一下这个文件；



下面介绍syscall

首先看user.h，里面有所有系统调用和库函数的用户程序可用接口；
