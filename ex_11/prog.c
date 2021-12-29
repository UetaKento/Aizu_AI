/**
 * @file prog.c
 * @date 2016/11/20
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Self-organizing neural network based on Winner-take-all Learning Rule.
 * @details
 *  Artificial Intelligence, 12th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory All Rights Reserved
 */

#include "prog.h"
#include <stdio.h>
#include <stdlib.h>


static void initialize(Sonnet*, Dataset*);
static void finalize(Sonnet*, Dataset*);
static double** alloc_2d_array(size_t, size_t);
static void free_2d_array(double**);
static void print_array(double*, size_t);


int main(void)
{
    Dataset dataset;
    Sonnet son;

    initialize(&son, &dataset);
    train_sonnet(&son, &dataset);

    // Print results.
    for (size_t i = 0; i < son.neuron_count; i++) {
        print_array(son.weight_vectors[i], dataset.feature_dimension);
    }
    puts("\n\n");

    for (size_t i = 0; i < son.neuron_count; i++) {
        for (size_t j = 0; j < dataset.vector_count; j++) {
            if (dataset.classified_tags[j] != i) {
                continue;
            }

            print_array(dataset.feature_vectors[j], dataset.feature_dimension);
        }

        puts("\n\n");
    }

    finalize(&son, &dataset);

    return 0;
}


/**
 * @brief Initialize the network and the training dataset.
 * @param son The pointer to the given sonnet.
 * @param dataset The pointer to the given dataset.
 */
static void initialize(Sonnet* son, Dataset* dataset)
{
    scanf("%zu %zu %lf", &son->neuron_count, &son->update_limit, &son->learning_rate);
    scanf("%zu %zu", &dataset->vector_count, &dataset->feature_dimension);

    // Load data from the input.
    dataset->feature_vectors = alloc_2d_array(dataset->vector_count, dataset->feature_dimension);
    for (size_t i = 0; i < dataset->vector_count; i++) {
        for (size_t j = 0; j < dataset->feature_dimension; j++) {
            scanf("%lf", &dataset->feature_vectors[i][j]);
        }
    }

    dataset->classified_tags = malloc(sizeof(size_t) * dataset->vector_count);

    son->weight_vectors = alloc_2d_array(son->neuron_count, dataset->feature_dimension);
}


/**
 * @brief Deallocate memories.
 * @param son The pointer to the given sonnet.
 * @param dataset The pointer to the given dataset
 */
static void finalize(Sonnet* son, Dataset* dataset)
{
    free_2d_array(dataset->feature_vectors);
    free(dataset->classified_tags);
    free_2d_array(son->weight_vectors);
}


/**
 * @brief Allocate 2 dimension array.
 * @param d1 The size of first dimension.
 * @param d2 The size of second dimension.
 * @return The pointer to 2 dimension array.
 */
static double** alloc_2d_array(size_t d1, size_t d2)
{
    double** array = malloc(sizeof(double*) * d1);
    array[0] = malloc(sizeof(double) * d1 * d2);

    for (size_t i = 0; i < d1; i++) {
        array[i] = &array[0][i * d2];
    }

    return array;
}


/**
 * @brief Deallocate 2 dimension array.
 * @param The pointer to 2 dimention array.
 */
static void free_2d_array(double** array)
{
    free(array[0]);
    free(array);
}


/**
 * @brief Print out array.
 * @param array Array of double.
 * @param length Length of array.
 */
static void print_array(double* array, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        printf("%lf ", array[i]);
    }
    putchar('\n');
}
