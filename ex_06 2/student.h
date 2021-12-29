/**
 * @file student.h
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief student object header.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#ifndef STUDENT_H
#define STUDENT_H



#include <stddef.h>
#include "human.h"
#include "score_board.h"

enum scholarship {
    SCHOLARSHIP_FULL,
    SCHOLARSHIP_HARF,
    SCHOLARSHIP_NONE,
};
typedef enum scholarship Scholarship;

typedef size_t (*FuncGetRank)(void const*, ScoreBoard const*);
typedef double (*FuncGetAverageScore)(void const*, ScoreBoard const*);
typedef Scholarship (*FuncGetScholarship)(void const*, ScoreBoard const*);


struct student {
    Human super;
    FuncGetName get_name;
    FuncGetGender get_gender;
    FuncGetJob get_job;
    FuncGetRank get_rank;
    FuncGetScholarship get_scholarship;
    FuncGetAverageScore get_average_score;
};
typedef struct student Student;


extern Student* construct_student(Student*, char const*, Gender);



#endif
