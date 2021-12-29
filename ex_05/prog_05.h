/**
 * @file prog_05.h
 * @date 2016/10/17
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Main header file.
 * @details
 *  Artificial Intelligence, 5th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved.
 */

#ifndef PROG_05_H
#define PROG_05_H


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief This enum has numerical constants.
 */
enum {
    MAX_STR_LEN         = 64,
    MAX_CONDITION_NUM   = 8,
    MAX_CONCLUSION_NUM  = 8,
    MAX_RULE_NUM        = 32,
    MAX_OBSERVATION_NUM = 16,
};

/**
 * @enum action
 * @brief This enum defines actions/operations to working memory.
 */
enum action {
    ACTION_ADD,    //!< Add conclusion to working memory.
    ACTION_DELETE, //!< Delete conclusion from working memory.
};
typedef enum action Action;

/**
 * @struct observation
 * @brief This struct defines an observed fact.
 */
struct observation {
    size_t time_tag;        //!< Time when observation was operated.
    char fact[MAX_STR_LEN]; //!< Fact string.
};
typedef struct observation Observation;

/**
 * @struct rule
 * @brief This struct defines a production rule.
 */
struct rule {
    Action action;                                                //!< Type of action of this rule.
    bool is_used;                                                 //!< This is true if this rule was used to resolve conflict.
    char paddings[sizeof(unsigned int) - sizeof(bool)];           //!< Padding for member alignment.
    size_t condition_count;                                        //!< The number of conditions.
    size_t conclusion_count;                                       //!< The number of conclusion.
    size_t matched_observation_count;                              //!< The number of matched observations.
    Observation const* matched_observations[MAX_OBSERVATION_NUM]; //!< Observations which correspond to all conditions of this rule.
    char conclusions[MAX_CONCLUSION_NUM][MAX_STR_LEN];            //!< Conclusions of this rule.
    char conditions[MAX_CONDITION_NUM][MAX_STR_LEN];              //!< Conditions of this rule.
};
typedef struct rule Rule;

/**
 * @struct production_system
 * @brief This struct defines a production system.
 */
struct production_system {
    size_t time_tag;                                 //!< Origin of time.
    size_t rule_count;                               //!< The number of rules in the knowledge base.
    size_t conflict_count;                           //!< The number of rule conflicts.
    size_t observation_count;                        //!< The number of observations.
    Rule* conflict_set[MAX_RULE_NUM];                //!< Confliction set.
    char target_name[MAX_STR_LEN];                   //!< Target name to inference.
    Rule knowledge_base[MAX_RULE_NUM];               //!< Knowledge base, it consists of rules.
    Observation working_memory[MAX_OBSERVATION_NUM]; //!< Working memory, it consists of observations and conclusions of rules.
};
typedef struct production_system ProductionSystem;


extern void forward_inference(ProductionSystem*);


#endif
