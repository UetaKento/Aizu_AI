/**
 * @file func.c
 * @date 2016/10/17
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Production system implementation.
 * @details
 *  Artificial Intelligence, 5th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#include "prog_05.h"
#include <stdio.h>
#include <string.h>


typedef int (*FuncComparator)(void const*, void const*);

static void pattern_matching(ProductionSystem*);
static Rule* resolve_conflict(ProductionSystem const*);
static void do_action(Rule const*, ProductionSystem*);
static size_t find_newest_time_tag(Rule const*);
static int compare_time_tag(void const*, void const*);
static int compare_conditions(void const*, void const*);
static void bubble_sort(void*, size_t, size_t, FuncComparator);
static uint32_t rand_xorshift(void);
static void print_rules(Rule**, size_t);
static void print_working_memory(ProductionSystem const*);


/**
 * @brief Function for forward inference using LEX strategy.
 * @param p_system The pointer to the given production system.
 */
void forward_inference(ProductionSystem* p_system)
{
    puts("Start inference");
    printf("Target: %s\n", p_system->target_name);
    printf("Observations: ");
    print_working_memory(p_system);
    printf("Output format: is_used time_tag if(XXX) -> then(YYY)\n");

    while (1) {
        puts("----------------------------------------------------------------------------------------------------");
        // Step 1: pattern matching.
        // Derive matched rules (conflict set).
        pattern_matching(p_system);

        // Step 2: conflict resolution.
        // Resolve conflicts to get a rule.
        Rule* r = resolve_conflict(p_system);
        if (r == NULL) {
            // There is no matched rule.
            break;
        }

        puts("Conflict set:");
        print_rules(p_system->conflict_set, p_system->conflict_count);

        puts("Selected rule:");
        print_rules(&r, 1);

        r->is_used = true;

        // Step 3: action
        // Do action from selected rule and update working memory.
        do_action(r, p_system);

        puts("Current working memory:");
        print_working_memory(p_system);
    }

    printf("Conclusion: %s %s\n\n", p_system->target_name, p_system->working_memory[p_system->observation_count - 1].fact);
}


/**
 * @brief This function finds a conflict set which has observations in the working memory.
 * @param p_system The pointer to the given production system.
 */
static void pattern_matching(ProductionSystem* p_system)
{
    p_system->conflict_count = 0;

    // Find rules which correspond to the observations in the working memory.
    for (size_t i = 0; i < p_system->rule_count; ++i) {
        Rule* rule = &p_system->knowledge_base[i];
        rule->matched_observation_count = 0;

        // Compare the conditions of the rule and the observations in the working memory.
        for (size_t j = 0; j < rule->condition_count; ++j) {
            char const* cond = rule->conditions[j];
            for (size_t k = 0; k < p_system->observation_count; ++k) {
                Observation const* obs = &p_system->working_memory[k];
                if (strcmp(cond, obs->fact) == 0) {
                    rule->matched_observations[rule->matched_observation_count++] = obs;
                }
            }
        }

        // Check the all conditions were matched.
        if (rule->matched_observation_count == rule->condition_count) {
            // Store the matched rule into the conflict set.
            p_system->conflict_set[p_system->conflict_count++] = rule;
        }
    }
}


/**
 * @brief Resolve conflict.
 * @param p_system The pointer to the given production system.
 * @return Pointer to rule which has been selected by LEX strategy.
 */
static Rule* resolve_conflict(ProductionSystem const* p_system)
{
    Rule* selected_rules[MAX_RULE_NUM];
    size_t num_selected_rules = 0;

    // LEX strategy 1: Pick up unused rules from the conflict set.
    for (size_t i = 0; i < p_system->conflict_count; i++) {
        if (p_system->conflict_set[i]->is_used == false) {
            // TODO: 1. add rule into the selected rules when the i-th element in the conflict set was not used.
            selected_rules[num_selected_rules] = p_system->conflict_set[i];
            num_selected_rules ++;
        }
    }

    if (num_selected_rules == 0) {
        // Any rule was not matched.
        return NULL;
    } else if (num_selected_rules == 1) {
        printf("Resolve: step 1\n");

        return selected_rules[0];
    }

    // LEX strategy 2: Select rule which matches to newest observation based on time_tag.
    // TODO: 2. In following strategy execution, you must specify optimal rule for each case.
    //          Set a suitable comparator function as the argument.
    // NOTE: `bubble_sort` updates elements set (`selected_rules` in this context) in descending order.
    bubble_sort(selected_rules, num_selected_rules, sizeof(Rule*), compare_time_tag);

    // Checking whether there are no rules which has same time tag.
    if (find_newest_time_tag(selected_rules[0]) != find_newest_time_tag(selected_rules[1])) {
        printf("Resolve: step 2\n");

        // TODO: 3. Return newest rule.
        return selected_rules[0];
    }

    // If there are rules which has same time tag, remove not duplicated rules.
    size_t newest_time_tag = find_newest_time_tag(selected_rules[0]);
    size_t count_rules = 1;
    for (size_t i = 1; i < num_selected_rules; i++) {
        if (find_newest_time_tag(selected_rules[i]) < newest_time_tag) {
            break;
        }
        count_rules++;
    }
    num_selected_rules = count_rules;

    // LEX strategy 3: Select rule which has many conditions.
    // TODO: 4. Set a suitable comparator function as the argument.
    bubble_sort(selected_rules, num_selected_rules, sizeof(Rule*), compare_conditions);

    // Checking whether there are no rules with the same number of condition.
    if (selected_rules[0]->condition_count != selected_rules[1]->condition_count) {
        printf("Resolve: step 3\n");

        // TODO: 5. Return effective rule which contains many conditions.
        return selected_rules[0];
    }

    // If there are rules which has has many conditions, remove not duplicated rules.
    size_t condition_count = selected_rules[0]->condition_count;
    count_rules = 1;
    for (size_t i = 1; i < num_selected_rules; i++) {
        if (selected_rules[i]->condition_count < condition_count) {
            break;
        }
        count_rules++;
    }
    num_selected_rules = count_rules;

    printf("Resolve: step 4\n");

    // LEX strategy 4: Select randomly.
    // TODO: 6. Generate random index using `rand_xorshift`.
    // Do NOT use `rand` function because it depends on a system environment.
    uint32_t random_idx = rand_xorshift() % num_selected_rules;
    return selected_rules[random_idx];
}


/**
 * @brief Do action to the working memory.
 * @param r Executed rule.
 * @param p_system The pointer to the given production system.
 */
static void do_action(Rule const* r, ProductionSystem* p_system)
{
    Observation* working_memory = p_system->working_memory;
    size_t observation_count = p_system->observation_count;

    switch (r->action) {
        case ACTION_ADD:
            p_system->time_tag++;
            // Add conclusions of rules into the working memory.
            for (size_t i = 0; i < r->conclusion_count; i++) {
                // Skip if the conclusion is already exist.
                bool is_exist = false;
                for (size_t j = 0; j < observation_count; j++) {
                    if (strcmp(working_memory[j].fact, r->conclusions[i]) == 0) {
                        is_exist = true;
                        break;
                    }
                }

                if (is_exist == true) {
                    continue;
                }

                strcpy(working_memory[observation_count].fact, r->conclusions[i]);
                working_memory[observation_count].time_tag = p_system->time_tag;
                observation_count++;
            }
            p_system->observation_count = observation_count;
            break;
        case ACTION_DELETE:
            // Delete the conclusions of rules from the working memory.
            for (size_t i = 0; i < r->conclusion_count; i++) {
                for (size_t j = 0; j < observation_count; j++) {
                    if (strcmp(working_memory[j].fact, r->conclusions[i]) == 0) {
                        for (size_t k = j; k < observation_count - 1; k++) {
                            working_memory[k] = working_memory[k + 1];
                        }
                        p_system->observation_count--;
                        break;
                    }
                }
            }
            break;
    }
}


/**
 * @brief Find newest time tag in matched observations in the rule.
 * @param rule Pointer to rule.
 * @return Newest time tag value.
 */
static size_t find_newest_time_tag(Rule const* rule)
{
    size_t newest_time_tag = 0;
    for (size_t i = 0; i < rule->matched_observation_count; i++) {
        size_t time_tag = rule->matched_observations[i]->time_tag;
        if (newest_time_tag < time_tag) {
            newest_time_tag = time_tag;
        }
    }

    return newest_time_tag;
}


/**
 * @brief Comparison function based on time tag.
 * @param x Pointer to rule.
 * @param y Pointer to rule.
 * @return Comparison result.
 */
static int compare_time_tag(void const* x, void const* y)
{
    return (int)find_newest_time_tag(*((Rule const* const*)y)) - (int)find_newest_time_tag(*((Rule const* const*)x));
}


/**
 * @brief Comparison function based on the number of conditions.
 * @param x Pointer to rule.
 * @param y Pointer to rule.
 * @return Comparison result.
 */
static int compare_conditions(void const* x, void const* y)
{
    return (int)((*(Rule const* const*)y)->condition_count) - (int)(((*(Rule const* const*)x))->condition_count);
}


/**
 * @brief Sorting array by bubble sort (stable sort).
 * @param base Pointer to array which is sorted.
 * @param n The number of array elements.
 * @param size The size of array element.
 * @param f Comparator function for sorting.
 */
static void bubble_sort(void* base, size_t n, size_t size, FuncComparator f)
{
    if ((base == NULL) || (n == 0) || (size == 0) || (f == NULL)) {
        return;
    }
    uintptr_t base_addr = (uintptr_t)base;

    for (size_t i = 0; i < (n - 1); i++) {
        for (size_t j = n - 1; i < j; j--) {
            uintptr_t x = base_addr + size * (j);
            uintptr_t y = base_addr + size * (j - 1);
            if (f((void const*)x, (void const*)y) < 0) {
                // Swap.
                uint8_t* a = (uint8_t*)x;
                uint8_t* b = (uint8_t*)y;

                for (size_t k = 0; k < size; k++) {
                    uint8_t t = a[k];
                    a[k] = b[k];
                    b[k] = t;
                }
            }
        }
    }
}


/**
 * @brief generate random number based xorshift.
 * @return random number
 */
static uint32_t rand_xorshift(void)
{
    static uint32_t x = 102938749;
    static uint32_t y = 120938741;
    static uint32_t z = 789698090;
    static uint32_t r = 17492190;

    uint32_t t = x ^ x << 11;
    x = y;
    y = z;
    z = r;
    r ^= r >> 19 ^ t ^ t >> 8;

    return r;
}


/**
 * @brief Print rules.
 * @param rules Array of pointers to rule struct.
 * @param n The number of pointer to rules.
 */
static void print_rules(Rule** rules, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        Rule const* r = rules[i];

        printf(" %c %zu if(", (r->is_used == true) ? ('#') : (' '), find_newest_time_tag(r));
        for (size_t j = 0; j < r->condition_count; j++) {
            printf("%s && ", r->conditions[j]);
        }
        printf("\b\b\b\b) -> then(");

        for (size_t j = 0; j < r->conclusion_count; j++) {
            printf("%s, ", r->conclusions[j]);
        }
        printf("\b\b)\n");
    }
}


/**
 * @brief Print working memory.
 * @param p_system The pointer to the given production system.
 */
static void print_working_memory(ProductionSystem const* p_system)
{
    for (size_t i = 0; i < p_system->observation_count; i++) {
        printf("%s ", p_system->working_memory[i].fact);
    }
    putchar('\n');
}
