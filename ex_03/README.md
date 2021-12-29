# EX_03

## Task
The answer should be submitted BEFORE the next exercise class.  
If you cannot finish by the deadline,
please get a permission of extension from the TA or SA of your class. 

Before trying to make your own programs,
please try to read the source codes and try to understand.  
Please pay attention to the comments
because they contain useful hints for solving the problem.
Do not forget (MUST!) to write a brief summary in the file summary_XX.txt.

## 今回の問題の概要
- Ex02を元に、指定した頂点と指定した終点間の経路を幅優先探索と深さ優先探索を行うように変更せよ。また、それぞれの辺に重みを与え、合計コストも計算するようにせよ。
- 教科書p18 -p20 参照せよ

## prog_03.c
- inputファイルを読み込みグラフを初期化する。(`bool init_garph()`)
- 読み込んだグラフを用いて、`dfs()`, `bfs()`を実行する。(`dfs()`, `bfs()`は`func.c`で定義されている)

## prog_03.h
### vertex_state
```c
enum vertex_state {
    VISITED,   //!< Take this value if the vertex is visited.
    UNVISITED, //!< Take this value if the vertex is unvisited.
};
typedef enum vertex_state VertexState;
```

### edge_state
```c
enum edge_state {
    NOT_CONNECTED = 0, //!< Take this value if the edge does not exist.
    CONNECTED,         //!< Take this value if the edge exists.
};
``` 

### graph
- EX_02 と比べて、edge_costs, vertex_end_search が加わった。

```c
struct graph {
    EdgeState adjacent_matrix[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE]; //!< The adjacent matrix.
    float edge_costs[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE];          //!< The edge costs.
    size_t vertex_count;                                         //!< The number of vertices.
    size_t vertex_starting_search;                               //!< The starting vertex for search.
    size_t vertex_end_search;                                    //!< The end vertex for search.
};
``` 


## func.c
### dfs()
```
1. 各ノードの初期化。各ノードをunvisited状態にしておく。
2. `dfs_visit()`で探索を行いコストを計算する。今回はコストの初期値が必要になる。
```

### dfs_visit()
open listの処理は再帰を使って(FILO)実装している。
今回はcostを返り値として返す。


### bfs()

```
1. 各ノードの初期化。各ノードをunvisited状態にしておく。
2. スタート位置の設定、スタート位置のノードの状態をvisitedに変更。スタート位置のノードをopen listに追加。
3. open listが空でない場合、4へ。空の場合、終了。
4. open listから一つ取り出し、現在の位置のノードとする。現在の位置のノードに隣接しているノードの中で、unvisited状態のノードをvisit状態に変更して、open listに入れる。コストの計算を行う。3へ。
```
open listはqueue(FIFO)で実装してある。

## s_queue.c
第二回と同じ。

## s_queue.h
第二回と同じ。