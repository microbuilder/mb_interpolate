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

#include <math.h>
#include "interpolate/interpolate.h"
#include "interpolate_priv.h"

/* See: https://www.youtube.com/watch?v=vp4nKygufEc */

int
intpl_lerp(float v0, float v1, float t, float *v)
{
    int rc;

    /* Ensure t = 0.0..1.0 */
    if ((t < 0.0f) || (t > 1.0f)) {
        rc = OS_EINVAL;
        *v = NAN;
        goto err;
    }

    *v = (1.0f - t) * v0 + t * v1;

    return 0;
err:
    return rc;
}

int
intpl_find_x(struct intpl_xy xy[], unsigned int n, float x, int *idx)
{
    int rc;
    unsigned int idx_upper;  /* Upper limit */
    unsigned int idx_mid;    /* Midpoint */
    unsigned int idx_lower;  /* Lower limit */
    int order;              /* Ascending (1) or descending (0) */

    /* Init lower and upper limits. */
    idx_lower = 0;
    idx_upper = n;

    /* Make sure we have an appropriately large dataset. */
    if (n < 2) {
        *idx = -1;
        rc = OS_EINVAL;
        goto err;
    }

    /* Determine order (1 = ascending, 0 = descending). */
    order = (xy[n-1].x >= xy[0].x);

    /* xy[0] and xy[n-1] bounds checks. */
    if ((x > xy[n-1].x && order) || (x < xy[n-1].x && !order)) {
        /* Out of bounds on the high end. */
        *idx = n;
        rc = OS_EINVAL;
        goto err;
    } else if ((x < xy[0].x && order) || (x > xy[0].x && !order)) {
        /* Out of bounds on the low end. */
        *idx = -1;
        rc = OS_EINVAL;
        goto err;
    };

    /* Repetitive mid-point computation until a match is made. */
    while (idx_upper - idx_lower > 1) {
        idx_mid = (idx_upper + idx_lower) >> 1;
        if ((x >= xy[idx_mid].x && order) || (x <= xy[idx_mid].x && !order))
        {
            /* Set lower limit to current mid-point. */
            idx_lower = idx_mid;
        } else {
            /* Set upper limit to current mid-point. */
            idx_upper = idx_mid;
        }
    }

    /* Set the output index value. */
    if (x == xy[0].x) {
        /* Return absolute lower limit. */
        *idx = 0;
    } else if(x == xy[n-1].x) {
        /* Return absolute upper limit. */
        *idx = n - 2;
    } else {
        /* Return a value in between. */
        *idx = idx_lower;
    }

    return 0;
err:
    return rc;
}

int
intpl_nn(struct intpl_xy *xy1, struct intpl_xy *xy3, float x2, float *y2)
{
    int rc;
    float delta;

    /* Make sure there is a delta x between xy1 and xy3. */
    delta = xy3->x - xy1->x;
    if (delta < 1E-6F && -delta < 1E-6F) {
        rc = OS_EINVAL;
        *y2 = NAN;
        goto err;
    }

    /* Ensure that x1 <= x2 <= x3. */
    if ((x2 < xy1->x) || (x2 > xy3->x)) {
        rc = OS_EINVAL;
        goto err;
    }

    /* Determine which value is closest, rounding up on 0.5. */
    *y2 = x2 >= (xy1->x * 1.5f) ? xy3->y : xy1->y;

    return 0;
err:
    return rc;
}

int
intpl_nn_arr(struct intpl_xy xy[], unsigned int n, float x, float *y)
{
    int rc;
    int idx;

    /* Find the starting position in xy[] for x. */
    rc = intpl_find_x(xy, n, x, &idx);
    if (rc) {
        *y = NAN;
        goto err;
    }

    /* Perform nearest neighbour interpolation between xy[idx] and xy[idx+1]. */
    rc = intpl_nn(&xy[idx], &xy[idx+1], x, y);
    if (rc) {
        *y = NAN;
        goto err;
    }

    return 0;
 err:
     return rc;
}

int
intpl_lin_y(struct intpl_xy *xy1, struct intpl_xy *xy3, float x2, float *y2)
{
    int rc;
    float delta;

    /* Make sure there is a delta on x between xy1 and xy3. */
    delta = xy3->x - xy1->x;
    if (delta < 1E-6F && -delta < 1E-6F) {
        rc = OS_EINVAL;
        *y2 = NAN;
        goto err;
    }

    /* Ensure that x2 >= x1 && x2 <= x3. */
    if ((x2 < xy1->x) || (x2 > xy3->x)) {
        rc = OS_EINVAL;
        *y2 = NAN;
        goto err;
    }

    /*
     *      (x2 -  x1)(y3 - y1)
     * y2 = ------------------- + y1
     *           (x3 - x1)
     */

    *y2 = ((x2 - xy1->x) * (xy3->y - xy1->y)) / (xy3->x - xy1->x) + xy1->y;

    return 0;
err:
    return rc;
}

int
intpl_lin_y_arr(struct intpl_xy xy[], unsigned int n, float x, float *y)
{
   int rc;
   int idx;

   /* Find the starting position in xy[] for x. */
   rc = intpl_find_x(xy, n, x, &idx);
   if (rc) {
       *y = NAN;
       goto err;
   }

   /* Perform linear interpolation of x between xy[idx] and xy[idx+1]. */
   rc = intpl_lin_y(&xy[idx], &xy[idx+1], x, y);
   if (rc) {
       *y = NAN;
       goto err;
   }

   return 0;
err:
    return rc;
}

int
intpl_lin_x(struct intpl_xy *xy1, struct intpl_xy *xy3, float y2, float *x2)
{
    int rc;
    float min, max, delta;

    /* Make sure there is a delta on x between xy1 and xy3. */
    delta = xy3->x - xy1->x;
    if (delta < 1E-6F && -delta < 1E-6F) {
        rc = OS_EINVAL;
        *x2 = NAN;
        goto err;
    }

    /* Ensure that y2 is within the range of min and max. */
    min = xy1->y < xy3->y ? xy1->y : xy3->y;
    max = xy1->y < xy3->y ? xy3->y : xy1->y;
    if ((y2 <  min) || (y2 > max)) {
        rc = OS_EINVAL;
        *x2 = NAN;
        goto err;
    }

    /*
     *      (y3 - y2) * x1 + (y2 - y1) * x3
     * x2 = -------------------------------
     *                y3 - y1
     */

    *x2 = ((xy3->y - y2) * xy1->x + (y2 - xy1->y) * xy3->x) / (xy3->y - xy1->y);

    return 0;
err:
    return rc;
}

int
intpl_cubic_calc (struct intpl_xyc xyc[], unsigned int n, float yp1, float ypn)
{
    int rc;
    int i;
    int k;
    float sigma;
    float p;
    float qn;
    float un;
    float *u;

    /* Make sure we have at least three values. */
    if (n < 3) {
        rc = OS_EINVAL;
        goto err;
    }

    u = (float *)os_malloc((n-1) * sizeof(float));
    if (u == NULL) {
        rc = OS_ENOMEM;
        goto err;
    }

    if (yp1 > 0.99e30f) {
        xyc[0].y2 = u[0] = 0.0f;
    } else {
        xyc[0].y2 = -0.5f;
        u[0] = (3.0f / (xyc[1].x - xyc[0].x)) *
            ((xyc[1].y - xyc[0].y) / (xyc[1].x - xyc[0].x) - yp1);
    }

    for (i = 1; i < n-1; i++) {
        /* Break out common values. */
        float x_i_im1 = xyc[i].x - xyc[i-1].x;
        float x_ip1_im1 = xyc[i+1].x - xyc[i-1].x;
        sigma = x_i_im1 / x_ip1_im1;
        p = sigma * xyc[i-1].y2 + 2.0f;
        xyc[i].y2 = (sigma - 1.0f) / p;
        u[i] = (xyc[i+1].y - xyc[i].y) / (xyc[i+1].x - xyc[i].x) -
            (xyc[i].y - xyc[i-1].y) / (x_i_im1);
        u[i] = (6.0f * u[i] / (x_ip1_im1) - sigma * u[i-1]) / p;
    }

    if (ypn > 0.99e30f) {
        qn = un = 0.0f;
    } else {
        qn = 0.5f;
        un = (3.0f / (xyc[n-1].x - xyc[n-2].x)) * (ypn - (xyc[n-1].y -
            xyc[n-2].y) / (xyc[n-1].x - xyc[n-2].x));
    }

    xyc[n-1].y2 = (un - qn * u[n-2]) / (qn * xyc[n-2].y2 + 1.0f);

    for (k = n-2; k >= 0; k--) {
        xyc[k].y2 = xyc[k].y2 * xyc[k+1].y2 + u[k];
    }

    os_free(u);

    return 0;
err:
    return rc;
}

int
intpl_cubic_arr(struct intpl_xyc xyc[], unsigned int n, float x, float *y)
{
    int rc;
    int k;              /* Array index value for mid point. */
    int klo;            /* Array index value for low point. */
    int khi;            /* Array index value for high point. */
    static int pklo;    /* Persisted low point for repeat bisection search. */
    static int pkhi;    /* Persisted high point for repeat bisection search. */
    float h;            /* xyc[j+1].x - xyc[j].x */
    float a;            /* (xyc[j+1].x - x) / h */
    float b;            /* (x - xyc[j].x) / h */

    pklo = 0;
    pkhi = 1;

    /* Make sure we have at least three values. */
    if (n < 3) {
        rc = OS_EINVAL;
        goto err;
    }

    /* First check if pklo and pkhi from the last run are still a valid match.
     * The static values are maintained across calls to this function, allowing
     * us to avoid unnecessarily performing a full array search. */
    if(xyc[pklo].x <= x && xyc[pkhi].x > x) {
        klo = pklo;
        khi = pkhi;
    } else {
        /* Search the full array for x using bisection. */
        klo = 0;
        khi = n - 1;
        while(khi - klo > 1) {
            /* Set the midpoint based on the current high/low points.. */
            k = (khi + klo) >> 1;
            /* Determine whether we need to search in upper or lower half. */
            if (xyc[k].x > x) {
                /* If the current midpoint is greater than search value 'x'
                 * set the high marker to the current midpoint, which will
                 * cause search to continue in the lower half. */
                khi = k;
            } else {
                /* Otherwise set the low marker to the current midpoint, which
                 * will cause search to continue in the upper half. */
                klo = k;
            }
            /* Search until results are reduced to neighbouring xyc values. */
        }
        /* Persist pklo and pkhi for future calls to this function. */
        pklo = klo;
        pkhi = khi;
    }

    h = xyc[khi].x - xyc[klo].x;
    if(h == 0) {
        /* No diff = invalid x input! */
        rc = OS_EINVAL;
        goto err;
    }

    /* Calculate coefficients for hi-x (a) and x-lo (b). */
    a = (xyc[khi].x - x) / h;
    b = (x - xyc[klo].x) / h;

    /* Interpolate for y based on a, b using previously calculated y2 vals. */
    *y = a * xyc[klo].y + b * xyc[khi].y +
        ((a * a * a - a) * xyc[klo].y2 + (b * b * b - b) *
        xyc[khi].y2) * (h * h) / 6.0f;

    return 0;
err:
    return rc;
}
