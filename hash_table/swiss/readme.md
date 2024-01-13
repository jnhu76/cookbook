# SwissMap

SwissMap: A smaller, faster Golang Hash Table

It uses AES instructions for fast-hashing and performs key lookups in parallel using SSE instructions. Because of these optimizations, SwissMap is faster and more memory efficient than Golang's built-in map. 

原文：https://www.dolthub.com/blog/2023-03-28-swiss-map/

Git地址：https://github.com/dolthub/swiss

## Swisstable

1. https://github.com/abseil/abseil-cpp
2. https://zhuanlan.zhihu.com/p/277732297
3. https://github.com/taviso/swisstable

https://martin.ankerl.com/2022/08/27/hashmap-bench-01/#stable-references
https://github.com/taviso/swisstable/blob/master/benchmark.c