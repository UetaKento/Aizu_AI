/**
 * @file prog_09.h
 * @date 2016/11/05
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Main header file.
 * @details
 *  Artificial Intelligence 9th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#ifndef PROG_09_H
#define PROG_09_H



#include <stddef.h>


typedef double (*FuncMembership)(double);


/**
 * @brief This enum defines numerical constants.
 */
enum {
    MAX_MEMBERSHIP_FUNCTION_COUNT = 3,
};


/**
 * @struct fuzzy_rule
 * @brief This struct defines a fuzzy rule.
 * @details
 * Fuzzy rule is usually represented below.
 *  If (A is P) and (B is Q), Then (C is R)
 * However, this struct does not distinguish If part and Then part.
 * The reason is Mamdani's fuzzy inference method.
 * Please refer the method.
 */
struct fuzzy_rule {
    size_t function_count;                                          //!< The number of membership functions.
    FuncMembership membership_funcs[MAX_MEMBERSHIP_FUNCTION_COUNT]; //!< The function pointers to membership function.
    double* function_arguments[MAX_MEMBERSHIP_FUNCTION_COUNT];      //!< The pointer to input values of the membership function.
};
typedef struct fuzzy_rule FuzzyRule;


extern void print_car_speed_control_fuzzy(void);



#endif
