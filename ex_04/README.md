# EX_04

## Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.


## Summary of today's homework
In the program made for Ex03, add the functions for uniform cost
search, best first search, and A* algorithm.

Ex03を元に、均一コスト探索(教科書p22, 表2.4)、最良優先探索(教科書p24
2.4.1と教科書p25, 表2.6(注)ただし今回の課題では 節2.4.1で紹介されてい
るものを実装する)、A*アルゴリズム(教科書p27, 表2.8)を実装せよ。参照、
教科書 p.21~p.29

## prog_04.c

### main()
Initialize the graph, and conduct search based on DFS, BFS, uniform
cost search, best first search, and A* algorithm.

グラフを読み込み、DFS, BFS, 均一コスト探索、最良優先、A*アルゴリズムを実行する。

## prog_04.h
Definitions of structure related to the graph.

### vertex_state
```
enum vertex_state {
    VISITED,   //!< Take this value if the vertex is visited.
    UNVISITED, //!< Take this value if the vertex is unvisited.
    IN_QUEUE,  //!< Take this value if the vertex is in a queue.
};
```

### edge_state
```
enum edge_state {
    NOT_CONNECTED = 0, //!< Take this value if the edge does not exist.
    CONNECTED,         //!< Take this value if the edge exists.
};
```

### graph
- heuristic_values is the value of the heuristic function H(x).

  heuristic_values はヒューリスティック関数値 H(x) 教科書p24参照

```
struct graph {
    EdgeState adjacent_matrix[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE]; //!< Adjacent matrix.
    float edge_costs[MAX_VERTEX_SIZE][MAX_VERTEX_SIZE];          //!< Edge costs.
    float heuristic_values[MAX_VERTEX_SIZE];                     //!< Heuristic values.
    size_t vertex_count;                                         //!< The number of vertices in the graph.
    size_t vertex_starting_search;                               //!< Start vertex for searching.
    size_t vertex_end_search;                                    //!< End vertex for searching.
};
```

## func.c

### Explanation of variables

- eval_values: float[] 
    - This is the sum of the accumulated cost and the heursitic value H(x).

      初期ノードから現在のノードまでのコストと、現在ノードから目標ノー
      ドまでのヒューリスティック関数値H(x)の和。A*アルゴリズムで用いる。
      教科書の　p.26, (2.1)式を参照。

- accumulate_costs: float[]
    - Accumulated cost from the initial node to the current node.

      初期ノードから現在のノードまでのコストを保存していく。

- child_eval_value: float
    - This is the total cost of a child node. Used by A* algorithm to find a better solution during search.
    A*アルゴリズムで用いる。教科書の　p.26, (2.2)式を参照。

### enum search_type
Definition of the search type, to unify all search methods.
探索のタイプの定義.
```c
enum search_type {
    SEARCH_TYPE_BFS,
    SEARCH_TYPE_UNIFORM_COST,
    SEARCH_TYPE_BEST_FIRST,
    SEARCH_TYPE_ASTAR,
};
typedef enum search_type SearchType;
```

### base_bfs()

#### Summary 概要
This is a generalized search algorithm that may cover uniform cost
search, best first search, and A* algorithm. To select a method you
want, you can use `search_type` defined above.

均一コスト探索、最良優先探索、A*アルゴリズムは幅優先探索を拡張することで実装できる。
そのため、関数の中の処理を各アルゴリズム毎に切り替えながら実装する。
切り替えるために上記で定義した探索のタイプを用いる。

#### Process 処理
1. Select a search type
   探索のタイプの明示
2. Initialize the starting node, the search cost, and the evluation value.
   探索ノード、探索コスト、eval valueの初期化
3. Select an operation based on the search type
   探索のタイプにもとづいて、以下の処理を行う
    - For BFS: Same as last time
      BFSの場合: 前回のプログラムを参照
    
    - For uniform cost search: see Table 2.4 in p. 22 of the textbook.
      均一コスト探索の場合: 教科書p22, 表2.4参照
    
    - For best first search: see table 2.6 in p. 24 of the textbook.
      最良優先探索の場合: 教科書p24 節2.4.1と教科書p25, 表2.6
        - (注)ただし今回の課題では 節2.4.1で紹介されているものを実装する
    
    - For A* algorithm: see table 2.8 in p. 27 of the textbook.
      A*アルゴリズムの場合: 教科書p27,表2.8参照 

## s_queue.c
- Same as Ex_03

## s_queue.h
- Same as Ex_03