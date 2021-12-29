/**
 * @file prog.h
 * @date 2016/11/20
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
 * @struct sonnet
 * @brief This struct defines a self-organizing neural network.
 */
struct sonnet {
    size_t neuron_count;     //!< The number of neurons.
    size_t update_limit;     //!< Update limitation of the training.
    double learning_rate;    //!< Learning rate.
    double** weight_vectors; //!< Weights of the neurons.
};
typedef struct sonnet Sonnet;

/**
 * @struct dataset
 * @brief This struct define an input dataset for training.
 */
struct dataset {
    size_t feature_dimension; //!< The dimension of feature vector.
    size_t vector_count;      //!< The number of feature vectors.
    size_t* classified_tags;  //!< Classified results of the feature vectors
    double** feature_vectors; //!< Feature vectors.
};
typedef struct dataset Dataset;


extern void train_sonnet(Sonnet*, Dataset const*);



#endif
