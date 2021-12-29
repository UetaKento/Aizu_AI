# EX_06

# Task
The answer should be submitted BEFORE the next exercise class. If you cannot finish by the deadline, please get a permission of extension from the TA or SA of your class.

Before trying to make your own programs, please try to read the source codes and try to understand. Please pay attention to the comments because they contain useful hints for solving the problem. Do not forget (MUST!) to write a brief summary in the file summary_XX.txt.

## 今回の問題の概要
フレームを実装せよ。
教科書p.81 4.2.2を参照せよ。
ハンドアウトp.18を参照せよ。

### `summary_06.txt`
Create input data by yourself and append the input data into `data_06.txt`.  
Expected outputs of the input must be appended to `answer_06.txt`.  
And compare actual outputs and your expectations after execution this program.  
Then, write following things into discussion section in `summary_06.txt`.  

- What you have done.
- The results of `average`, `rank` etc.
- The changes of `rank` of the students.


## data_06.txt
### フォーマット
```
[S]: The number of students.
[C]: The number of course.
[N]: Name of student.
[G]: Gender of student.
[P]: Point of a course.
[O]: any command string.
EOF means end of the file(input).

Input set:
[S] [C]
[N_0] [G_0] [P_0_0] ... [P_0_C]
     .......
[N_S] [G_S] [P_S_0] ... [P_S_C]
[O_0]
 ...
 ...
 ...
EOF

Command format:
[C]: command signature.
[T]: target
[A]: argument of command.

[C] [T] [A_1] ... [A_N]

Available command:
ask [target] [slot name].
conf [target] [slot name] [value].
```


## `prog_06.c`
### main()
1. データフレームにデータを代入する。
2. 各命令を読み込み、推論の実行をする。

## `func.h`
```c
struct student_info_frame {
    ScoreBoard score_board;             //!< This contains all score information of students in this frame.
    Student students[MAX_STUDENT_NUM];  //!< Student struct array.
    size_t student_num;                 //!< The number of students.
};
typedef struct student_info_frame StudentInfoFrame;
```


## `func.c`
### execute_command()
指定されたコマンドを実行する関数。

### Command 
```c
enum command {
    COMMAND_ASK,
    COMMAND_CONF,
    COMMAND_UNKNOWN
};
typedef enum command Command;
```


### load_word()
参考
```
ex)                              
input                            
command_str: "ask Yamada gender" 
cammand_name: not initialized              
->                              
output                           
command_str: "Yamada gender"     
command_name: "ask"              
```

### identify_command()
文字列からconfかaskかそれ以外か(unkonwn)を判定する。

### find_student()
- param: ``` StudentInfoFrame const* s_frame, char const* name```
- return: ```Student```

今回の問題。フレームの中から、引数である学生のnameにマッチしたStudentを返す。

1. TODO1: フレームの中から生徒の情報を探すためにループを回す。
2. TODO2: 探している名前とフレームに格納されている名前を比べていく。
3. TODO3: ヒント:返り値のtargetが後のexecute_command()でどのように使われているのか確認してください。

### command_ask()
ask コマンド。
質問とフレームをもとに、スロットの内容と比較し解答を返す。

### command_conf()
confirm コマンド。yesかnoで答える。

## その他のファイル
- object.h / object.c
- human.h / human.c
- student.h /student.c
- score_board.h /score_bord.c

### frame

| Object    |      |
|-----------|------|
| attribute | name |


| Human     |        |
|-----------|--------|
| is-a      | Object |
| attribute | gender |
| attribute | job    |


| Student   |                             |
|-----------|-----------------------------|
| is-a      | Human                       |
| procedure | get_rank(score_bord)        |
| procedure | get_score(score_bord)       |
| procedure | get_scholarship(score_bord) |


| Scorebord |                                   |
|-----------|-----------------------------------|
| is-a      | object                            |
| attribute | cource_num                        |
| attribute | student_num                       |
| attribute | student_names                     |
| attribute | scores                            |
| attribute | ranks                             |
| procedure | get_student_average(student_name) |
| procedure | get_student_rank(stduent_name)    |


| Badboy    |                             |
|-----------|-----------------------------|
| is-a      | Student                     |
| name      | Badboy                      |
| gender    | MALE                        |
| job       | student                     |
| procedure | get_rank(score_bord)        |
| procedure | get_score(score_bord)       |
| procedure | get_scholarship(score_bord) |



