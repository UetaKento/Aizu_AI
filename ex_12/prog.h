/**
 * @file prog.h
 * @date 2016/11/17
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Main header file.
 * @details
 *  Artificial Intelligence 12th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#ifndef PROG_H
#define PROG_H



#include <stddef.h>
#include <stdint.h>


/**
 * @struct feature_vector
 * @brief This struct define a feature vector.
 */
struct feature_vector {
    double* elements; //!< Values of the vector.
};
typedef struct feature_vector FeatureVector;
typedef struct feature_vector TeacherVector;

/**
 * @struct dataset
 * @brief This struct defines a dataset.
 */
struct dataset {
    size_t vector_count;      //!< The number of feature vector and the number of teacher vectors, the two numbers are equal.
    size_t feature_dimension; //!< The dimension of a feature vector.
    size_t teacher_dimension; //!< The dimension of a teacher vector.
    FeatureVector* features;  //!< Array of feature vectors.
    TeacherVector* teachers;  //!< Array of teacher vectors.
};
typedef struct dataset Dataset;

/**
 * @struct cross_validation_set
 * @brief For k-fold cross validation.
 */
struct cross_validation_dataset {
    size_t fold_count;      //!< The number of fold.
    Dataset* test_dataset;  //!< The dataset for test.
    Dataset* train_dataset; //!< The dataset for training.
};
typedef struct cross_validation_dataset CrossValidationDataset;

/**
 * @struct mpl3
 * @brief This struct defines a three layer perceptron.
 */
struct mlp3 {
    size_t input_neuron_count;  //!< The number of input neurons.
    size_t output_neuron_count; //!< The number of output neurons.
    size_t hidden_neuron_count; //!< The number of hidden neurons.
    double learning_rate;       //!< The learning rate of the MLP.
    double desired_error;       //!< The criterion for the terminating learning iteration.
    double** hidden_weights;    //!< Array of weights between input layer and hidden layer.
    double** output_weights;    //!< Array of weights between hidden layer and output layer.
    double* hidden_outputs;     //!< Array of outputs of hidden layer.
    double* outputs;            //!< Array of outputs of output layer.
    double* hidden_errors;      //!< Array of errors of hidden layer for backpropagation.
    double* output_errors;      //!< Array of errors of output layer for backpropagation.
};
typedef struct mlp3 Mlp3;


extern void train_mlp(Mlp3*, Dataset const*);
extern void compute_outputs(Mlp3*, FeatureVector const*);
extern double compute_errors(Mlp3*, TeacherVector const*);
extern uint32_t rand_xorshift(void);



#endif
