# EX_05

#Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.


## Summary of today's exercise
Write a program for implementing a production rule system that can conduct forward reasoning.
The strategy for conflit resolution is LEX.
See sub-section 4.1.4 in p. 71 of the textbook.

ハンドアウトにある観測事実とルール(知識ベース)を用いて、前向き推論を行うプロダクションシステムのプログラムを作成せよ。
ただし、戦略はLEX戦略を用いる。
教科書p.71 節4.1.4を参照せよ。

## `prog_05.c`

### main()
Initialize the production system, and conduct forward reasoning.

プロダクションシステムの初期化(init_system())を行い、前向き推論(forward_inference())を実行する。

### init_system()
Initialize the system based on given parameters.

引数で与えられたプロダクションシステムを初期化する。
入力ファイルを読み込む。

## `prog_05.h`

### Actions that can be taken by the if-then rules
```c
enum action {
    ACTION_ADD,    //!< Add conclusion to working memory.
    ACTION_DELETE, //!< Delete conclusion from working memory.
};
typedef enum action Action;
```

### Observations
The observed data or facts.

観測事実。教科書p.68 表4.3、 表4.4 参照
```c
struct observation {
    size_t time_tag;        //!< Time when observation was operated.
    char fact[MAX_STR_LEN]; //!< Fact string.
};
typedef struct observation Observation;
```

### rule
The production rules (obtained from human experts)

プロダクションルール。教科書p.65 節4.1.1参照。教科書p.66 表4.1参照。教科書p.67 表4.2参照。
```c
struct rule {
    Action action;                                                //!< Type of action of this rule.
    bool is_used;                                                 //!< This is true if this rule was used to resolve conflict.
    char paddings[sizeof(unsigned int) - sizeof(bool)];           //!< Padding for member alignment.
    size_t condition_count;                                        //!< The number of conditions.
    size_t conclusion_count;                                       //!< The number of conclusion.
    size_t matched_observation_count;                              //!< The number of matched observations.
    Observation const* matched_observations[MAX_OBSERVATION_NUM]; //!< Observations which correspond to all conditions of this rule.
    char conclusions[MAX_CONCLUSION_NUM][MAX_STR_LEN];            //!< Conclusions of this rule.
    char conditions[MAX_CONDITION_NUM][MAX_STR_LEN];              //!< Conditions of this rule.
};
typedef struct rule Rule;
```


### production_system
Structure for defining the production system.
```c
struct production_system {
    size_t time_tag;                                 //!< Origin of time.
    size_t rule_count;                               //!< The number of rules in the knowledge base.
    size_t conflict_count;                           //!< The number of rule conflicts.
    size_t observation_count;                        //!< The number of observations.
    Rule* conflict_set[MAX_RULE_NUM];                //!< Confliction set.
    char target_name[MAX_STR_LEN];                   //!< Target name to inference.
    Rule knowledge_base[MAX_RULE_NUM];               //!< Knowledge base, it consists of rules.
    Observation working_memory[MAX_OBSERVATION_NUM]; //!< Working memory, it consists of observations and conclusions of rules.
};
typedef struct production_system ProductionSystem;
```

## `data_05.txt`
The format of the data file is show as follows.

dat_05.txtのフォーマットを以下に示す。
```
[R]: The number of rules in knowledge base.
[N]: The number of conditions.
[M]: The number of conclusions.
[X]: A condition.
[Y]: A conclusion.
[A]: An action for a rule.
[L]: The number of observations.
[P]: An observation.
[T]: The observed target name.

Input set:
    [R]
    [N_0] [M_0]
    [X_0] ..... [X_N_0]
    [Y_0] ..... [Y_M_0]
    [A_0]
    [N_1] [M_1]
    [X_0] ..... [X_N_1]
    [Y_0] ..... [Y_M_1]
    [A_1]
         .......
    [N_R] [M_R]
    [X_0] ..... [X_N_R]
    [Y_0] ..... [Y_M_R]
    [A_R]
    [L]
    [P_0] .....  [P_L]
    [T]
```

#### Example
```
17
1 1
has_body_hair
is_a_mammal
add_to
1 1
drink_milk
is_a_mammal
add_to
1 1
has_feather
is_a_bird
add_to
1 1
can_fly_lay_eggs
is_a_bird
add_to
3 1
is_bird body_size_is_large outlook_is_white
is_a_swan
add_to
3 1
is_bird body_size_is_small outlook_is_black
is_a_swallow
add_to
3 1
is_bird body_size_is_moderate outlook_is_black
is_a_crow
add_to
2 1
is_a_mammal eats_meat
is_a_carnivore
add_to
3 1
is_a_mammal has_sharp_teeth has_sharp_claws
is_a_carnivore
add_to
3 1
is_a_carnivore has_brown_body_color has_big_body
is_a_lion
add_to
3 1
is_a_carnivore has_brown_body_color has_medium_sized_body
is_a_fox
add_to
2 1
is_a_mammal has_hoofs
is_an_ungulate
add_to
2 1
is_an_ungulate has_an_even_number_of_toes
is_an_even-toed_ungulate
add_to
2 1
is_an_ungulate ruminates
is_an_even-toed_ungulate
add_to
2 1
is_an_ungulate has_an_odd_number_of_toes
is_an_odd-toed_ungulate
add_to
3 1
is_an_even-toed_ungulate has_brown_body_color has_black_spots
is_a_deer
add_to
3 1
is_an_odd-toed_ungulate has_white_body_color has_black_strings
is_a_zebra
add_to
5
has_body_hair has_sharp_teeth has_sharp_claws has_brown_body_color has_medium_sized_body
unknown_animal
```

## `func.c`
### forward_inferece()
Forward inference (or reasoning) based on the LEX strategy.

LEX戦略を用いた前向き推論を行う。

### pattern_matching()
Find the conflict set.
Match the observed facts with all rules.
A rule is added to the conflict set if all conditions are satisfied.

競合集合を見つける。
観測事実と各ルールの条件を比較していき、全ての条件がマッチした場合、そのルールを競合集合に入れる。

### resolve_conflict()
Conflict resolution based on LEX.

LEX戦略にもとづいて、競合集合を解決する。
#### LEX Strategy 1
- Add the rule to selected_rules[] if it is not used yet.
使われてないルールを選択し、selected_rules[]: Rule* に入れる。

- If the number of selected rules is 0, stop;
選択されたルールの数が0の場合、終了する。

- If there is only 1 rule selected, use this rule.
選択されたルールの数が1の場合、そのルールを採用する。

- Otherwise, use Strategy 2.
それ以外の場合、戦略2にすすむ。

#### LEX Strategy 2
- Sort selected_rule[] based on the time stamp.
タイムタグをもとに新しい順にselected_rules[]をソートする。

- If there is only one rule that matches the newly observed facts, use it.
最新の観測事実にマッチしたルールが1つだけの場合、そのルールを採用する。

- Otherwise, use Strategy 3.
そうでない場合(つまり、最新の観測事実にマッチしたルールが複数存在する場合)、その最新の観測事実にマッチしたルールだけを残し、それ以外のルールはselcted_rulesから削除(参照しないように)する。戦略3にすすむ。

#### LEX Strategy 3
- Sort selected_rules[] based on the number of conditions.
ルールの条件部の数が多い順にselected_rules[]をソートする。

- If there is only one rule that has the maximum number of condictions, use it.
条件部の数を最大数もつルールが1つだけの場合、そのルールを採用する。

- Otherwise, use Strategy 4
そうでない場合、条件部の数を最大数もつルールだけを残し、それ以外のルールはselected_rulesから削除(参照しないように)する。戦略4にすすむ。

#### LEX Strategy 4
- Get one rule from selected_rules[] randomly, and use it.
selcted_rules[]の中からランダムに1つ選び、そのルールを採用する。

### do_action()
Do something based on the action specified by the rule.
If action is add, add the new inference result to the working memory.
If action is delete, delete a specified fact (maybe a noise) from the working memory.

ruleのactionに従って、動作を行う。
actionがaddの場合、ワーキングメモリに推論結果を追加する。
actionがdeleteの場合、推論結果をワーキングメモリから削除する。


### print_working_memory()
Output the data in the working memory.
ワーキングメモリの要素を標準出力する。

### bubble_sort()
Function for sorting a given data list based a given criteron.

バブルソート。第三引数に関数とる。
引数に取った関数を基準にソートを行う。
「関数ポインタ」を使って実装されている。
