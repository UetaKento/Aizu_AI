/**
 * @file func.h
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Command execution header.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#ifndef FUNC_H
#define FUNC_H



#include <stddef.h>
#include "score_board.h"
#include "student.h"

/**
 * @struct student_info_frame
 * @brief This struct defines a student information frame.
 */
struct student_info_frame {
    ScoreBoard score_board;             //!< This contains all score information of students in this frame.
    Student students[MAX_STUDENT_NUM];  //!< Student struct array.
    size_t student_num;                 //!< The number of students.
};
typedef struct student_info_frame StudentInfoFrame;


int execute_command(StudentInfoFrame const*, char const*);



#endif
