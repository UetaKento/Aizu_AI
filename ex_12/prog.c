/**
 * @file prog.c
 * @date 2016/11/17
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Multi-layer perceptron.
 * @details
 *  Artificial Intelligence 12th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */


#include "prog.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static bool initialize(Mlp3*, Dataset**, CrossValidationDataset*);
static void shuffle_dataset(Dataset*);
static double validation(Mlp3*, Dataset*);
static void finalize(Mlp3*, Dataset*, CrossValidationDataset*);
static double** alloc_2d_array(size_t, size_t);
static void free_2d_array(double**);
static Dataset* alloc_dataset(size_t, size_t, size_t);
static void free_dataset(Dataset*);
static void derive_ith_cross_validation_dataset(CrossValidationDataset*, size_t, Dataset const*);


int main(void)
{
    Mlp3 mlp;
    Dataset* src_dataset;
    CrossValidationDataset cv_dataset;

    while (initialize(&mlp, &src_dataset, &cv_dataset) == true) {
        if (cv_dataset.fold_count == 0) {
            train_mlp(&mlp, src_dataset);
            printf("Error: %lf\n", validation(&mlp, src_dataset));
        } else {
            shuffle_dataset(src_dataset);
            for (size_t i = 0; i < cv_dataset.fold_count; i++) {
                derive_ith_cross_validation_dataset(&cv_dataset, i, src_dataset);

                printf("%zd-th Training\n", i);
                train_mlp(&mlp, cv_dataset.train_dataset);

                printf("%zd-th Test\n", i);
                printf("Error: %lf\n\n", validation(&mlp, cv_dataset.test_dataset));
            }
        }

        // Print results.
        printf("\nResults: input -> output (answer)\n");

        for (size_t i = 0; i < src_dataset->vector_count; i++) {
            FeatureVector const* input_vector = &src_dataset->features[i];
            TeacherVector const* teacher_vector = &src_dataset->teachers[i];

            compute_outputs(&mlp, input_vector);

            for (size_t j = 0; j < src_dataset->feature_dimension; j++) {
                printf("%.2lf ", input_vector->elements[j]);
            }
            printf("-> ");
            for (size_t j = 0; j < mlp.output_neuron_count; j++) {
                printf("%.4lf ", mlp.outputs[j]);
            }
            printf("( ");
            for (size_t j = 0; j < src_dataset->teacher_dimension; j++) {
                printf("%.4lf ", teacher_vector->elements[j]);
            }
            puts(")");
        }
        putchar('\n');

        finalize(&mlp, src_dataset, &cv_dataset);
    }
}


/**
 * @brief Initialize MLP and datasets.
 * @param mlp The pointer to the MLP.
 * @param dataset The pointer to the dataset.
 * @param cv_dataset The pointer to the cross validation dataset.
 */
static bool initialize(Mlp3* mlp, Dataset** dataset, CrossValidationDataset* cv_dataset)
{
    size_t vector_count;

    if (scanf("%zu %zu %zu", &mlp->input_neuron_count, &mlp->hidden_neuron_count, &mlp->output_neuron_count) != 3) {
        return false;
    }
    scanf("%lf %lf %zu", &mlp->learning_rate, &mlp->desired_error, &cv_dataset->fold_count);
    scanf("%zu", &vector_count);

    // Add 1 to the number of neurons for the biases.
    mlp->hidden_weights = alloc_2d_array(mlp->hidden_neuron_count, mlp->input_neuron_count + 1);
    mlp->hidden_outputs = malloc(sizeof(double) * mlp->hidden_neuron_count);
    mlp->output_weights = alloc_2d_array(mlp->output_neuron_count, mlp->hidden_neuron_count + 1);
    mlp->outputs        = malloc(sizeof(double) * mlp->output_neuron_count);
    mlp->hidden_errors  = malloc(sizeof(double) * mlp->hidden_neuron_count);
    mlp->output_errors  = malloc(sizeof(double) * mlp->output_neuron_count);

    Dataset* src_dataset = alloc_dataset(vector_count, mlp->input_neuron_count, mlp->output_neuron_count);

    // Store the input data into the dataset struct.
    for (size_t i = 0; i < src_dataset->vector_count; i++) {
        FeatureVector* feature_vector = &src_dataset->features[i];
        for (size_t j = 0; j < src_dataset->feature_dimension; j++) {
            scanf("%lf", &feature_vector->elements[j]);
        }

        TeacherVector* teacher_vector = &src_dataset->teachers[i];
        for (size_t j = 0; j < src_dataset->teacher_dimension; j++) {
            scanf("%lf", &teacher_vector->elements[j]);
        }
    }

    *dataset = src_dataset;

    // Initialize cross validation dataset.
    cv_dataset->test_dataset = NULL;
    cv_dataset->train_dataset = NULL;

    return true;
}


/**
 * @brief Shuffle the feature vectors in the given dataset randomly.
 * @param dataset The pointer to the given dataset.
 */
static void shuffle_dataset(Dataset* dataset)
{
    // Shuffle feature vectors for cross validation.
    size_t vector_count = dataset->vector_count;
    for (size_t i = 0; i < vector_count; i++) {
        size_t index1 = rand_xorshift() % vector_count;
        size_t index2 = rand_xorshift() % vector_count;

        // Swap.
        FeatureVector tmp_f       = dataset->features[index1];
        dataset->features[index1] = dataset->features[index2];
        dataset->features[index2] = tmp_f;

        TeacherVector tmp_t       = dataset->teachers[index1];
        dataset->teachers[index1] = dataset->teachers[index2];
        dataset->teachers[index2] = tmp_t;
    }
}


/**
 * @brief Test MLP using test dataset.
 * @param mlp The pointer to the MLP.
 * @param dataset The pointer to the test dataset.
 * @return errors of MLP output.
 */
static double validation(Mlp3* mlp, Dataset* dataset)
{
    double error = 0.0;
    for (size_t i = 0; i < dataset->vector_count; i++) {
        FeatureVector const* input_vector = &dataset->features[i];
        TeacherVector const* teacher_vector = &dataset->teachers[i];

        compute_outputs(mlp, input_vector);

        error += compute_errors(mlp, teacher_vector);
    }

    return error;
}


/**
 * @brief Deallocate memories.
 * @param mlp The pointer to the MLP.
 * @param src_dataset the pointer to the dataset.
 * @param cv_dataset the pointer to the cross validation dataset.
 */
static void finalize(Mlp3* mlp, Dataset* src_dataset, CrossValidationDataset* cv_dataset)
{
    free_dataset(src_dataset);
    free_dataset(cv_dataset->test_dataset);
    free_dataset(cv_dataset->train_dataset);

    free_2d_array(mlp->hidden_weights);
    free(mlp->hidden_outputs);
    free_2d_array(mlp->output_weights);
    free(mlp->outputs);
    free(mlp->hidden_errors);
    free(mlp->output_errors);
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
 * @brief Allocate dataset struct and its members.
 * @param vector_count The number of the vectors.
 * @param feature_dimension The dimension of a feature vector.
 * @param teacher_dimension The dimension of a teacher vector.
 * @return The pointer to the allocated dataset.
 */
static Dataset* alloc_dataset(size_t vector_count, size_t feature_dimension, size_t teacher_dimension)
{
    Dataset* dataset           = malloc(sizeof(Dataset));
    dataset->vector_count      = vector_count;
    dataset->feature_dimension = feature_dimension;
    dataset->teacher_dimension = teacher_dimension;
    dataset->features          = malloc(vector_count * sizeof(FeatureVector));
    dataset->teachers          = malloc(vector_count * sizeof(TeacherVector));

    for (size_t i = 0; i < dataset->vector_count; i++) {
        dataset->features[i].elements = malloc(feature_dimension * sizeof(double));
        dataset->teachers[i].elements = malloc(feature_dimension * sizeof(double));
    }

    return dataset;
}


/**
 * @brief Deallocate the dataset.
 * @param dataset The pointer to the given dataset.
 */
static void free_dataset(Dataset* dataset)
{
    if (dataset == NULL) {
        return;
    }

    for (size_t i = 0; i < dataset->vector_count; i++) {
        free(dataset->features[i].elements);
        free(dataset->teachers[i].elements);
    }

    free(dataset->features);
    free(dataset->teachers);
    free(dataset);
}



/**
 * @brief Generate k-fold cross valudation dataset based on `current_fold` value.
 * @param cv_dataset The pointer to the given cross validation dataset.
 * @param current_fold The currenc fold index.
 * @param src_dataset The pointer to the given source dataset.
 */
static void derive_ith_cross_validation_dataset(CrossValidationDataset* cv_dataset, size_t current_fold, Dataset const* src_dataset)
{
    size_t test_vector_count  = src_dataset->vector_count / cv_dataset->fold_count;
    size_t train_vector_count = src_dataset->vector_count - test_vector_count;
    size_t test_index_begin   = current_fold * test_vector_count;
    size_t test_index_end     = test_index_begin + test_vector_count;

    Dataset* test_dataset           = alloc_dataset(test_vector_count, src_dataset->feature_dimension, src_dataset->teacher_dimension);
    test_dataset->vector_count      = test_vector_count;
    test_dataset->feature_dimension = src_dataset->feature_dimension;
    test_dataset->teacher_dimension = src_dataset->teacher_dimension;

    size_t count = 0;
    for (size_t i = test_index_begin; i < test_index_end; i++) {
        memcpy(test_dataset->features[count].elements, src_dataset->features[i].elements, sizeof(double) * src_dataset->feature_dimension);
        memcpy(test_dataset->teachers[count].elements, src_dataset->teachers[i].elements, sizeof(double) * src_dataset->teacher_dimension);
        count++;
    }

    // Free the previous dataset because the number of vectors in the dataset is not always the same.
    free_dataset(cv_dataset->test_dataset);
    cv_dataset->test_dataset = test_dataset;

    Dataset* train_dataset           = alloc_dataset(train_vector_count, src_dataset->feature_dimension, src_dataset->teacher_dimension);
    train_dataset->vector_count      = train_vector_count;
    train_dataset->feature_dimension = src_dataset->feature_dimension;
    train_dataset->teacher_dimension = src_dataset->teacher_dimension;

    count = 0;
    for (size_t i = 0; i < test_index_begin; i++) {
        memcpy(train_dataset->features[count].elements, src_dataset->features[i].elements, sizeof(double) * src_dataset->feature_dimension);
        memcpy(train_dataset->teachers[count].elements, src_dataset->teachers[i].elements, sizeof(double) * src_dataset->teacher_dimension);
        count++;
    }

    for (size_t i = test_index_end; i < src_dataset->vector_count; i++) {
        memcpy(train_dataset->features[count].elements, src_dataset->features[i].elements, sizeof(double) * src_dataset->feature_dimension);
        memcpy(train_dataset->teachers[count].elements, src_dataset->teachers[i].elements, sizeof(double) * src_dataset->teacher_dimension);
        count++;
    }

    // Free the previous dataset because the number of vectors in the dataset is not always the same.
    free_dataset(cv_dataset->train_dataset);
    cv_dataset->train_dataset = train_dataset;
}
