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

TEST_CASE(find_x_asc)
{
    int rc;
    int idx;
    unsigned int n;
    float x;
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
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 0);

    /* Test 2: Try to find x = 3.5 (valid range) */
    x = 3.5f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 4);

    /* Test 3: Try to find x = -1.25 (invalid range on the low end). */
    x = -1.25f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == -1);

    /* Test 4: Try to find x = 4.25 (invalid range on the high end). */
    x = 4.25f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == n);

    /* Test 5: Try to find x = upper limit. */
    x = xy[n-1].x;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == n-2);

    /* Test 6: Try to find x = lower limit. */
    x = xy[0].x;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 0);

    /* Test 7: Try to find x = upper limit + 0.1 */
    x = xy[n-1].x + 0.1f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == n);

    /* Test 8: Try to find x = lower limit - 0.1 */
    x = xy[0].x - 0.1f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == -1);
}

TEST_CASE(find_x_desc)
{
    int rc;
    int idx;
    unsigned int n;
    float x;
    struct intpl_xy xy[6];

    /* (x:4, y:2.0) */
    xy[0].x = 4;
    xy[0].y = 2.0f;

    /* (x:3, y:1.0) */
    xy[1].x = 3;
    xy[1].y = 1.0f;

    /* (x:2, y:0) */
    xy[2].x = 2;
    xy[2].y = 0.0f;

    /* (x:1, y:-1.0) */
    xy[3].x = 1;
    xy[3].y = -1.0f;

    /* (x:0, y:-2.0) */
    xy[4].x = 0;
    xy[4].y = -2.0f;

    /* (x:-1, y:-3.0) */
    xy[5].x = -1;
    xy[5].y = -3.0f;

    /* Calculate the number of entries in xy. */
    n = sizeof xy / sizeof xy[0];

    /* Test 1: Try to find x = -0.25 (valid range) */
    x = -0.25f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 4);

    /* Test 2: Try to find x = 3.5 (valid range) */
    x = 3.5f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 0);

    /* Test 3: Try to find x = 1.75 (valid range) */
    x = 1.75f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 2);

    /* Test 4: Try to find x = -1.25 (invalid range on the high end). */
    x = -1.25f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == n);

    /* Test 5: Try to find x = 4.25 (invalid range on the low end). */
    x = 4.25f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == -1);

    /* Test 6: Try to find x = upper limit. */
    x = xy[n-1].x;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == n-2);

    /* Test 7: Try to find x = lower limit. */
    x = xy[0].x;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(idx == 0);

    /* Test 8: Try to find x = upper limit - 0.1 */
    x = xy[n-1].x - 0.1f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == n);

    /* Test 9: Try to find x = lower limit + 0.1 */
    x = xy[0].x + 0.1f;
    rc = intpl_find_x(xy, n, x, &idx);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
    TEST_ASSERT(idx == -1);
}
