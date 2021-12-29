/**
 * @file func.c
 * @date 2016/11/15
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief k-means algorithm.
 * @details
 *  Artificial Intelligence, 10th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#include "prog.h"
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


static double EPS = 0.00001;

static void classify_vectors(Dataset const*, size_t*, FeatureVector const*, size_t);
static bool update_reference_pattern(Dataset const*, size_t const*, FeatureVector*, size_t);
static double euclidean_distance(double const*, double const*, size_t);


/**
 * @brief Do k-means algorithm to Classify the given data.
 * @param dataset The pointer to the given dataset.
 * @param predicted_labels The array which contains the classification results from k-means algorithm.
 * @param reference_patterns The array of the reference patterns.
 * @param cluster_count The number of clusters.
 */
void do_kmeans(Dataset const* dataset, size_t* predicted_labels, FeatureVector* reference_patterns, size_t cluster_count)
{
    // Step: 1. Define the all reference patterns randomly using the given data.
    for (size_t i = 0; i < cluster_count; i++) {
        reference_patterns[i].label = i;
    }
    for (size_t i = 0; i < dataset->feature_count; i++) {
        predicted_labels[i] = rand_xorshift() % cluster_count;
    }
    update_reference_pattern(dataset, predicted_labels, reference_patterns, cluster_count);

    bool is_almost_equal = false;
    do {
        // Step: 2. Calculate nearest reference pattern for each vector.
        classify_vectors(dataset, predicted_labels, reference_patterns, cluster_count);

        // Step: 3. Update the all reference patterns using mean.
        is_almost_equal = update_reference_pattern(dataset, predicted_labels, reference_patterns, cluster_count);

        // Step: 4. If new reference patterns are almost equal, stop iteration. Otherwise, go to Step 2.
    } while (is_almost_equal == false);
}


/**
 * @brief Classify the vectors in the given dataset using the reference patterns.
 * @param dataset The pointer to the given dataset.
 * @param predicted_labels The array which contains the classification results from k-means algorithm.
 * @param reference_patterns The array of the reference patterns.
 * @param cluster_count The number of clusters.
 */
static void classify_vectors(Dataset const* dataset, size_t* predicted_labels, FeatureVector const* reference_patterns, size_t cluster_count)
{
    // Find most the similar reference pattern.
    for (size_t i = 0; i < dataset->feature_count; i++) {
        FeatureVector const* input_vector = &dataset->features[i];

        double min_distance = DBL_MAX;
        for (size_t j = 0; j < cluster_count; j++) {
            FeatureVector const* reference_pattern = &reference_patterns[j];

            // TODO: 1. Calculate the most similar reference pattern using euclidean distance.
            double distance = euclidean_distance(input_vector -> elements, reference_pattern -> elements, dataset -> feature_dimension);
            if (distance < min_distance) {
                // TODO: 2. Update the predicted label of the feature vector if a more similar reference pattern is found.
                *(predicted_labels+i) = reference_pattern -> label;
                min_distance = distance;
            }
        }
    }
}


/**
 * @brief Update all reference patterns using the centroids.
 * @param dataset The pointer to the given dataset.
 * @param predicted_labels The array which contains the classification results from k-means algorithm.
 * @param reference_patterns The array of the reference patterns.
 * @param cluster_count The number of clusters.
 * @return true if the difference of the reference patterns was not large, otherwise false.
 */
static bool update_reference_pattern(Dataset const* dataset, size_t const* predicted_labels, FeatureVector* reference_patterns, size_t cluster_count)
{
    size_t feature_dimension      = dataset->feature_dimension;
    size_t feature_dimension_byte = sizeof(double) * feature_dimension;
    double* previous_elements     = malloc(feature_dimension_byte * cluster_count);

    for (size_t i = 0; i < cluster_count; i++) {
        FeatureVector* reference_pattern = &reference_patterns[i];

        // Keep previous reference patterns for comparison.
        memcpy(&previous_elements[i * feature_dimension], reference_pattern->elements, feature_dimension_byte);

        // Cleanup the reference pattern for calculating the centroids.
        memset(reference_pattern->elements, 0, feature_dimension_byte);
    }

    size_t* label_counters = malloc(sizeof(size_t) * cluster_count);
    memset(label_counters, 0, sizeof(size_t) * cluster_count);
    for (size_t i = 0; i < dataset->feature_count; i++) {
        size_t label = predicted_labels[i];
        ++label_counters[label];

        // Calculate summations of the feature vectors for each cluster.
        FeatureVector* reference_pattern = &reference_patterns[label];
        for (size_t j = 0; j < feature_dimension; j++) {
            reference_pattern->elements[j] += dataset->features[i].elements[j];
        }
    }

    for (size_t i = 0; i < cluster_count; i++) {
        FeatureVector* reference_pattern = &reference_patterns[i];

        // Divide the summation by the number of feature vectors.
        for (size_t j = 0; j < feature_dimension; j++) {
            reference_pattern->elements[j] /= label_counters[i];
        }
    }

    bool is_almost_equal = true;
    for (size_t i = 0; i < cluster_count; i++) {
        FeatureVector* reference_pattern = &reference_patterns[i];

        // Check it is over the threshold or not.
        double d = euclidean_distance(reference_pattern->elements, &previous_elements[i * feature_dimension], dataset->feature_dimension);
        if (EPS < d) {
            is_almost_equal = false;
        }
    }

    free(label_counters);
    free(previous_elements);

    return is_almost_equal;
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
    // TODO: 3. Update the predicted label of the feature vector if more similar reference patterns is found.
    double distance = 0.0;
    for (size_t i = 0; i < length; i++){
        distance = distance + (*(v1+i) - *(v2+i)) * (*(v1+i) - *(v2+i));
    }
    return sqrt(distance);
}


/**
 * @brief Adjust labels using confusion matrix.
 * @param dataset The pointer to the given dataset.
 * @param predicted_labels The array which contains the classification results from k-means algorithm.
 * @param reference_patterns The array of the reference patterns.
 * @param cluster_count The number of clusters.
 */
void adjust_labels(Dataset const* dataset, size_t* predicted_labels, FeatureVector* reference_patterns, size_t cluster_count)
{
    size_t byte_size = sizeof(size_t) * cluster_count * cluster_count;
    size_t* confusion_matrix = malloc(byte_size);
    memset(confusion_matrix, 0, byte_size);

    for (size_t i = 0; i < dataset->feature_count; i++) {
        FeatureVector const* feature = &dataset->features[i];
        confusion_matrix[(predicted_labels[i] * cluster_count) + feature->label]++;
    }

    // Label changing mapping.
    size_t* label_mapping = malloc(byte_size);
    memset(label_mapping, 0, byte_size);

    for (size_t i = 0; i < cluster_count; i++) {
        size_t cnt = 0;
        for (size_t j = 0; j < cluster_count; j++) {
            size_t num = confusion_matrix[(i * cluster_count) + j];
            if ((cnt < num)) {
                cnt = num;
                reference_patterns[i].label = j;
                label_mapping[i] = j;
            }
        }
    }

    for (size_t i = 0; i < dataset->feature_count; i++) {
        predicted_labels[i] = label_mapping[predicted_labels[i]];
    }

    free(label_mapping);
    free(confusion_matrix);
}


/**
 * @brief Validate the classified results by k-means algorithm.
 * @param dataset The pointer to the given dataset.
 * @param predicted_labels The array which contains the classification results from k-means algorithm.
 * @param reference_patterns The array of the reference patterns.
 * @param cluster_count The number of clusters.
 * @return Accuracy.
 */
double validate(Dataset const* dataset, size_t* predicted_labels, FeatureVector* reference_patterns, size_t cluster_count)
{
    classify_vectors(dataset, predicted_labels, reference_patterns, cluster_count);

    double correct_count = 0.0;
    for (size_t i = 0; i < dataset->feature_count; i++) {
        // TODO: 4. Count the number of valid results (predicted label == truth label or not).
        if (*(predicted_labels+i) == (dataset -> features+i)->label){
            correct_count = correct_count + 1.0;
        }
    }

    return correct_count / dataset->feature_count;
}


/**
 * @brief Generate random number based on xorshift.
 * @return Random number
 */
size_t rand_xorshift(void)
{
    static uint32_t x = 102938749;
    static uint32_t y = 120938741;
    static uint32_t z = 789698090;
    static uint32_t r = 25252;

    uint32_t t = x ^ x << 11;
    x = y;
    y = z;
    z = r;
    r ^= r >> 19 ^ t ^ t >> 8;

    return (size_t)r;
}
