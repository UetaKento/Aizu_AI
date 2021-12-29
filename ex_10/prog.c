/**
 * @file prog.c
 * @date 2016/11/15
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief k-means algorithm.
 * @details
 *  Artificial Intelligence, 11th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 *
 * - Assignment:
 *  - Fill in all blank parts of codes to work.
 *
 * - Input:
 *      - K:    The number of clusters in dataset.
 *      - M:    The number of feature vectors.
 *      - N:    The dimension of feature vectors.
 *      - D_ij: jth element of ith feature vector.
 *      - L_i:  The label of ith feature vector.
 *
 *      [K] [M] [N]
 *      [D_11, D_12, ... D_1N] [L_1]
 *                 ...
 *      [D_M1, D_M2, ... D_MN] [L_M]
 */

#include "prog.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * @struct kmeans_pack
 * @brief This struct defines a k-means data package.
 */
struct kmeans_pack {
    size_t cluster_count;              //!< The number of clusters.
    Dataset* src_dataset;              //!< The original input data.
    size_t* predicted_labels;          //!< The predicted labels of the vectors by k-means.
    FeatureVector* reference_patterns; //!< The reference patterns.
    CrossValidationDataset cv_dataset; //!< The cross-validation dataset.
};
typedef struct kmeans_pack KmeansPack;


static bool initialize(KmeansPack*);
static void finalize(KmeansPack*);
static Dataset* alloc_dataset(size_t, size_t);
static void free_dataset(Dataset*);
static void shuffle_dataset(Dataset*);
static void derive_ith_cross_validation_dataset(CrossValidationDataset*, size_t, Dataset const*);


int main(void)
{
    KmeansPack kmeans_pack;

    while (initialize(&kmeans_pack) == true) {
        double accuracies[FOLD_COUNT];
        double accuracies_sum = 0.0;
        CrossValidationDataset* cv_dataset = &kmeans_pack.cv_dataset;

        // Iterate the all combinations of the cross-validation.
        for (size_t i = 0; i < FOLD_COUNT; i++) {
            derive_ith_cross_validation_dataset(cv_dataset, i, kmeans_pack.src_dataset);

            Dataset const* train_dataset      = cv_dataset->train_dataset;
            Dataset const* test_dataset       = cv_dataset->test_dataset;
            size_t* predicted_labels          = kmeans_pack.predicted_labels;
            FeatureVector* reference_patterns = kmeans_pack.reference_patterns;
            size_t cluster_count              = kmeans_pack.cluster_count;

            do_kmeans(train_dataset, predicted_labels, reference_patterns, cluster_count);

            adjust_labels(train_dataset, predicted_labels, reference_patterns, cluster_count);

            accuracies[i] = validate(test_dataset, predicted_labels, reference_patterns, cluster_count);

            printf("%zd-th Test\n", i);
            printf("  Accuracy: %.3lf\n", accuracies[i]);
            accuracies_sum += accuracies[i];
        }

        double mean = accuracies_sum / FOLD_COUNT;
        double var = 0.0;
        for (size_t i = 0; i < FOLD_COUNT; i++) {
            double t = accuracies[i] - mean;
            var += t * t;
        }
        var /= FOLD_COUNT;

        printf("\n%d-fold cross-validation result.\n", FOLD_COUNT);
        printf("Mean:    %.3lf\n", mean);
        printf("Variant: %.3lf\n", var);

        finalize(&kmeans_pack);
    }

    return 0;
}


/**
 * @brief Initialize data structs for k-means.
 * @param kmeans_pack The pointer to the given data structs.
 */
static bool initialize(KmeansPack* kmeans_pack)
{
    size_t cluster_count;
    size_t feature_count;
    size_t feature_dimension;

    if (scanf("%zu %zu %zu", &cluster_count, &feature_count, &feature_dimension) != 3) {
        return false;
    }
    kmeans_pack->cluster_count = cluster_count;

    // Allocate memory regions for the input dataset.
    Dataset* src_dataset = alloc_dataset(feature_count, feature_dimension);

    // Load dataset from the input.
    for (size_t i = 0; i < src_dataset->feature_count; i++) {
        for (size_t j = 0; j < src_dataset->feature_dimension; j++) {
            scanf("%lf", &src_dataset->features[i].elements[j]);
        }
        scanf("%zu", &src_dataset->features[i].label);
    }

    // Shuffle dataset for cross-validation.
    shuffle_dataset(src_dataset);

    kmeans_pack->src_dataset = src_dataset;

    // Initialize cross-validation dataset.
    CrossValidationDataset* cv_dataset = &kmeans_pack->cv_dataset;
    cv_dataset->fold_count             = FOLD_COUNT;
    cv_dataset->test_dataset           = NULL;
    cv_dataset->train_dataset          = NULL;

    // Allocate memory regions for the reference patterns.
    FeatureVector* reference_patterns = malloc(sizeof(FeatureVector) * kmeans_pack->cluster_count);
    for (size_t i = 0; i < kmeans_pack->cluster_count; i++) {
        reference_patterns[i].elements = malloc(src_dataset->feature_dimension * sizeof(double));
    }
    kmeans_pack->reference_patterns = reference_patterns;

    // Allocate memory regions for the predicted labels.
    kmeans_pack->predicted_labels = malloc(sizeof(size_t) * src_dataset->feature_count);
    memset(kmeans_pack->predicted_labels, 0, sizeof(size_t) * src_dataset->feature_count);

    return true;
}



/**
 * @brief Deallocate the all memory regions used for k-means.
 * @param kmeans_pack The pointer to the given data structs.
 */
static void finalize(KmeansPack* kmeans_pack)
{
    free(kmeans_pack->predicted_labels);
    for (size_t i = 0; i < kmeans_pack->cluster_count; i++) {
        free(kmeans_pack->reference_patterns[i].elements);
    }
    free(kmeans_pack->reference_patterns);
    free_dataset(kmeans_pack->src_dataset);
    free_dataset(kmeans_pack->cv_dataset.test_dataset);
    free_dataset(kmeans_pack->cv_dataset.train_dataset);
}


/**
 * @brief Allocate dataset struct and its members.
 * @param feature_dimension The dimension of the feature vector.
 * @param feature_count The number of the feature vectors.
 * @return The pointer to the allocated dataset.
 */
static Dataset* alloc_dataset(size_t feature_count, size_t feature_dimension)
{
    Dataset* dataset           = malloc(sizeof(Dataset));
    dataset->feature_count     = feature_count;
    dataset->feature_dimension = feature_dimension;
    dataset->features          = malloc(feature_count * sizeof(FeatureVector));

    for (size_t i = 0; i < dataset->feature_count; i++) {
        dataset->features[i].elements = malloc(feature_dimension * sizeof(double));
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

    for (size_t i = 0; i < dataset->feature_count; i++) {
        free(dataset->features[i].elements);
    }

    free(dataset->features);
    free(dataset);
}


/**
 * @brief Shuffle the feature vectors in the given dataset randomly.
 * @param dataset The pointer to the given dataset.
 */
static void shuffle_dataset(Dataset* dataset)
{
    // Shuffle feature vectors for cross validation.
    size_t feature_count = dataset->feature_count;
    for (size_t i = 0; i < feature_count; i++) {
        size_t index1 = rand_xorshift() % feature_count;
        size_t index2 = rand_xorshift() % feature_count;

        // Swap.
        FeatureVector tmp = dataset->features[index1];
        dataset->features[index1] = dataset->features[index2];
        dataset->features[index2] = tmp;
    }
}


/**
 * @brief Generate k-fold cross valudation dataset based on `current_fold` value.
 * @param cv_dataset The pointer to the given cross validation dataset.
 * @param current_fold The currenc fold index.
 * @param src_dataset The pointer to the given source dataset.
 */
static void derive_ith_cross_validation_dataset(CrossValidationDataset* cv_dataset, size_t current_fold, Dataset const* src_dataset)
{
    size_t test_feature_count  = src_dataset->feature_count / cv_dataset->fold_count;
    size_t train_feature_count = src_dataset->feature_count - test_feature_count;
    size_t test_index_begin    = current_fold * test_feature_count;
    size_t test_index_end      = test_index_begin + test_feature_count;

    Dataset* test_dataset           = alloc_dataset(test_feature_count, src_dataset->feature_dimension);
    test_dataset->feature_count     = test_feature_count;
    test_dataset->feature_dimension = src_dataset->feature_dimension;
    size_t count = 0;
    for (size_t i = test_index_begin; i < test_index_end; i++) {
        memcpy(test_dataset->features[count].elements, src_dataset->features[i].elements, sizeof(double) * src_dataset->feature_dimension);
        test_dataset->features[count].label = src_dataset->features[i].label;
        count++;
    }

    // Free the previous dataset because the number of vectors in the dataset is not always the same.
    free_dataset(cv_dataset->test_dataset);
    cv_dataset->test_dataset = test_dataset;

    Dataset* train_dataset           = alloc_dataset(train_feature_count, src_dataset->feature_dimension);
    train_dataset->feature_count     = train_feature_count;
    train_dataset->feature_dimension = src_dataset->feature_dimension;

    count = 0;
    for (size_t i = 0; i < test_index_begin; i++) {
        memcpy(train_dataset->features[count].elements, src_dataset->features[i].elements, sizeof(double) * src_dataset->feature_dimension);
        train_dataset->features[count].label = src_dataset->features[i].label;
        count++;
    }

    for (size_t i = test_index_end; i < src_dataset->feature_count; i++) {
        memcpy(train_dataset->features[count].elements, src_dataset->features[i].elements, sizeof(double) * src_dataset->feature_dimension);
        train_dataset->features[count].label = src_dataset->features[i].label;
        count++;
    }

    // Free the previous dataset because the number of vectors in the dataset is not always the same.
    free_dataset(cv_dataset->train_dataset);
    cv_dataset->train_dataset = train_dataset;
}
