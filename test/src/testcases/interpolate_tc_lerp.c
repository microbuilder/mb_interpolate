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

TEST_CASE(lerp)
{
    int rc;
    float v0, v1, v;
    float t;

    v0 = 0.1f;
    v1 = 0.2f;
    t = 0.5f;

    /* Test 1: Normal conversion. */
    rc = intpl_lerp(v0, v1, t, &v);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(v, 0.15f, 1E-4F, "lerp 1"));

    /* Test 2: t = 0. */
    t = 0.0f;
    rc = intpl_lerp(v0, v1, t, &v);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(v, v0, 1E-4F, "lerp 2"));

    /* Test 3: t = 1.0. */
    t = 1.0f;
    rc = intpl_lerp(v0, v1, t, &v);
    TEST_ASSERT_FATAL(rc == 0);
    TEST_ASSERT(f_is_equal(v, v1, 1E-4F, "lerp 3"));

    /* Test 4: t = -0.01 (invalid value). */
    t = -0.01f;
    rc = intpl_lerp(v0, v1, t, &v);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);

    /* Test 5: t = 1.01 (invalid value). */
    t = 1.01f;
    rc = intpl_lerp(v0, v1, t, &v);
    TEST_ASSERT_FATAL(rc == OS_EINVAL);
}
