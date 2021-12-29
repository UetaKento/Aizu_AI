/**
 * @file prog.h
 * @date 2016/11/15
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Main header file.
 * @details
 *  Artificial Intelligence 11th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#ifndef PROG_H
#define PROG_H



#include <stddef.h>


/**
 * @brief Numerical constants.
 */
enum {
    FOLD_COUNT = 3,
};

/**
 * @struct feature_vector
 * @brief This struct defines a feature vector.
 */
struct feature_vector {
    size_t label;     //!< The label of the vector.
    double* elements; //!< The features values.
};
typedef struct feature_vector FeatureVector;

/**
 * @struct dataset
 * @brief This struct defines a dataset.
 */
struct dataset {
    size_t feature_count;     //!< The number of features.
    size_t feature_dimension; //!< The dimension of feature vector.
    FeatureVector* features;  //!< Array of feature vectors.
};
typedef struct dataset Dataset;

/**
 * @struct cross_validation_dataset
 * @brief This struct defines a dataset.
 */
struct cross_validation_dataset {
    size_t fold_count;      //!< The number of folds.
    Dataset* test_dataset;  //!< The dataset for test.
    Dataset* train_dataset; //!< The dataset for training.
};
typedef struct cross_validation_dataset CrossValidationDataset;


extern void do_kmeans(Dataset const*, size_t*, FeatureVector*, size_t);
extern void adjust_labels(Dataset const*, size_t*, FeatureVector*, size_t);
extern double validate(Dataset const*, size_t*, FeatureVector*, size_t);
extern size_t rand_xorshift(void);



#endif
