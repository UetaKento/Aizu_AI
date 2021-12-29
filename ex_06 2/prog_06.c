/**
 * @file prog_06.c
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Student information frame program.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 *
 * Assignment:
 *  Fill in the all blanks for completing the program.
 *  Next, Create input data by yourself and append the input data into `data_06.txt`.
 *  Expected outputs of the input must be appended to `answer_06.txt`.
 *  And compare actual outputs and your expectations after execution this program.
 *  Then, write following things into discussion section in `summary_06.txt`.
 *      - What you have done.
 *      - The results of `average`, `rank` etc.
 *      - The changes of `rank` of the students (If changes exist, show the ranks before changes and after changes).
 *
 *  [S]: The number of students.
 *  [C]: The number of course.
 *  [N]: Name of student.
 *  [G]: Gender of student.
 *  [P]: Point of a course.
 *  [O]: any command string.
 *  EOF means end of the file(input).
 *
 *  Input set:
 *  [S] [C]
 *  [N_0] [G_0] [P_0_0] ... [P_0_C]
 *       .......
 *  [N_S] [G_S] [P_S_0] ... [P_S_C]
 *  [O_0]
 *   ...
 *   ...
 *   ...
 *  EOF
 *
 *  Command format:
 *  [C]: command signature.
 *  [T]: target
 *  [A]: argument of command.
 *
 *  [C] [T] [A_1] ... [A_N]
 *
 *  Available command:
 *  ask [target] [slot name].
 *  conf [target] [slot name] [value].
 *
 *  example.
 *      ask Yamada gender
 *      conf Yamada rank 10
 */

#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void init_frame(StudentInfoFrame*);


int main(void)
{
    StudentInfoFrame s_frame = {
        .student_num = 0,
    };

    init_frame(&s_frame);

    while (1) {
        // Read command string from the input.
        char command_str[MAX_STR_LEN] = {'\0'};
        char* p = fgets(command_str, sizeof(command_str), stdin);
        if (p == NULL) {
            break;
        }

        // Remove \n.
        p = strchr(command_str, '\n');
        if (p != NULL) {
            *p = '\0';
        }

        if (strlen(command_str) == 0) {
            // Skip empty string.
            continue;
        }

        // Execute command.
        printf("%s\n-> ", command_str);
        int result_code = execute_command(&s_frame, command_str);
        if (result_code == EXIT_FAILURE) {
            puts("-> Command execution failure.");
        }
    }

    return 0;
}


/**
 * @brief Initialize student information frame from input.
 * @param s_frame The pointer to the given student information frame.
 */
static void init_frame(StudentInfoFrame* s_frame)
{
    size_t course_num;
    scanf("%zu %zu\n", &s_frame->student_num, &course_num);

    // Initialize score board.
    construct_score_board(&s_frame->score_board, "exam", course_num);

    for (size_t i = 0; i < s_frame->student_num; i++) {
        // Read and set student information.
        char name[MAX_STR_LEN];
        char gender[7];
        scanf("%s %s ", name, gender);

        Gender g = (strcmp(gender, "MALE") == 0) ? (GENDER_MALE) : (GENDER_FEMALE);
        construct_student(&s_frame->students[i], name, g);

        // Add scores.
        double scores[MAX_COURSE_NUM];
        for (size_t j = 0; j < course_num; j++) {
            scanf("%lf", &scores[j]);
        }
        scanf("\n");
        s_frame->score_board.add_score(&s_frame->score_board, name, scores);
    }
}
