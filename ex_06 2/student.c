/**
 * @file student.c
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief student object implementation.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#include "student.h"

static char const* get_name(void const*);
static Gender get_gender(void const*);
static Job get_job(void const*);
static size_t get_rank(void const*, ScoreBoard const*);
static Scholarship get_scholarship(void const*, ScoreBoard const*);
static double get_average_score(void const*, ScoreBoard const*);


Student* construct_student(Student* self, char const* name, Gender g)
{
    construct_human(&self->super, name, g, JOB_STUDENT);
    self->get_name          = get_name;
    self->get_gender        = get_gender;
    self->get_job           = get_job;
    self->get_rank          = get_rank;
    self->get_scholarship   = get_scholarship;
    self->get_average_score = get_average_score;

    return self;
}


static char const* get_name(void const* self)
{
    return ((Student const*)self)->super.get_name(self);
}


static Gender get_gender(void const* self)
{
    return ((Student const*)self)->super.get_gender(self);
}


static Job get_job(void const* self)
{
    return ((Student const*)self)->super.get_job(self);
}


static size_t get_rank(void const* self, ScoreBoard const* sb)
{
    return sb->get_student_rank(sb, ((Student const*)self)->get_name(self));
}


static Scholarship get_scholarship(void const* self, ScoreBoard const* sb)
{
    size_t rank = ((Student const*)self)->get_rank(self, sb);
    return (rank <= 5) ? (SCHOLARSHIP_FULL) : ((10 <= rank) ? (SCHOLARSHIP_NONE) : (SCHOLARSHIP_HARF));
}


static double get_average_score(void const* self, ScoreBoard const* sb)
{
    return sb->get_student_average(sb, ((Student const*)self)->get_name(self));
}
