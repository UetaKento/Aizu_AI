## EX_11

## Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.


## Summary of this homework  今回の問題の概要
Implement a self-organizing neural network. 
Explanations can be found in p. 132 of the textbook.

自己組織ニューラルネット(sonnet)を実装せよ。
sonnetについては教科書p.132 6.4.2 を参照せよ

## data.txt
- NC: Number of neurons.                 sonnetのニューロンの数 (プロトタイプの数)
- L:  Number of learning cycles.         学習回数の上限
- R:  Learning rate.                     学習率
- VC: Number of data (feature vectors).  データ(特徴ベクトル)の数
- FD: Dimension of the feature space.    特徴ベクトルの次元数

```
[NC] [L] [R] 
[VC] [FD]
[x_1_1] [x_1_2]
[x_2_1] [x_2_2]
...
...
[x_VC_1] [x_VC_2]
```
## Todo 
Fill several places to complete the function train_sonnet(). 
sonnetを学習する関数train_sonnet()を実装する。

Todo1:
Generate the weights (of the prototypes) randomly.
重み(プロトタイプ)を乱数で初期化する。

Todo2:
Find the Euclidean distance between a given pattern x and a prototype.
パターンxとを重み(プロトタイプ)のユークリッド距離を計算する。

Todo3:
Update the weights of a prototype.
重み(プロトタイプ)を更新する。