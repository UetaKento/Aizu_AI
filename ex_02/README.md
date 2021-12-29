# EX_02

## Task
Please submit the answer of each exercise before the next exercise class.
ANY submission after that will NOT be counted.

Read source codes and try to understand.
Also, Pay attention to comments because of hints for solving task.

Do not forget (MUST!) to write a brief summary in the file summary_XX.txt.

## 今回の問題の概要
幅優先探索(BFS)と深さ優先探索(DFS)を用いて、指定した頂点から、全ての頂点を訪問し、訪問した順番で頂点の番号を出力するプログラムを作成する。
`func.c`のプログラムが未完成である。コメントに従って正しく動作するようにせよ。
教科書p 18 〜 p 20を参照せよ。

## 変更すべきファイル

* `func.c` - コメントになっているところを実装してください。
* `summary_XX.txt` (XXは演習の番号) - 簡単な要約を書いてください（日本語可）。

## 実行と確認方法

以下のコマンドで、コンパイルと実行と正しく実装できているを確認することができます（gccも自動でしてくれます）。

```sh
$ make test
```

以下のように`diff ...`の後に何も出力が出ていなければ正しく実装できていることになります。

```
$ make test
diff -y --suppress-common-lines answer_XX.txt result_XX.txt
```

(XXは演習の番号)

## prog_02.c
- inputファイルを読み込みグラフを初期化する。(`bool init_garph()`)
- 読み込んだグラフを用いて、`dfs()`, `bfs()`を実行する。(`dfs()`, `bfs()`は`func.c`で定義されている)

## prog_02.h
グラフ構造の定義

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
```c
struct graph {
    EdgeState adjacent_matrix[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE]; //!< The adjacent matrix.
    size_t vertex_count;                                         //!< The number of vertices.
    size_t vertex_starting_search;                               //!< The starting vertex for search.
};
``` 

## func.c
このファイルのプログラムの穴埋めを行なう。

### dfs()
```
1. 各ノードの初期化。各ノードをunvisited状態にしておく。
2. `dfs_visit()`で探索を行う。
```

### dfs_visit()
open listの処理は再帰を使って(FILO)実装している。

### bfs()

```
1. 各ノードの初期化。各ノードをunvisited状態にしておく。
2. スタート位置の設定、スタート位置のノードの状態をvisitedに変更。スタート位置のノードをopen listに追加。
3. open listが空でない場合、4へ。空の場合、終了。
4. open listから一つ取り出し、現在の位置のノードとする。現在の位置のノードに隣接しているノードの中で、unvisited状態のノードをvisit状態に変更して、open listに入れる。3へ。
```
open listはqueue(FIFO)で実装してある。

## s_queue.c
BFSに用いるsortable queueの実装。

### void s_queue_enqueue(size_t element)
- 要素をキューに追加する。

### size_t s_queue_dequeue(void)
- 要素をキューから取り出す。

## s_queue.h
- queueに使う関数の宣言。
