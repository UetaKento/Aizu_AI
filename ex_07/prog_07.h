/**
 * @file prog_07.h
 * @date 2016/11/05
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Truth table creator header.
 * @details
 *  Artificial Intelligence 7th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */


#ifndef PROG_07_H
#define PROG_07_H



#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Numerical constants.
 */
enum {
    MAX_STR_LEN    = 64,
    MAX_LOGIC_VARS = 32,
};

/**
 * @enum func_signature
 * @brief This enum defines function signatures.
 */
enum func_signature {
    FUNC_NOT = 0,
    FUNC_AND,
    FUNC_OR,
    FUNC_IMP,
    FUNC_UNKNOWN,
};
typedef enum func_signature FuncSignature;

/**
 * @struct logic_variable
 * @brief This struct defines a logic variable.
 */
struct logic_variable {
    char name[16]; //!< Logic variable name.
    bool value;    //!< Logic variable value.
};
typedef struct logic_variable LogicVariable;


extern void print_truth_table(char const*, LogicVariable*, size_t);



#endif
