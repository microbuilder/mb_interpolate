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

TEST_CASE(lin_y)
{
    int rc;
    struct intpl_xy xy1;
    struct intpl_xy xy3;
    float y2;

    xy1.x = 10.0f;
    xy1.y = 0.20f;
    xy3.x = 20.0f;
    xy3.y = 0.22f;

    /* Test 1: Interpolate y at x = 13.6 */
    rc = intpl_lin_y(&xy1, &xy3, 13.6f, &y2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(y2, 0.2072f, 1E-4F, "lin_y 1"));

    /* Test 2: Check for x too low. */
    rc = intpl_lin_y(&xy1, &xy3, xy1.x - 0.01f, &y2);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);

    /* Test 3: Check for x too high. */
    rc = intpl_lin_y(&xy1, &xy3, xy3.x + 0.01f, &y2);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);

    /* Test 4: Check for delta 0.0 on x. */
    xy3.x = xy1.x;
    rc = intpl_lin_y(&xy1, &xy3, xy1.x, &y2);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    xy3.x = 20.0f;

    /* Test 5: Negative values. */
    xy1.x = -4.5f;
    xy3.x = 10.0f;
    rc = intpl_lin_y(&xy1, &xy3, 2.75f, &y2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(y2, 0.21f, 1E-4F, "lin_y 5"));
    xy1.x = 10.0f;
    xy3.x = 20.0f;

    /* Test 6: x2 == x1. */
    rc = intpl_lin_y(&xy1, &xy3, xy1.x, &y2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(y2, xy1.y, 1E-4F, "lin_y 6"));

    /* Test 7: x2 == x3. */
    rc = intpl_lin_y(&xy1, &xy3, xy3.x, &y2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(y2, xy3.y, 1E-4F, "lin_y 7"));
}

TEST_CASE(lin_y_arr)
{
    int rc;
    unsigned int n;
    float x;
    float y;
    struct intpl_xy xy[6];

    /* (x:-1, y:-3.0) */
    xy[0].x = -1;
    xy[0].y = -3.0f;

    /* (x:0, y:-2.0) */
    xy[1].x = 0;
    xy[1].y = -2.0f;

    /* (x:1, y:-1.0) */
    xy[2].x = 1;
    xy[2].y = -1.0f;

    /* (x:2, y:0) */
    xy[3].x = 2;
    xy[3].y = 0.0f;

    /* (x:3, y:1.0) */
    xy[4].x = 3;
    xy[4].y = 1.0f;

    /* (x:4, y:2.0) */
    xy[5].x = 4;
    xy[5].y = 2.0f;

    /* Calculate the number of entries in xy. */
    n = sizeof xy / sizeof xy[0];

    /* Test 1: Try to find x = -0.25 (valid range) */
    x = -0.25f;
    rc = intpl_lin_y_arr(xy, n, x, &y);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(y, -2.25f, 1E-4F, "lin_y_arr"));
}

TEST_CASE(lin_x)
{
    int rc;
    struct intpl_xy xy1;
    struct intpl_xy xy3;
    float x2;

    xy1.x = 10.0f;
    xy1.y = 0.20f;
    xy3.x = 20.0f;
    xy3.y = 0.22f;

    /* Test 1: Interpolate x for x = 0.2072f */
    rc = intpl_lin_x(&xy1, &xy3, 0.2072f, &x2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(x2, 13.6f, 1E-4F, "lin_x 1"));

    /* Test 2: Check for y too low. */
    rc = intpl_lin_y(&xy1, &xy3, xy1.y - 0.01f, &x2);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);

    /* Test 3: Check for y too high. */
    rc = intpl_lin_y(&xy1, &xy3, xy3.y + 0.01f, &x2);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);

    /* Test 4: Check for delta 0.0 on x. */
    xy3.x = xy1.x;
    rc = intpl_lin_x(&xy1, &xy3, xy1.y, &x2);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    xy3.x = 20.0f;

    /* Test 5: Negative values. */
    xy1.y = -0.20f;
    xy3.y = 0.20f;
    rc = intpl_lin_x(&xy1, &xy3, -0.1f, &x2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(x2, 12.5f, 1E-4F, "lin_x 5"));
    xy1.y = 0.20f;
    xy3.y = 0.22f;

    /* Test 6: y2 == y1. */
    rc = intpl_lin_x(&xy1, &xy3, xy1.y, &x2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(x2, xy1.x, 1E-4F, "lin_x 6"));

    /* Test 7: y2 == y3. */
    rc = intpl_lin_x(&xy1, &xy3, xy3.y, &x2);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(x2, xy3.x, 1E-4F, "lin_x 7"));
}
