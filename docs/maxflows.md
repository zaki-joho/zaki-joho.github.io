# フローアルゴリズム一覧

## Introduction

Combinatorial optimization -Theory and Algorithms- 6th ed. (組合せ最適化―理論とアルゴリズム) の 8 Network Flows を基に実装したものです.
バグや説明の誤りがありましたら [Issues](https://github.com/zaki-joho/zaki-pages/issues) で報告していただけると助かります.

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


- $n$: 頂点数
- $m$: 辺数
- $F$: 最大フローの大きさ
- $u_{max}$: 辺の容量の最大値
