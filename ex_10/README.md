# EX_10

## Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.


## Summary of this homework 今回の問題の概要
Implement and understand the k-means algorithm.
Understand n-fold cross validation.

k近傍法の実装と理解
n-fold cross validationの理解

## TODO
TODO: 1
Find the nearest reference pattern (prototype) for each datum.
Euclidean distance is used. You may use the function defined for finding the Euclidean.

それぞれのデータのもっとも近い参照パターンをユークリッド距離を使用して計算する。
ユークリッド距離計算用の関数が定義されてので、それを使用してください。

TODO :2
Update the predicted label of the feature pattern if a more similar reference pattern is found.

それぞれのデータポイントに対して、今まで予測されていた参照パターンよりも近いものが見つかった場合、予測ラベルを更新する。

TODO: 3
Complete the function for finding the Euclidean distance.

ユークリッド距離を求めるアルゴリズムを描いてください。
教科書 p.113 (6.4)式参照。
引数の二つのベクトルにはそれぞれのデータの特徴ベクトルが入ります。

TODO: 4
Count the number of valid results (see if the predicted label equals to the true label or not).

k-meansで予測されたポイントのラベルと、正解データを比べて、正解率を計算する。

## data.txt
```
[S]: Number of classes (or labels) ラベルの要素数、アヤメの種類
[R]: Number of data データ数
[F]: Number features for each datum (dimension) 各データの特徴数。次元数
[D]: The feature vector 各データの特徴ベクトル
[L]: Labels of the data アヤメの種類のラベル
[O]: any command string.
EOF means end of the file(input).

Input set:
[S] [R] [F]
[D_1_1] [D_1_2] [D_1_3] [D_1_4] [L_1]
     .......
[D_150_1] [D_150_2] [D_150_3] [D_150_4] [L_150]
EOF
```

## `func.c`
### do_kmeans()
Classify a given data set using the k-means algorithm.  
与えられたデータをk-meansアルゴリズムを使って分割する。

### classify_vectors()
Classify the vectors in the given dataset using the reference patterns.

参照されたパターンから与えられたデータセットを分割する。
それぞれのデータごとに、どのパターンにもっとも似ているのかをユークリッド距離を使用して判断する。

### update_reference_pattern()
Update all reference patterns using the centroids.  
参照されるパターンをセントロイドを使用して更新する。

### euclidean_distance()
Find the Euclidiean distance.  
ユークリッド距離を計算する

### adjust_labels()
Adjust labels using a confusion matrix.  
k-meansが教師なし学習のため、データセットのラベルと予測したラベルが一致するは限らないため、
予測されたラベルのままでは正解率を正常に出すことはできない。
そのため、confusion matrixを使用して正常に正解率を出せるようにラベルの付け替えを行う。

### validate()
Validate the results classified by k-means algorithm.  
k-meansで予測されたポイントのラベルと、正解データを比べて、正解率を計算する。

### rand_xorshift()
Generate a random number.  
ランダムな数値の作成
