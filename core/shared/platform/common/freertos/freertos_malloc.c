/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "platform_api_vmcore.h"

void *
os_platform_malloc(unsigned size)
{
    return NULL;
}

void *
os_platform_realloc(void *ptr, unsigned size)
{
    return NULL;
}

void
os_platform_free(void *ptr)
{}
