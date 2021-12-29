/**
 * @file score_board.h
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief score board object header.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#ifndef SCORE_BOARD_H
#define SCORE_BOARD_H



#include <stdbool.h>
#include <stddef.h>
#include "object.h"

enum {
    MAX_COURSE_NUM = 8,
    MAX_STUDENT_NUM = 32,
};

typedef bool (*FuncAddScore)(void*, char const*, double const*);
typedef double (*FuncGetStudentAverage)(void const*, char const*);
typedef size_t (*FuncGetStudentRank)(void const*, char const*);

struct score_board {
    Object super;
    size_t course_num;
    size_t student_num;
    char student_names[MAX_STUDENT_NUM][MAX_STR_LEN];
    double scores[MAX_STUDENT_NUM][MAX_COURSE_NUM];
    double ranks[MAX_COURSE_NUM];
    FuncGetName get_name;
    FuncAddScore add_score;
    FuncGetStudentAverage get_student_average;
    FuncGetStudentRank get_student_rank;
};
typedef struct score_board ScoreBoard;


extern ScoreBoard* construct_score_board(ScoreBoard*, char const*, size_t);



#endif
