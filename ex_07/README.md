# EX_07 

## Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.

## Summary of this homework 今回の問題の概要
Make a program to prove the validity of a given Boolean formula using
the truth table.

与えられた論理式が妥当であるかどうかを真理値表で証明をするプログラムを作成せよ。


## data_07.txt

```
and(x,or(y,z))
or(and(P,Q),Q)
imp(and(P,Q),or(P,Q))
imp(and(not(P),Q),or(P,and(Q, P)))
imp(P, Q)
imp(P, P)
imp(and(imp(P, Q), P), Q)
and(A, and(B, C))
and(not(or(A, and(B, C))), D)
and(input1, or(input2, input3))
```

### Correspondance table 対応表

| Formulas            | Corresponding inputs      |
|---------------------|---------------------------|
| P -> P              | imp(P, P)                 |
| ((P -> Q) & P) -> Q | imp(and(imp(P, Q), P), Q) |


## prog_07.h
```c
enum func_signature {
    FUNC_NOT = 0,
    FUNC_AND,
    FUNC_OR,
    FUNC_IMP,
    FUNC_UNKNOWN,
};
typedef enum func_signature FuncSignature;
```

## prog_07.c
### remove_tokens()
```
"and(input1, or(input2, input3))"
-> "input1,input2,input3"
```

## func.c
Please see comments on the source code!