/**
 * (c) 2014-2016 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"
#include "nucleus/system/lv1.h"

namespace sys {

enum {
    L1GPU_CONTEXT_ALLOCATE_FLAG_UNK2    = (1 <<  1),  // Use 64 KB pages for GART memory mapping (otherwise 1 MB)
    L1GPU_CONTEXT_ALLOCATE_FLAG_UNK4    = (1 <<  2),  // Create DMA objects: 0xFEED0003, 0xFEED0004
    L1GPU_CONTEXT_ALLOCATE_FLAG_UNK20   = (1 <<  5),  // Create DMA objects: 0xBAD68000
    L1GPU_CONTEXT_ALLOCATE_FLAG_UNK400  = (1 << 10),  // Use 512 MB of VRAM IO address space (otherwise 256 MB)
    L1GPU_CONTEXT_ALLOCATE_FLAG_UNK800  = (1 << 11),  // Set IRQ mask to 0x00000000 (otherwise 0xFFFFFFFF)
};

enum {
    L1GPU_CONTEXT_ATTRIBUTE_FIFO_SETUP        = 0x0001,
    L1GPU_CONTEXT_ATTRIBUTE_FIFO_PAUSE        = 0x0002,
    L1GPU_CONTEXT_ATTRIBUTE_FIFO_CONTINUE     = 0x0003,
    L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_MODE_SET  = 0x0100,
    L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_SYNC      = 0x0101,
    L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_FLIP      = 0x0102,
    L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_QUEUE     = 0x0103,
    L1GPU_CONTEXT_ATTRIBUTE_DISPLAY_BUFFER    = 0x0104,
    L1GPU_CONTEXT_ATTRIBUTE_UNK105            = 0x0105,
    L1GPU_CONTEXT_ATTRIBUTE_UNK106            = 0x0106,
    L1GPU_CONTEXT_ATTRIBUTE_UNK107            = 0x0107,
    L1GPU_CONTEXT_ATTRIBUTE_UNK108            = 0x0108,
    L1GPU_CONTEXT_ATTRIBUTE_UNK109            = 0x0109,
    L1GPU_CONTEXT_ATTRIBUTE_UNK10A            = 0x010A,
    L1GPU_CONTEXT_ATTRIBUTE_CURSOR_UNK10B     = 0x010B,
    L1GPU_CONTEXT_ATTRIBUTE_CURSOR_UNK10C     = 0x010C,
    L1GPU_CONTEXT_ATTRIBUTE_CURSOR_UNK10D     = 0x010C,
    L1GPU_CONTEXT_ATTRIBUTE_UNK200            = 0x0200,
    L1GPU_CONTEXT_ATTRIBUTE_UNK201            = 0x0201,
    L1GPU_CONTEXT_ATTRIBUTE_UNK202            = 0x0202,
    L1GPU_CONTEXT_ATTRIBUTE_UNK300            = 0x0300,
    L1GPU_CONTEXT_ATTRIBUTE_UNK301            = 0x0301,
    L1GPU_CONTEXT_ATTRIBUTE_UNK302            = 0x0302,
    L1GPU_CONTEXT_ATTRIBUTE_UNK400            = 0x0400,
    L1GPU_CONTEXT_ATTRIBUTE_FB_SETUP          = 0x0600,
    L1GPU_CONTEXT_ATTRIBUTE_FB_BLIT           = 0x0601,
    L1GPU_CONTEXT_ATTRIBUTE_FB_BLIT_SYNC      = 0x0602,
    L1GPU_CONTEXT_ATTRIBUTE_FB_CLOSE          = 0x0603,
};

enum {
    L1GPU_ATTRIBUTE_UNK002 = 0x0002,
    L1GPU_ATTRIBUTE_UNK100 = 0x0100,
    L1GPU_ATTRIBUTE_UNK105 = 0x0105,
    L1GPU_ATTRIBUTE_UNK202 = 0x0202,
    L1GPU_ATTRIBUTE_UNK400 = 0x0400,
    L1GPU_ATTRIBUTE_UNK401 = 0x0401,
    L1GPU_ATTRIBUTE_UNK402 = 0x0402,
    L1GPU_ATTRIBUTE_UNK403 = 0x0403,
};

/**
 * LV1 SysCall 217 (0xD9): lv1_gpu_context_allocate
 * NOTE: The signature of the function is modified, to make integration with LV2 HLE easier. Original signature below.
 * @param[in]  r3  Argument #1
 * @param[in]  r4  Flags
 * @param[in]  r5  Context
 */
S32 lv1_gpu_context_allocate(BE<U32>* context_id, BE<U64>* lpar_dma_control, BE<U64>* lpar_driver_info, BE<U64>* lpar_reports, U64 mem_ctx, U64 system_mode);

/**
 * LV1 SysCall 225 (0xE1): lv1_gpu_context_attribute
 */
S32 lv1_gpu_context_attribute(S32 context_id, U32 operation_code, U64 p1, U64 p2, U64 p3, U64 p4);

}  // namespace sys
