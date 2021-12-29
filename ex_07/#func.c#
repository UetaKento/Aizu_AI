/**
 * @file func.c
 * @date 2016/10/30
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Truth table creator.
 * @details
 *  Artificial Intelligence, 7th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */

#include "prog_07.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static bool evaluate_logic_formula(char const* const, LogicVariable const*, size_t);
static int get_variable_value_by_name(char const*, LogicVariable const*, size_t, bool*);
static FuncSignature conv_func_signature_from_name(char const*);
static bool logic_func_not(bool);
static bool logic_func_and(bool, bool);
static bool logic_func_or(bool, bool);
static bool logic_func_imp(bool, bool);


/**
 * @brief Print truth table from the formula string.
 * @param formula_str The formula string which will be executed.
 * @param variables Logic variables.
 * @param variable_count The number of logic variables.
 */
void print_truth_table(char const* formula_str, LogicVariable* variables, size_t variable_count)
{
    // Print logic variable names and logic formula.
    for (size_t i = 0; i < variable_count; i++) {
        printf("%s ", variables[i].name);
    }
    puts(formula_str);

    // Loop for each combination of bits.
    size_t bit_pattern_end = (~0u) >> ((sizeof(0u) * 8u) - variable_count);
    for (size_t i = 0; i <= bit_pattern_end; i++) {
        // Set bit for each logic variable.
        for (size_t j = 0; j < variable_count; j++) {
            size_t bit_value = (i >> (variable_count - j - 1)) & 1u;

            // Align the output of the variable name.
            printf("%zd ", bit_value);
            for (size_t k = 0; k < strlen(variables[j].name) - 1; k++) {
                putchar(' ');
            }

            variables[j].value = (bit_value == 1) ? (true) : (false);
        }

        // Evaluate the logic formula.
        bool result = evaluate_logic_formula(formula_str, variables, variable_count);
        printf("%d\n", (result == true) ? (1) : (0));
    }
}


/**
 * @brief Evaluate logic formula using specified logic variables.
 * @param formula_str Formula string.
 * @param variables Logic variables.
 * @param variable_count The number of logic variables.
 * @return Evaluation result value.
 */
static bool evaluate_logic_formula(char const* const formula_str, LogicVariable const* variables, size_t variable_count)
{
    if (strpbrk(formula_str, "()") == NULL) {
        // Only one variable in the formula.
        bool value = true;
        int r = get_variable_value_by_name(formula_str, variables, variable_count, &value);
        if (r == EXIT_FAILURE) {
            fprintf(stderr, "ERROR: variable %s does not exist.\n", formula_str);
        }

        return value;
    }

    // Extract argument string from the formula string.
    char const* tail_left     = strchr(formula_str, '(') + 1;
    char const* tail_right    = strrchr(formula_str, ')') - 1;
    uintptr_t len             = (uintptr_t)tail_right - (uintptr_t)tail_left + 1;
    char arg_str[MAX_STR_LEN] = {'\0'};
    strncat(arg_str, tail_left, len);

    // Extract the function name from the formula string.
    len = ((uintptr_t)tail_left - 1) - (uintptr_t)formula_str;
    char func_name[8] = {'\0'};
    strncat(func_name, formula_str, len);

    // Identify the function signature.
    FuncSignature func_sig = conv_func_signature_from_name(func_name);

    // If the function is `not` function, the number of argument is 1 because `not` is unary operation.
    if (func_sig == FUNC_NOT) {
        // Evaluate argument recursively.
        return logic_func_not(evaluate_logic_formula(arg_str, variables, variable_count));
    }

    // Parse the arguments.
    // These strings should be considered in this process.
    //  P,Q
    //  P,or(P,Q)
    //  and(P,Q),Q
    //  and(P,Q),or(P,Q)
    //  and(not(P),Q),or(P,and(Q, P))
    char arg_strs[2][MAX_STR_LEN];
    size_t argc = 0;
    size_t paren_count = 0;
    char const* p = arg_str;
    char const* prev_arg_p = arg_str - 1;
    while (1) {
        p = strpbrk(p, ",()");

        if (p == NULL) {
            // Extract the last argument.
            len = strlen(prev_arg_p + 1);
            strncpy(arg_strs[argc], prev_arg_p + 1, len);
            arg_strs[argc][len] = '\0';
            argc++;
            break;
        } else if ((*p == ',') && (paren_count == 0)) {
            // If comma is found and `paren_count` is zero, this comma is argument separator of current function.
            len = ((uintptr_t)p - (uintptr_t)prev_arg_p) - 1;
            strncpy(arg_strs[argc], prev_arg_p + 1, len);
            arg_strs[argc][len] = '\0';
            argc++;

            prev_arg_p = p;
        } else if (*p == '(') {
            paren_count++;
        } else if (*p == ')') {
            paren_count--;
        }

        p++;
    }

    if (paren_count != 0) {
        fprintf(stderr, "ERROR: Parentheses are mismatched.\n  -> %s", arg_str);
    }

    static bool (*binary_operators[])(bool, bool) = {
        [FUNC_AND] = logic_func_and,
        [FUNC_OR]  = logic_func_or,
        [FUNC_IMP] = logic_func_imp,
    };

    // Evaluate the arguments recursively.
    bool operand1 = evaluate_logic_formula(arg_strs[0], variables, variable_count);
    bool operand2 = evaluate_logic_formula(arg_strs[1], variables, variable_count);

    return (binary_operators[func_sig])(operand1, operand2);
}


/**
 * @brief Get logic variable by name from logic variable array.
 * @param name The given logic variable name.
 * @param vars Logic variables.
 * @param n The number logic variables.
 * @param value Pointer to return value.
 * @return If logic variable does not exist, EXIT_FAILURE will be returned.
 *         Otherwise, EXIT_SUCCESS will be returned.
 */
static int get_variable_value_by_name(char const* name, LogicVariable const* vars, size_t n, bool* value)
{
    for (size_t i = 0; i < n; i++) {
        if (strcmp(name, vars[i].name) == 0) {
            *value = vars[i].value;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}


/**
 * @brief Convert function signature from function name.
 * @param func_name Function name string.
 * @return Function signature.
 */
static FuncSignature conv_func_signature_from_name(char const* func_name)
{
    if (0 == strcmp(func_name, "not")) {
        return FUNC_NOT;
    } else if (0 == strcmp(func_name, "and")) {
        return FUNC_AND;
    } else if (0 == strcmp(func_name, "or")) {
        return FUNC_OR;
    } else if (0 == strcmp(func_name, "imp")) {
        return FUNC_IMP;
    }

    return FUNC_UNKNOWN;
}


/**
 * @brief Function for not operator.
 * @param x The input value.
 * @return The output value.
 */
static bool logic_func_not(bool x)
{
    // TODO: 1. Write logical operation,
}


/**
 * @brief Function for and operator.
 * @param x The first input value.
 * @param y The second input value.
 * @return The output value.
 */
static bool logic_func_and(bool x, bool y)
{
    // TODO: 2. Write logical operation,
}


/**
 * @brief Function for or operator.
 * @param x The first input value.
 * @param y The second input value.
 * @return The output value.
 */
static bool logic_func_or(bool x, bool y)
{
    // TODO: 3. Write logical operation,
}


/**
 * @brief Function for implication operator.
 * @param x The first input value.
 * @param y The second input value.
 * @return The output value.
 */
static bool logic_func_imp(bool x, bool y)
{
    // TODO: 4. Write logical operation,
}
