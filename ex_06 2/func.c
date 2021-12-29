/**
 * @file func.c
 * @date 2016/10/21
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Command execution implementation.
 * @details
 *  Artificial Intelligence, 6th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "score_board.h"
#include "student.h"
#include "func.h"


/**
 * @enum command
 * @brief command signatures.
 */
enum command {
    COMMAND_ASK,
    COMMAND_CONF,
    COMMAND_UNKNOWN
};
typedef enum command Command;


typedef int (*FuncCommand)(StudentInfoFrame const*, Student const*, char const*, char*);

static Student const* find_student(StudentInfoFrame const* s_frame, char const* name);
static Command identify_command(char const*);
static int command_ask(StudentInfoFrame const*, Student const*, char const*, char*);
static int command_conf(StudentInfoFrame const*, Student const*, char const*, char*);
static char const* load_word(char const*, char*);
static char* load_slot_value_as_str(StudentInfoFrame const*, Student const*, char const*, char*);


/**
 * @brief Execute command.
 * @param s_frame The pointer to the given student information frame.
 * @param command_str The pointer to the given command string.
 * @return If command success, this returns EXIT_SUCCESS.
 *         Otherwise, this returns EXIT_FAILURE.
 */
int execute_command(StudentInfoFrame const* s_frame, char const* command_str)
{
    // Read command name.
    char command_name[16];
    command_str = load_word(command_str, command_name);
    if (command_str == NULL) {
        return EXIT_FAILURE;
    }

    // Get command signature.
    Command command = identify_command(command_name);
    if (command == COMMAND_UNKNOWN) {
        printf("Detected unknown command: %s", command_name);
        return EXIT_FAILURE;
    }

    // Read target name.
    char target_name[MAX_STR_LEN];
    command_str = load_word(command_str, target_name);
    if (command_str == NULL) {
        return EXIT_FAILURE;
    }

    // Find target student.
    Student const* target = find_student(s_frame, target_name);
    if (target == NULL) {
        printf("Student %s is NOT found.\n", target_name);
        return EXIT_FAILURE;
    }

    static FuncCommand command_func_map[] = {
        [COMMAND_ASK]  = command_ask,
        [COMMAND_CONF] = command_conf
    };

    // Execute command function.
    char result_str[MAX_STR_LEN];
    int result_code = (command_func_map[command])(s_frame, target, command_str, result_str);
    if (result_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    // Print command result.
    printf("%s\n", result_str);

    return EXIT_SUCCESS;
}


/**
 * @brief Find student by the name.
 * @param s_frame The pointer to the given student information frame.
 * @param name Target student name.
 * @return If the student was found, this returns the pointer to student.
 *         otherwise, this returns NULL.
 */
static Student const* find_student(StudentInfoFrame const* s_frame, char const* name)
{
    Student const* target = NULL;
    // TODO: 1. Fill in for statement to iterate each student.
    for (/* ------------------------------------------- */) {
        Student const* s = &s_frame->students[i];
        // TODO: 2. Fill in second argument to compare `name` and student name.
        if (strcmp(name, /* -------------- */) == 0) {
            // TODO: 3. Write variable which satisfies condition.
            target = /* - */;
            break;
        }
    }

    return target;
}


/**
 * @brief Identify command from string.
 * @param command_name Command name string.
 * @return Command signature.
 */
static Command identify_command(char const* command_name)
{
    if (strcmp(command_name, "conf") == 0) {
        return COMMAND_CONF;
    } else if (strcmp(command_name, "ask") == 0) {
        return COMMAND_ASK;
    }

    return COMMAND_UNKNOWN;
}


/**
 * @brief Ask command.
 * @param s_frame The pointer to the given student information frame.
 * @param target The pointer to the given target student.
 * @param arguments_str String for arguments.
 * @param result String will be written into this variable.
 * @return If command success, this returns EXIT_SUCCESS.
 *         Otherwise, this returns EXIT_FAILURE.
 */
static int command_ask(StudentInfoFrame const* s_frame, Student const* target, char const* arguments_str, char* result)
{
    char const* tmp;

    // Read slot name.
    char slot_name[MAX_STR_LEN];
    tmp = load_word(arguments_str, slot_name);
    if (NULL == tmp) {
        return EXIT_FAILURE;
    }

    // Get slot value as string.
    tmp = load_slot_value_as_str(s_frame, target, slot_name, result);
    if (NULL == tmp) {
        sprintf(result, "No slot: %s", slot_name);
    }

    return EXIT_SUCCESS;
}


/**
 * @brief Confirm command.
 * @param s_frame The pointer to the given student information frame.
 * @param target The pointer to the given target student.
 * @param arguments_str String of arguments.
 * @param result string will be written into this variable.
 * @return If command success, this returns EXIT_SUCCESS.
 *         Otherwise, this returns EXIT_FAILURE.
 */
static int command_conf(StudentInfoFrame const* s_frame, Student const* target, char const* arguments_str, char* result)
{
    // Read slot name.
    char slot_name[MAX_STR_LEN];
    arguments_str = load_word(arguments_str, slot_name);
    if (NULL == arguments_str) {
        return EXIT_FAILURE;
    }

    // Get slot value as string.
    char slot_value_str[MAX_STR_LEN];
    char* tmp = load_slot_value_as_str(s_frame, target, slot_name, slot_value_str);
    if (NULL == tmp) {
        sprintf(result, "No slot: %s", slot_name);
    }

    // Read expected slot value.
    char expected_slot_value_str[MAX_STR_LEN];
    arguments_str = load_word(arguments_str, expected_slot_value_str);
    if (NULL == arguments_str) {
        return EXIT_FAILURE;
    }

    if (0 == strcmp(slot_value_str, expected_slot_value_str)) {
        strcpy(result, "You are right.");
    } else {
        strcpy(result, "You are wrong.");
    }

    return EXIT_SUCCESS;
}


/**
 * @brief load a word into argument variable.
 * @param str Source string.
 * @param dst Destination string.
 * @return Sum of str address and the size of word + 1.
 *         If loading is failed, this returns NULL.
 */
static char const* load_word(char const* str, char* dst)
{
    int n = sscanf(str, "%s", dst);
    return (n == EOF) ? (NULL) : (str + strlen(dst) + 1);
}


/**
 * @brief Load slot value into argument variable as string..
 * @param s_frame The pointer to the given student information frame.
 * @param target The pointer to the given target student.
 * @param slot_name The name of slot.
 * @param result String will be written into this variable.
 * @return If slot is not found, this returns NULL.
 *         Otherwise, this returns same value to result.
 */
static char* load_slot_value_as_str(StudentInfoFrame const* s_frame, Student const* target, char const* slot_name, char* result)
{
    if (0 == strcmp(slot_name, "gender")) {
        strcpy(result, ((target->get_gender(target) == GENDER_MALE) ? ("MALE") : ("FEMALE")));
    } else if (0 == strcmp(slot_name, "rank")) {
        sprintf(result, "%zd", target->get_rank(target, &s_frame->score_board));
    } else if (0 == strcmp(slot_name, "scholarship")) {
        switch (target->get_scholarship(target, &s_frame->score_board)) {
            case SCHOLARSHIP_FULL:
                strcpy(result, "Scholarship_Full");
                break;
            case SCHOLARSHIP_HARF:
                strcpy(result, "Scholarship_Half");
                break;
            case SCHOLARSHIP_NONE:
                strcpy(result, "Scholarship_None");
                break;
        }
    } else if (0 == strcmp(slot_name, "average")) {
        sprintf(result, "%.2lf", target->get_average_score(target, &s_frame->score_board));
    } else {
        result = NULL;
    }

    return result;
}
