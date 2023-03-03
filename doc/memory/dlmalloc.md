# Doug Lea Malloc
dlmalloc 是 Doug Lea 为程序提供的一个用户态的通用内存分配算法，曾经亦为 glibc 中的默认内存分配算法。dlmalloc 的主要重点如下：
- 算法核心：边界标记法以及分箱式内存管理
- malloc 与 free 的实现
- dlmalloc 的局限性及参考意义
- dlmalloc 的编程技巧

## 0. 核心算法

### 0.0 边界标记法
### 0.1 分箱式内存管理

## 1. malloc 与 free 的实现
### 1.0 dlmalloc
### 1.1 dlfree

## 2. dlmalloc 的局限性及参考意义

## 3. dlmalloc 的编程技巧
### 3.1 更好地节约内存
在 dlmalloc 中，小内存的箱子是用 gm 中的 smallbins 进行管理的，它本质是一个 malloc_chunk 的指针数组，如下所示：
```c
typedef struct malloc_chunk* mchunkptr;

struct malloc_chunk {
  size_t               prev_foot;  /* Size of previous chunk (if free).  */
  size_t               head;       /* Size and inuse bits. */
  struct malloc_chunk* fd;         /* double links -- used only if free. */
  struct malloc_chunk* bk;
};

#define NSMALLBINS        (32U)

mchunkptr  smallbins[(NSMALLBINS+1)*2];
```
smallbins 总共有 32 个箱子，但是数组的个数实际却为 `(NSMALLBINS+1)*2 = 66` 个，这里 Doug Lea 使用了一个小技巧去节约内存的使用：
从 `malloc_chunk` 的定义中我们可以得知，在 64 位操作系统下它的结构体大小为 32 bytes。因此一个含有 32 个箱子的 smallbins 需要占用 `32 * 32 = 1024` 字节的内存空间。但是需要注意的是，对于每个箱子的头节点来说，它们只有字段 `fd` 和 `bk` 是有用的，`prev_foot` 和 `head` 完全不需要使用到，有效内存空间仅为 512 bytes，白白地造成一半的内存空间浪费。
因此 Doug Lea 在这里使用了一个小技巧，它创建了一个大小为 66 的指针类型数组，所占内存空间为：`66 * 8 = 528` bytes，528 > 512。而对于无效的字段，直接将它们覆盖掉即可，如此便可以达到节约内存的目的。访问某个索引的箱子代码如下所示：
```c
#define smallbin_at(M, i)   ((sbinptr)((char*)&((M)->smallbins[(i)<<1])))
```
可以看到访问第 i 个索引的箱子的时候，访问的是 i * 2 的数组下标成员，刚好就是对应箱子的头节点 chunk，内存分布图如下：
![](image/smallbins%E5%86%85%E5%AD%98%E5%88%86%E5%B8%83.png)

