/**
 * @file prog_09.c
 * @date 2016/11/05
 * @author Yuta Kobiyama (m5191140@u-aizu.ac.jp)
 * @brief Inference washing time by fuzzy.
 * @details
 *  Artificial Intelligence 9th Exercise\n
 *  Copyright (C) 2016 System Intelligence Laboratory, All Rights Reserved
 *
 * Mamdani's fuzzy inference method is used in this program.
 *
 * Assignment:
 *  Fill in all blanks to complete the program.
 *  Then, plot 3D figure using gnuplot.
 *  Consider relation between Brake (v) and (Curvature (r), Speed (v)), and write these consideration into `summary_09.txt`.
 */

#include "prog_09.h"


int main(void)
{
    print_car_speed_control_fuzzy();
    return 0;
}
