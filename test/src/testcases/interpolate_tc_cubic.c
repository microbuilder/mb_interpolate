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

#include <string.h>
#include "interpolate_test_priv.h"

/** Checks if two float values are 'nearly' equal (diff < epsilon) */
static bool
f_is_equal(float a, float b, float epsilon)
{
    float c;

    c = a - b;

    return c < epsilon && -c < epsilon;
}

TEST_CASE(cubic_arr)
{
    int rc;
    unsigned int n;
    float x;
    float y;
    struct intpl_xyc xyc[7];

    /* Make sure all Y2 values are set to 0 by default. */
    memset(xyc, 0, sizeof xyc);

    xyc[0].x = -3;
    xyc[0].y = 0.0f;
    xyc[1].x = -2;
    xyc[1].y = 1.0f;
    xyc[2].x = -1;
    xyc[2].y = 2.0f;
    xyc[3].x = 0;
    xyc[3].y = .75f;
    xyc[4].x = 1;
    xyc[4].y = 0.0f;
    xyc[5].x = 2;
    xyc[5].y = 2.5f;
    xyc[6].x = 3;
    xyc[6].y = -1.25f;

    /* Calculate the number of entries in xy. */
    n = sizeof xyc / sizeof xyc[0];

    /* Calculate y2 values for xyc. */
    rc = intpl_cubic_calc(xyc, n, 1e30, 1e30);
    TEST_ASSERT_FATAL(rc == 0);

    /* Test 1: Try to find x = -1.25 (valid range) */
    x = -1.25f;
    rc = intpl_cubic_arr(xyc, n, x, &y);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(y, 1.907918f, 1E-4F));

    /* Test 2: Not enough samples. */
    x = 0.7f;
    rc = intpl_cubic_arr(xyc, 2, x, &y);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
}
