/**
 * @file func.c
 * @date 2016/11/20
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Self-organizing neural network based on Winner-take-all Learning Rule.
 * @details
 *  Artificial Intelligence 11th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#include "prog.h"
#include <float.h>
#include <stdint.h>
#include <math.h>


static double euclidean_distance(double const*, double const*, size_t);
static uint32_t rand_xorshift(void);
static double frand_xorshift(void);


/**
 * @brief Training Self-organizing neural network based on Winner-take-all learning rule.
 * @param son The pointer to the given sonnet.
 * @param dataset The pointer to the given dataset
 */
void train_sonnet(Sonnet* son, Dataset const* dataset)
{
    size_t feature_dimension = dataset->feature_dimension;

    // Step 1. Randomize all weights.
    for (size_t i = 0; i < son->neuron_count; i++) {
        double* weight = son->weight_vectors[i];
        for (size_t j = 0; j < feature_dimension; j++) {
            // TODO: 1. Initialize the weight randomly.
            weight[j] = frand_xorshift();
        }
    }

    for (size_t i = 0; i < son->update_limit; i++) {
        for (size_t j = 0; j < dataset->vector_count; j++) {
            double const* feature = dataset->feature_vectors[j];

            // Step 2. Find the winner neuron which is most similar to the given feature vector.
            size_t winner_index = 0;
            double min_distance = DBL_MAX;
            for (size_t k = 0; k < son->neuron_count; k++) {
              // TODO: 2. Find the most similar prototype using euclidean distance.
              double dist = euclidean_distance(feature, son->weight_vectors[k], dataset->feature_dimension);
              if (dist < min_distance){
                min_distance = dist;
                winner_index = k;
              }
            }
            dataset->classified_tags[j] = winner_index;

            // Step 3. Update the weights of the winner neuron.
            double* winner_weight = son->weight_vectors[winner_index];
            for (size_t k = 0; k < feature_dimension; k++) {
                // TODO: 3. Update the weights.
                winner_weight[k] = winner_weight[k] + son->learning_rate * (feature[k] - winner_weight[k]);
            }
        }
    }
}


/**
 * @brief Calculate euclidean distance.
 * @param v1 The array of double.
 * @param v2 The array of double.
 * @param length The length of the gien vectors.
 * @return Euclidean distance between the two vectors.
 */
static double euclidean_distance(double const* v1, double const* v2, size_t length)
{
    double sq_sum = 0;
    for (size_t i = 0; i < length; i++) {
        double tmp = v1[i] - v2[i];
        sq_sum += tmp * tmp;
    }

    return sqrt(sq_sum);
}


/**
 * @brief Generate random number based on xorshift.
 * @return Random number.
 */
static uint32_t rand_xorshift(void)
{
    static uint32_t x = 584638537;
    static uint32_t y = 714673918;
    static uint32_t z = 634982433;
    static uint32_t r = 25252;

    uint32_t t = x ^ x << 11;
    x = y;
    y = z;
    z = r;
    r ^= r >> 19 ^ t ^ t >> 8;

    return r;
}


/**
 * @brief Generate random floating number based on xorshift.
 * @return Random floating number.
 */
static double frand_xorshift(void)
{
    return (rand_xorshift() / (double)UINT32_MAX);
}
