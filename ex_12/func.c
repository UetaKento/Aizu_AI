/**
 * @file func.c
 * @date 2016/11/17
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Multi-layer perceptron.
 * @details
 *  Artificial Intelligence 12th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */


#include "prog.h"
#include <math.h>
#include <stdio.h>


static void randomize_weights(Mlp3*);
static void compute_layer_outputs(double*, size_t, double const*, size_t, double**);
static double sigmoid(double);
static double inner_product(double const*, double const*, size_t);
static void update_weights(double**, size_t, double const*, size_t, double const*, double);
static double frand_xorshift(void);
// static double sigmoid_d(double);
/**
 * @brief Training the given MLP using the given dataset.
 * @param mlp The pointer to the given MLP which will be trained.
 * @param dataset The pointer to the given training dataset.
 */
void train_mlp(Mlp3* mlp, Dataset const* dataset)
{
    // Initialize the weights by random numbers.
    randomize_weights(mlp);

    size_t count = 0;
    double error;
    do {
        // Reset the total error.
        error = 0.0;
        for (size_t i = 0; i < dataset->vector_count; i++) {
            // Get a training example.
            FeatureVector const* input_vector   = &dataset->features[i];
            TeacherVector const* teacher_vector = &dataset->teachers[i];

            // Compute the outputs of each layer in the MLP.
            compute_outputs(mlp, input_vector);

            // Compute the error signal.
            error += compute_errors(mlp, teacher_vector);

            // Update weights of output and hidden layer.
            update_weights(mlp->output_weights, mlp->output_neuron_count, mlp->hidden_outputs, mlp->hidden_neuron_count, mlp->output_errors, mlp->learning_rate);
            update_weights(mlp->hidden_weights, mlp->hidden_neuron_count, input_vector->elements, mlp->input_neuron_count, mlp->hidden_errors, mlp->learning_rate);
        }

        if ((count % 10000) == 0) {
            printf("Error in the % 7zd-th learning cycle = %f\n", count, error);
        }

        ++count;
    } while (mlp->desired_error < error);
}


/**
 * @brief Randomize the weights of the given MLP.
 * @param mlp The pointer to the given MLP.
 */
static void randomize_weights(Mlp3* mlp)
{
    for (size_t i = 0; i < mlp->hidden_neuron_count * (mlp->input_neuron_count + 1); i++) {
        mlp->hidden_weights[0][i] = frand_xorshift();
    }

    for (size_t i = 0; i < mlp->output_neuron_count * (mlp->hidden_neuron_count + 1); i++) {
        mlp->output_weights[0][i] = frand_xorshift();
    }
}


/**
 * @brief Calculate the all outputs of the given MLP.
 * @param mlp The pointer to the given MLP.
 * @param input_vector The pointer to input vector.
 */
void compute_outputs(Mlp3* mlp, FeatureVector const* input_vector)
{
    // Calculate outputs of hidden and output layer.
    compute_layer_outputs(mlp->hidden_outputs, mlp->hidden_neuron_count, input_vector->elements, mlp->input_neuron_count, mlp->hidden_weights);
    compute_layer_outputs(mlp->outputs, mlp->output_neuron_count, mlp->hidden_outputs, mlp->hidden_neuron_count, mlp->output_weights);
}


/**
 * @brief Calculate the outputs of a layer.
 * @param outputs The pointer to outputs which will be updated.
 * @param output_count The number of outputs.
 * @param inputs The pointer to input vector of the layer.
 * @param input_dimension The dimension of input vector.
 * @param weights The pointer to weights of the layer.
 */
static void compute_layer_outputs(double* outputs, size_t output_count, double const* inputs, size_t input_dimension, double** weights)
{
    for (size_t i = 0; i < output_count; i++) {
        double const* weight = weights[i];

        // The last element of weight is the threshold (bias).
        outputs[i] = sigmoid(inner_product(weight, inputs, input_dimension) + (-1.0 * weight[input_dimension]));
    }
}


/**
 * @brief Unipolar sigmoid function.
 * @param x Input value.
 * @return Output value.
 */
static double sigmoid(double x)
{
    // TODO: 1. Write unipolar sigmoid function (lambda = 1).
    double y = 0.0;
    double lambda = 1.0;
    y = 1 / (1 + exp(-lambda * x));
    return y;
}

/*
static double sigmoid_d(double x)
{
  double y = 0.0;
  y = sigmoid(x) - sigmoid(x) * sigmoid(x);
  return y;
}
*/

/**
 * @brief Calculate the inner product of the two vectors.
 * @param v1 The pointer to first vector.
 * @param v2 The pointer to second vector.
 * @param length The dimension of two vectors.
 * @return Inner product.
 */
static double inner_product(double const* v1, double const* v2, size_t length)
{
    // TODO: 2. Calculate the inner product.
    double inner = 0.0;
    for(size_t i = 0; i < length; i++){
      inner = inner + v1[i] * v2[i];
    }
    return inner;
}


/**
 * @brief Calculate all errors (neural network errors, output layer errors and hidden layer errors).
 * @param mlp The pointer to the given MLP.
 * @param teacher_vector The pointer to the given teacher vector.
 * @return Error of MLP.
 */
double compute_errors(Mlp3* mlp, TeacherVector const* teacher_vector)
{
    double error = 0;

    for (size_t i = 0; i < mlp->output_neuron_count; i++) {
        double output = mlp->outputs[i];
        double diff = teacher_vector->elements[i] - output;

        // TODO: 3. Calculate the error of the output layer for backpropagation.
        //          Note that the activation function in this program is `the unipolar sigmoid function`.
        mlp->output_errors[i] = diff * (output - (output * output));
        // Calculate error of MLP (error of actual output).
        error += (diff * diff) / 2.0;
    }

    // Calculate errors outputs of hidden layer.
    for (size_t i = 0; i < mlp->hidden_neuron_count; i++) {
        double hidden_error = 0;
        for (size_t j = 0; j < mlp->output_neuron_count; j++) {
            hidden_error += mlp->output_errors[j] * mlp->output_weights[j][i];
        }
        hidden_error *= (1.0 - mlp->hidden_outputs[i]) * mlp->hidden_outputs[i];

        mlp->hidden_errors[i] = hidden_error;
    }

    return error;
}


/**
 * @brief Update weights of a layer.
 * @param weights The pointer to weights that will be updated.
 * @param neuron_count The number of neuron in the layer.
 * @param inputs The pointer to input vector of the layer.
 * @param input_dimension The dimension of input vector.
 * @param errors The pointer to errors of the layer.
 * @param learning_rate Learning late.
 */
static void update_weights(double** weights, size_t neuron_count, double const* inputs, size_t input_dimension, double const* errors, double learning_rate)
{
    for (size_t i = 0; i < neuron_count; i++) {
        for (size_t j = 0; j < input_dimension; j++) {
            // TODO:: 4. Update weight based on backpropagation.
            weights[i][j] = weights[i][j] + learning_rate * errors[i] * inputs[j];
        }

        // Update the threshold.
        weights[i][input_dimension] += learning_rate * errors[i] * -1.0;
    }
}


/**
 * @brief Generate random number based on xorshift.
 * @return Random number.
 */
uint32_t rand_xorshift(void)
{
    static uint32_t x = 123897478;
    static uint32_t y = 156734555;
    static uint32_t z = 693751985;
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
    return rand_xorshift() / (double)UINT32_MAX;
}
