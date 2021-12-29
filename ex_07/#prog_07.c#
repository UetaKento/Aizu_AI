/**
 * @file prog_07.c
 * @date 2016/10/30
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Generate truth table from input logic formula.
 * @details
 *  Artificial Intelligence, 7th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 *
 * Assignment:
 *  Fill in the all blanks for completing the program.
 *  Then, consider logic formula which satisfy truth table below.
 *  And test the logic formula using this program.
 *  After comfirming it is correct, write your formula into `summary_07.txt`.
 *      x y z output
 *      0 0 0      1
 *      0 0 1      1
 *      0 1 0      1
 *      0 1 1      1
 *      1 0 0      1
 *      1 0 1      1
 *      1 1 0      0
 *      1 1 1      1
 *
 * Input:
 *  Logical operators are only `not`, `and`, `or` and `imp`.
 *  Examples
 *      Formulas                    Corresponding inputs
 *          P -> P                      imp(P, P)
 *          ((P -> Q) & P) -> Q         imp(and(imp(P, Q), P), Q)
 */

#include "prog_07.h"
#include <stdio.h>
#include <string.h>


static char* read_input(char*, int, FILE* restrict);
static size_t initialize(char*, LogicVariable*);
static char* remove_tokens(char*, char const**, size_t);


int main(void)
{
    char formula_str[MAX_STR_LEN];
    LogicVariable variables[MAX_LOGIC_VARS];

    // Read a formula string from the input.
    while (read_input(formula_str, sizeof(formula_str), stdin) != NULL) {
        // Initialize.
        size_t variable_count = initialize(formula_str, variables);

        print_truth_table(formula_str, variables, variable_count);
        putchar('\n');
    }

    return 0;
}


/**
 * @brief Read string from specified file pointer into the first argument.
 * @param str The destination string.
 * @param str_len Maximum length of the destination string.
 * @param fp The pointer to the given FILE.
 * @return If reading is success, first argument will be returned.
 *         Otherwise, It will return NULL.
 */
static char* read_input(char* str, int str_len, FILE* restrict fp)
{
    // Read string from file `fp`.
    char* tmp = fgets(str, str_len, fp);
    if (tmp == NULL) {
        return NULL;
    }

    // Skip empty string.
    if (strlen(str) == 0) {
        return NULL;
    }

    // Change \n to \0.
    tmp = strchr(str, '\n');
    if (tmp != NULL) {
        *tmp = '\0';
    }

    return str;
}


/**
 * @brief Initialize logic variables and logic formula string.
 * @param formula_str logic formula string.
 * @param variables array of logic variables.
 * @return The number of logic variables.
 */
static size_t initialize(char* formula_str, LogicVariable* variables)
{
    // Space and reserved tokens.
    static char const* tokens[] = {
        " ",
        "and",
        "or",
        "imp",
        "not",
        "(",
        ")",
    };

    // Generate comma separated variables string from the string.
    char variables_str[MAX_STR_LEN];
    strcpy(variables_str, formula_str);
    remove_tokens(variables_str, tokens, sizeof(tokens) / sizeof(tokens[0]));

    // Create logic variables from the string.
    size_t variable_count = 0;
    char* var_name = variables_str;
    while ((var_name = strtok(var_name, ",")) != NULL) {
        // Check the variable exists already.
        bool is_exist = false;
        for (size_t i = 0; i < variable_count; i++) {
            if (strcmp(var_name, variables[i].name) == 0) {
                is_exist = true;
                break;
            }
        }

        // If the variable does not exist, it will be created.
        if (is_exist == false) {
            strcpy(variables[variable_count].name, var_name);
            variables[variable_count].value = false;
            variable_count++;
        }

        var_name = NULL;
    }

    // Remove only all spaces in the logic formula string.
    remove_tokens(formula_str, tokens, 1);

    return variable_count;
}


/**
 * @brief Remove all tokens from string.
 * @param str string.
 * @param tokens array of tokens.
 * @param token_size the number of tokens.
 */
static char* remove_tokens(char* str, char const** tokens, size_t token_size)
{
    for (size_t i = 0; i < token_size; i++) {
        size_t len = strlen(tokens[i]);
        char* p;
        while ((p = strstr(str, tokens[i])) != NULL) {
            // Shift string after token to left.
            char* after_token = p + len;
            size_t n = strlen(after_token);
            memmove(p, after_token, n + 1);
        }
    }

    return str;
}
