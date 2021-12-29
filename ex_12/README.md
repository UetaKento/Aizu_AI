# EX_12

## Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.

## Summary of this homework  今回の問題の概要
- Try to implement and understand multilayer neural networks (MLP)
  階層型ニューラルネットワークの実装と理解。
- Train an MLP using the BP algorithm.
  MLPをBack propagationを使って学習させるプログラムを完成させよ。
- Here, we fix the number of layers to 3.
  ただし、簡単のため3層(隠れ層は1層)のMLPとする。
- Since the outputs of the network are real numbers in [0,1], we use uni-polar sigmoid function.
  また、このMLPの出力は0か1であるためUnipolar sigmoid関数(ただし、lambda = 1)を使用することに注意せよ。


## Explanation 今回の問題の説明
1. Todo1: Implement the unipolar sigmoid function (lambda = 1).
    - シグモイド関数の実装
2. Todo2: Calculate the inner product. 
    - 内積の計算をするプログラムの記述

3. Todo3: Calculate the error of the output layer for backpropagation.
   Note that the activation function in this program is `the unipolar sigmoid function`. 
    - Add the code for finding the error.
      error(誤差)を求めるコードの記述。
    - Refer to p. 145 in the textbook.
      教科書 p.145を参照

4. Todo4: Update the weights based on the BP algorithm. See p. 145 in the textbook.
    backpropagation処理での重みの更新。教科書 p.145を参照


## `prog.c`
- `main()` function is here.
1. Write unipolar sigmoid function (lambda = 1).
2. Calculate the inner product.
3. Calculate the error of the output layer for backpropagation.
4. Update weight based on backpropagation.

## `prog.h`
### Explanation of structures  構造体の説明

#### feature_vector 
- This struct define a feature vector
- 学習、テストに使うデータの構造の定義

#### dataset
- This struct defines a dataset for training and test.
- 学習、テストに使用するデータの保管

```c
struct dataset {
    size_t vector_count;      //!< The number of feature vector and the number of teacher vectors, the two numbers are equal.
    size_t feature_dimension; //!< The dimension of a feature vector.
    size_t teacher_dimension; //!< The dimension of a teacher vector.
    FeatureVector* features;  //!< Array of feature vectors.
    TeacherVector* teachers;  //!< Array of teacher vectors.
};
```

#### mlp3
- This struct defines a three layer perceptron setting.
- この構造体内でmlpの設定を決定している。
```c
struct mlp3 {
    size_t input_neuron_count;  //!< The number of input neurons.
    size_t output_neuron_count; //!< The number of output neurons.
    size_t hidden_neuron_count; //!< The number of hidden neurons.
    double learning_rate;       //!< The learning rate of the MLP.
    double desired_error;       //!< The criterion for the terminating learning iteration.
    double** hidden_weights;    //!< Array of weights between input layer and hidden layer.
    double** output_weights;    //!< Array of weights between hidden layer and output layer.
    double* hidden_outputs;     //!< Array of outputs of hidden layer.
    double* outputs;            //!< Array of outputs of output layer.
    double* hidden_errors;      //!< Array of errors of hidden layer for backpropagation.
    double* output_errors;      //!< Array of errors of output layer for backpropagation.
};
```

## `func.c`
- Explanation of important functions 特に重要な関数を説明する。

### `void train_mlp(Mlp3* mlp, Dataset const* dataset)`
- function for training an MLP
  mlpの学習を行う
1. Pick-up a feature vector and its teacher signal.
   特徴ベクトルとその教師信号をデータセットから1つずつ取り出す。
2. Find the output for the MLP for the feature vector.
   1.で取り出した特徴ベクトルを用いてMLPの出力を求める。
3. Calculate the error.
   誤差を計算する。(``compute_erros()``)
4. Update the weights of the MLP.
   誤差に基づいて、MLPの重み(``mlp->hidden_weights``, ``mlp->output_weights``)を更新する。(``update_weghts()``)
5. Repeat 1-4 for all data.
   1~4を全てのデータに対して行う。
6. Repeat 1-5 until the error is smaller than a given value.
   1~5を誤差が許容誤差を下回るまで繰り返す。

### `double compute_errors(Mlp3* mlp, TeacherVector const* teacher_vector)`
- Calculate the errors (see p. 145 in the textbook).
  全ての誤差の計算をする。出力層、隠れ（中間）層(教科書 p.145参照)

### `static void update_weights(double** weights, size_t neuron_count, double const* inputs, size_t input_dimension, double const* errors, double learning_rate)`
- Update the weights (see p. 145 in the textbook). 
  重みの更新を行う。教科書 p.145参照