set grid
set output 'result.jpg'
set terminal jpeg size 1280,960

plot 'result.txt' index 0 with points pointtype 7 pointsize 2 linecolor 1 title 'weights', \
     'result.txt' index 1 with points pointtype 3 pointsize 1 linecolor 2, \
     'result.txt' index 2 with points pointtype 3 pointsize 1 linecolor 3, \
     'result.txt' index 3 with points pointtype 3 pointsize 1 linecolor 4, \
     'result.txt' index 4 with points pointtype 3 pointsize 1 linecolor 5, \
     'result.txt' index 5 with points pointtype 3 pointsize 1 linecolor 6
