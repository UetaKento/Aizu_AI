/**
 * @file score_board.c
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief score board object implementation.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#include <string.h>
#include "score_board.h"


static char const* get_name(void const*);
static bool add_score(void*, char const*, double const*);
static double get_student_average(void const*, char const*);
static size_t get_student_rank(void const*, char const*);


ScoreBoard* construct_score_board(ScoreBoard* self, char const* name, size_t course_num)
{
    construct_object(&self->super, name);
    self->course_num          = course_num;
    self->student_num         = 0;
    self->get_name            = get_name;
    self->add_score           = add_score;
    self->get_student_average = get_student_average;
    self->get_student_rank    = get_student_rank;

    return self;
}


static char const* get_name(void const* self)
{
    return ((ScoreBoard const*)self)->super.get_name(self);
}


static bool add_score(void* self, char const* name, double const* scores)
{
    ScoreBoard* sb = (ScoreBoard*)self;
    if (sb->student_num == MAX_STUDENT_NUM) {
        return false;
    }

    strcpy(sb->student_names[sb->student_num], name);
    for (size_t i = 0; i < sb->course_num; i++) {
        sb->scores[sb->student_num][i] = scores[i];
    }

    sb->student_num++;

    return true;
}


static double get_student_average(void const* self, char const* name)
{
    ScoreBoard const* sb = (ScoreBoard const *)self;

    double ave = 0.0;
    for (size_t i = 0; i < sb->student_num; i++) {
        if (strcmp(sb->student_names[i], name) == 0) {
            for (size_t j = 0; j < sb->course_num; j++) {
                ave += sb->scores[i][j];
            }
            break;
        }
    }

    return (ave / (double)sb->course_num);
}


static size_t get_student_rank(void const* self, char const* name)
{
    ScoreBoard const* sb = (ScoreBoard const*)self;
    double ave = get_student_average(sb, name);
    size_t rank = 1;

    for (size_t i = 0; i < sb->student_num; i++) {
        if (ave < get_student_average(sb, sb->student_names[i])) {
            rank++;
        }
    }

    return rank;
}
