/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
 
#ifndef _INTERPOLATE_H_
#define _INTERPOLATE_H_

#include <stdint.h>
#include "os/mynewt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* XY struct for nearest neighbour and linear interpolation. */
struct intpl_xy {
    float x;
    float y;
};

/** XY struct for cubic spline interpolation. */
struct intpl_xyc {
    float x;
    float y;
    float y2;   /** Second derivative from the spline. */
};

/**
 * Calculates a number between two numbers using linear interpolation, where
 * `t` is the interpolation factor between 0.0f and 1.0f.
 *
 * @param The lower value used when interpolating.
 * @param The upper value used when interpolating.
 * @param The interpolation factor between 0.0f and 1.0f.
 * @param Pointer to the placeholder for the interpolated value.
 *
 * @return 0 on success, error code on error.
 *
 * This can be used for alpha-blending or easing of motion, amongst
 * other things.
 *
 * For example, you can use intpl_lerp to move an object between two points
 * in 5% increments, creating a smooth animation that slows down as you
 * approach the second point. For each 'step' between the two points you can
 * calculate the new position as follows:
 *
 * x = intpl_lerp(x, finalX, 0.05f);
 * y = intpl_lerp(y, finalY, 0.05);
 *
 */
int intpl_lerp(float v0, float v1, float t, float *v);

/**
 * Uses bisection to search the intpl_xy array for the closest array position
 * to 'x', assigning the matching index value to the 'idx' pointer. The array
 * must be monotonic, but can be either increasing or decreasing between
 * xy[0] and xy[n].
 *
 * @param The array of float-based X,Y values to search.
 * @param The number of elements in the X,Y array.
 * @param The x value to search for.
 * @param Pointer to the placeholder for the position of x in the X,Y array.
 *
 * @return 0 on success, error code on error.
 */
int intpl_find_x(struct intpl_xy xy[], unsigned int n, float x, int *idx);

/**
 * Nearest neighbour (AKA 'piecewise constant') interpolation based on floats.
 *
 * @param The lower XY pair used when interpolating.
 * @param The upper XY pair used when interpolating.
 * @param The X value to interpolate for (>= x1 && <= x3).
 * @param Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int intpl_nn(struct intpl_xy *xy1, struct intpl_xy *xy3,
             float x2, float *y2);

/**
 * Nearest neighbour (AKA 'piecewise constant') interpolation based on an
 * array of floats.
 *
 * @param The array of XY pairs tp use when interpolating (min two!).
 * @param The number of elements in the XY array.
 * @param The X value to interpolate for (x >= xy[0].x, <= xy[n-1].x).
 * @param Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int intpl_nn_arr(struct intpl_xy xy[], unsigned int n, float x, float *y);

/**
 * Linear (AKA 'piecewise linear') interpolation for Y between two points,
 * based on floats.
 *
 * @param The lower XY pair used when interpolating.
 * @param The upper XY pair used when interpolating.
 * @param The X value to interpolate for (>= x1 && <= x3).
 * @param Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int intpl_lin_y(struct intpl_xy *xy1, struct intpl_xy *xy3,
                float x2, float *y2);

/**
 * Linear (AKA 'piecewise linear') interpolation for Y between two points,
 * based on floats.
 *
 * @param The array of XY pairs tp use when interpolating (min two!).
 * @param The number of elements in the XY array.
 * @param The X value to interpolate for (x >= xy[0].x, <= xy[n-1].x).
 * @param Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int intpl_lin_y_arr(struct intpl_xy xy[], unsigned int n, float x, float *y);

/**
 * Linear (AKA 'piecewise linear') interpolation for X between two points,
 * based on floats.
 *
 * @param The lower XY pair used when interpolating.
 * @param The upper XY pair used when interpolating.
 * @param The Y value to interpolate for (>= y1 && <= y3).
 * @param Pointer to the placeholder for the interpolated X value.
 *
 * @return 0 on success, error code on error.
 */
int intpl_lin_x(struct intpl_xy *xy1, struct intpl_xy *xy3,
                float y2, float *x2);

/**
 * Calculates xyc[n].y2 for natural cubic spline interpolation, based on the
 * assigned xyc[n].x and xyc[n].y values.
 *
 * @param The array of X,Y,Y2 values tp use when interpolating (min four!).
 * @param The number of elements in the X,Y,Y2 array.
 * @param 1st derivative at 1. Set to >= 1e30 for natural spline.
 * @param 1st derivative at n'th point. Set to >= 1e30 for natural spline.
 *
 * NOTE: This function must be called BEFORE using intpl_cubic_arr.
 */
int intpl_cubic_calc (struct intpl_xyc xyc[], unsigned int n,
    float yp1, float ypn);

/**
 * Natural cubic spline interpolation between two points, based on floats.
 *
 * @param The array of X,Y,Y2 values tp use when interpolating (min four!).
 * @param The number of elements in the X,Y,Y2 array.
 * @param The X value to interpolate for (x >= xyc[1].x, <= xyc[n-3].x).
 * @param Pointer to the placeholder for the interpolated Y value.
 *
 * @return 0 on success, error code on error.
 */
int intpl_cubic_arr(struct intpl_xyc xyc[], unsigned int n,
    float x, float *y);

#ifdef __cplusplus
}
#endif

#endif /* _INTERPOLATE_H_ */
