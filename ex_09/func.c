/**
 * @file func.c
 * @date 2017/10/30
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp), Masato Hashimoto (m5201129@u-aizu.ac.jp)
 * @brief Inference car speed control by fuzzy.
 * @details
 *  Artificial Intelligence 9th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 */

#include "prog_09.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>


static double inference(FuzzyRule const*, size_t);
static double compute_memberships(FuzzyRule const*);
static double base_membership(double, double, double, double);


/**
 * @def MAX(x, y)
 * @brief Compute the maximum of x and y.
 */
#define MAX(x, y) (((x) < (y)) ? (y) : (x))

/**
 * @def MIN(x, y)
 * @brief Compute the minimum of x and y.
 */
#define MIN(x, y) (((y) < (x)) ? (y) : (x))

/**
 * @def GEN_MEMBERSHIP_FUNC(left, center, right, label)
 * @brief Generate membership function
 */
#define GEN_MEMBERSHIP_FUNC(left, center, right, label) \
    static double membership_##label(double x)          \
    {                                                   \
        return base_membership(left, center, right, x); \
    }

// Generate each membership function.
GEN_MEMBERSHIP_FUNC( 20,  40,  60, speed_normal)
GEN_MEMBERSHIP_FUNC( 40,  60,  80, speed_bit_fast)
GEN_MEMBERSHIP_FUNC( 60,  80,  80, speed_fast)
GEN_MEMBERSHIP_FUNC( 75, 150, 225, curve_normal)
GEN_MEMBERSHIP_FUNC( 75,  75, 150, curve_steep)
GEN_MEMBERSHIP_FUNC(  0,  20,  40, brake_weak)
GEN_MEMBERSHIP_FUNC( 20,  40,  40, brake_strong)

void print_car_speed_control_fuzzy(void)
{
    // These variables correspond to the mathematical variables in the slides.
    //  v = Speed (velocity)
    //  r = Curvature (radius)
    //  b = Brake

    double velocity;
    double radius;
    double deceleration;

    FuzzyRule rules[] = {
        // Define fuzzy rule 1.
        //  If (v = Normal) and (r = Steep curve), Then (b = Weak)
        [0] = {
            .function_count = 3,
            .membership_funcs = {
                membership_speed_normal,
                membership_curve_steep,
                membership_brake_weak
            },
            .function_arguments = {
                &velocity,
                &radius,
                &deceleration
            }
        },

        // Define fuzzy rule 2.
        //  If (v = A little fast) and (r = Steep curve), Then (b = Weak)
        [1] = {
            .function_count = 3,
            .membership_funcs = {
                membership_speed_bit_fast,
                membership_curve_steep,
                membership_brake_weak
            },
            .function_arguments = {
                &velocity,
                &radius,
                &deceleration
            }
        },

        // Define fuzzy rule 3.
        // If (v = Fast) and (r = Normal), Then (b = Weak)
        [2] = {
            .function_count = 3,
            .membership_funcs = {
                membership_speed_fast,
                membership_curve_normal,
                membership_brake_weak
            },
            .function_arguments = {
                &velocity,
                &radius,
                &deceleration
            }
        },

        // Define fuzzy rule 4.
        // If (v = Fast) and (r = Steep curve), Then (b = Strong)
        [3] = {
            .function_count = 3,
            .membership_funcs = {
                membership_speed_fast,
                membership_curve_steep,
                membership_brake_strong
            },
            .function_arguments = {
                &velocity,
                &radius,
                &deceleration
            }
        }
    };


    size_t rule_count = sizeof(rules) / sizeof(rules[0]);
    double delta = 0.3;

    // Iterate the all values.
    for (velocity = 20.0; velocity <= 80.0; velocity += delta) {
        bool was_outputed = false;

        for (radius = 75.0; radius <= 225.0; radius += delta) {
            double numerator   = 0.0;
            double denominator = 0.0;

            for (deceleration = 0.0; deceleration <= 40.0; deceleration += delta) {
                 // TODO: 1. Fill in the bank parts for defuzzy using centroid.
                double membership_value = inference(rules, rule_count);
                numerator += deceleration * membership_value * delta;
                denominator += membership_value * delta;
            }

            if (0.0 < denominator) {
                // Defuzzify by centroid.
                double defuzzified_value = numerator / denominator;
                printf("%3.2f %3.2f %3.2f\n", velocity, radius, defuzzified_value);

                was_outputed = true;
            }
        }

        if (was_outputed == true) {
            printf("\n");
        }
    }

}


/**
 * @brief Inference based on rules using Mamdani's fuzzy inference method.
 * @param rules Array of function rules.
 * @param rule_count The number of rules.
 * @return Membership function value.
 */
static double inference(FuzzyRule const* rules, size_t rule_count)
{
    double max_value = 0.0;

    for (size_t i = 0; i < rule_count; i++) {
        // TODO: 2. Write codes based on Mamdani's fuzzy inference method.
        max_value = MAX(max_value, compute_memberships(&rules[i]));
    }


    return max_value;
}


/**
 * @brief Compute membership function values in if part.
 * @param rule The pointer to the fuzzy rule.
 * @return Membership function value.
 */
static double compute_memberships(FuzzyRule const* rule)
{
    double min_value = DBL_MAX;

    for (size_t i = 0; i < rule->function_count; i++) {
        // TODO: 3. Write codes based on Mamdani's fuzzy inference method.
        FuncMembership f = rule->membership_funcs[i];
        double *arg = rule->function_arguments[i];
        min_value = MIN(min_value, f(*arg));
    }

    return min_value;
}


/**
 * @brief Compute base membership function value (isosceles triangle).
 * @param left Left tail value.
 * @param center Center value.
 * @param right Right tail value.
 * @param x Calculating point.
 * @return Membership functon value.
 */
static double base_membership(double left, double center, double right, double x)
{

    if ((x <= left) || (right <= x)) {
        // Out of the triangle.
        return 0.0;
    }

    double gradient = 1.0 / MAX(center - left, right - center);
    return gradient * ((x <= center) ? (x - left) : (right - x));
}
