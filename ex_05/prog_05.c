/**
 * @file prog_05.c
 * @date 2016/10/17
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Inference program.
 * @details
 *  Artificial Intelligence, 5th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 *
 * Assignment:
 *  Fill in the all blanks for completing the program.
 *  This program is a production system with forward inference.
 *  For resolving the conflicts, LEX strategy is used.
 *
 *  [R]: The number of rules in knowledge base.
 *  [N]: The number of conditions.
 *  [M]: The number of conclusions.
 *  [X]: A condition.
 *  [Y]: A conclusion.
 *  [A]: An action for a rule.
 *  [L]: The number of observations.
 *  [P]: An observation.
 *  [T]: The observed target name.
 *
 *  Input set:
 *      [R]
 *      [N_0] [M_0]
 *      [X_0] ..... [X_N_0]
 *      [Y_0] ..... [Y_M_0]
 *      [A_0]
 *           .......
 *      [N_R] [M_R]
 *      [X_0] ..... [X_N_R]
 *      [Y_0] ..... [Y_M_R]
 *      [A_R]
 *      [L]
 *      [P_0] .....  [P_L]
 *      [T]
 */

#include "prog_05.h"
#include <stdio.h>
#include <string.h>


static bool init_system(ProductionSystem*);


int main(void)
{
    ProductionSystem p_system;

    while (init_system(&p_system) == true) {
        forward_inference(&p_system);
    }

    return 0;
}


/**
 * @brief Initialize the given production system.
 * @param p_system The pointer to the given production system.
 * @return if initialization was succeeded, true is returned. Otherwise false is returned.
 */
static bool init_system(ProductionSystem* p_system)
{
    p_system->time_tag = 0;
    p_system->conflict_count = 0;

    // Load the number of rules.
    if (scanf("%zu", &p_system->rule_count) != 1) {
        return false;
    }

    // Load rules.
    for (size_t i = 0; i < p_system->rule_count; ++i) {
        Rule* const rule = &p_system->knowledge_base[i];
        rule->is_used = false;

        // Load the number of conditions and the number of conclusions in the rule.
        scanf("%zu %zu", &rule->condition_count, &rule->conclusion_count);

        // Load conditions.
        for (size_t n = 0; n < rule->condition_count; ++n) {
            scanf("%s", rule->conditions[n]);
        }

        // Load conclusions.
        for (size_t m = 0; m < rule->conclusion_count; ++m) {
            scanf("%s", rule->conclusions[m]);
        }

        // Load an action.
        char action_str[MAX_STR_LEN];
        scanf("%s", action_str);
        if (strcmp(action_str, "add_to") == 0) {
            rule->action = ACTION_ADD;
        } else if (strcmp(action_str, "delete_from") == 0) {
            rule->action = ACTION_DELETE;
        }
    }

    // Load observations.
    scanf("%zu", &p_system->observation_count);
    for (size_t l = 0; l < p_system->observation_count; ++l) {
        p_system->working_memory[l].time_tag = p_system->time_tag;
        scanf("%s", p_system->working_memory[l].fact);
    }

    // Load inference target name..
    scanf("%s", p_system->target_name);

    return true;
}
