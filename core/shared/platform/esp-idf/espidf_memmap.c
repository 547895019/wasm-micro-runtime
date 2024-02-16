/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */

#include "platform_api_vmcore.h"
#include "platform_api_extension.h"

void *
os_mmap(void *hint, size_t size, int prot, int flags)
{
#ifdef CONFIG_SPIRAM
    return heap_caps_aligned_alloc(8, size, MALLOC_CAP_SPIRAM);
#else
    if (prot & MMAP_PROT_EXEC) {
        // Memory allocation with MALLOC_CAP_EXEC will return 4-byte aligned
        // Reserve extra 4 byte to fixup alignment and size for the pointer to
        // the originally allocated address
        void *buf_origin =
            heap_caps_malloc(size + 4 + sizeof(uintptr_t), MALLOC_CAP_EXEC);
        if (!buf_origin) {
            return NULL;
        }
        void *buf_fixed = buf_origin + sizeof(void *);
        if ((uintptr_t)buf_fixed & (uintptr_t)0x7) {
            buf_fixed = (void *)((uintptr_t)(buf_fixed + 4) & (~(uintptr_t)7));
        }

        uintptr_t *addr_field = buf_fixed - sizeof(uintptr_t);
        *addr_field = (uintptr_t)buf_origin;
        return buf_fixed;
    }
    else {
        return os_platform_malloc(size);
    }
#endif
}

void
os_munmap(void *addr, size_t size)
{
    // We don't need special handling of the executable allocations
    // here, free() of esp-idf handles it properly
#ifdef CONFIG_SPIRAM
    heap_caps_free(addr);
#else
    return os_platform_free(addr);
#endif
}

int
os_mprotect(void *addr, size_t size, int prot)
{
    return 0;
}

IRAM_ATTR void os_dcache_flush()
{
#ifdef CONFIG_SPIRAM
	#if CONFIG_IDF_TARGET_ESP32
    extern void esp_spiram_writeback_cache(void);
	esp_spiram_writeback_cache();
	#endif
    extern void spi_flash_disable_interrupts_caches_and_other_cpu(void);
    extern void spi_flash_enable_interrupts_caches_and_other_cpu(void);


    spi_flash_disable_interrupts_caches_and_other_cpu();
    spi_flash_enable_interrupts_caches_and_other_cpu();
#endif
}