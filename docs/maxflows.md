# フローアルゴリズム一覧

## Introduction

Combinatorial optimization -Theory and Algorithms- 6th ed. (組合せ最適化―理論とアルゴリズム) の 8 Network Flows を基に実装したものです.
バグや説明の誤りがありましたら [Issues](https://github.com/zaki-joho/zaki-pages/issues) で報告していただけると助かります.

verify 用プログラムは [こちら](https://github.com/zaki-joho/zaki-pages/tree/master/test) に置いています. verify には [AIZU ONLINE JUDGE](http://judge.u-aizu.ac.jp/onlinejudge/index.jsp) と
[online-judge-tools (oj)](https://github.com/online-judge-tools/oj/blob/master/docs/getting-started.ja.md) を利用させていただきました.

## Contents

最大流

- [Ford-Fulkerson](ford_fulkerson.md)
- [Edamonds-Karp](edmonds_karp.md)
- [Dinic](dinic.md)
- [Dinic + Karzanov](dinic_with_karzanov.md)
- [Fujishige](fujishige_maxflow.md)
- [push-relabel (Goldberg-Tarjan)](push_relabel.md)

最小費用流

- Todo

## Notations

各ページでは以下の記号を断りなく用いる場合があります

- $V$: 頂点集合
- $n$: 頂点数 (=&#140 $V$ &#140)
- $E$: 辺集合
- $m$: 辺数 (=&#140 $E$ &#140)
- $F$: 最大フローの大きさ
- $u$: 辺の容量

## 実装に関する注意事項

- `flow_t` については `int` もしくは `long long` を想定しています. 特に浮動小数点を使用した場合, プログラムが停止しない可能性があります.
