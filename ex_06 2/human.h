/**
 * @file human.h
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief human object header.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#ifndef HUMAN_H
#define HUMAN_H



#include "object.h"

enum gender {
    GENDER_MALE,
    GENDER_FEMALE,
};
typedef enum gender Gender;

enum job {
    JOB_NONE,
    JOB_STUDENT,
};
typedef enum job Job;


typedef Gender (*FuncGetGender)(void const*);
typedef Job (*FuncGetJob)(void const*);


struct human {
    Object super;
    Gender gender;
    Job job;
    FuncGetName get_name;
    FuncGetGender get_gender;
    FuncGetJob get_job;
};
typedef struct human Human;


extern Human* construct_human(Human*, char const*, Gender, Job);



#endif
