# EX_09

## Task
Please submit the answer of each exercise before the exercise class of
next week.  Submissions after the deadline will NOT be counted.

Before starting, read the source codes carefully, and try to
understand.  Do not forget (MUST!) to write a brief summary in the
file summary_XX.txt. You may use English or Japanese.

Many comments are included in the `func.c` program.

## Summary of this homework 今回の問題の概要
Write a program for car break control and plot the results.
Also, discuss the relations between the velocity (v), the radius of the curve (r), and the break strength (b).
You may use `gnuplot` to generate the 3-D graph.  
ハンドアウト中にある自動車のブレーキコントロールの例をプログラムで書き、結果をプロットせよ。
また、速度(v), カーブの角度(r)とブレーキ量(b)の関係を議論せよ。gnuplotなどのグラフ生成ツールを用いて、3次元グラフを生成せよ。

## print_car_speed_control_fuzzy()
Find the break strength b based on v and r in the following region:
- velocity: double 20.0 <= v <= 80.0
- radius: double 75.0 <= r <= 225.0  
の範囲のvとrで減速量を計算する。(ハンドアウトp16または93ページの(5.9)式)


## gnuplot

Example:

```sh
$ gnuplot
gnuplot> set term png size 960, 720　# change image size
Terminal type set to 'png'
Options are 'nocrop enhanced size 960,720 font "arial,12" '
gnuplot> set output 'plot.png'  # set output file name
gnuplot> set xlabel 'velocity'
gnuplot> set ylabel 'radius'
gnuplot> set zlabel 'deceleration'
gnuplot> set palette rgbformulae 23, 28, 3 # graph color (Not necessary)
gnuplot> set view 60, 110, 1, 1　# change view point (Not necessary)
gnuplot> splot 'result_09.txt' with pm3d # plot
```
