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
#include <assert.h>
#include <stddef.h>
#include "os/mynewt.h"
#include "testutil/testutil.h"
#include "interpolate_test_priv.h"

TEST_CASE_DECL(lerp)
TEST_CASE_DECL(find_x_asc)
TEST_CASE_DECL(find_x_desc)
TEST_CASE_DECL(nn)
TEST_CASE_DECL(nn_arr)
TEST_CASE_DECL(lin_y)
TEST_CASE_DECL(lin_y_arr)
TEST_CASE_DECL(lin_x)
TEST_CASE_DECL(cubic_arr)

int
intpl_fmt_test_all(void)
{
    intpl_fmt_test_suite();
    return tu_case_failed;
}

TEST_SUITE(intpl_fmt_test_suite)
{
    lerp();
    find_x_asc();
    find_x_desc();
    nn();
    nn_arr();
    lin_y();
    lin_y_arr();
    lin_x();
    cubic_arr();
}

#if MYNEWT_VAL(SELFTEST)

int
main(int argc, char **argv)
{
    sysinit();

    intpl_fmt_test_all();
    return tu_any_failed;
}

#endif
