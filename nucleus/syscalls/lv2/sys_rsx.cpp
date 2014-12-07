/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#include "sys_rsx.h"
#include "nucleus/syscalls/lv2.h"
#include "nucleus/syscalls/lv1/lv1_gpu.h"
#include "nucleus/emulator.h"

s32 sys_rsx_device_open()
{
    return CELL_OK;
}

s32 sys_rsx_device_close()
{
    return CELL_OK;
}

/**
 * LV2 SysCall 668 (0x29C): sys_rsx_memory_allocate
 * Allocate space in the RSX local memory.
 *  - mem_handle (OUT): Context / ID, which is used by sys_rsx_memory_free to free allocated memory.
 *  - mem_addr (OUT): Returns the local memory base address, usually 0xC0000000.
 *  - size (IN): Local memory size. E.g. 0x0F900000 (249 MB).
 *  - flags (IN): E.g. Immediate value passed in cellGcmSys is 8.
 *  - a5 (IN): E.g. Immediate value passed in cellGcmSys is 0x00300000 (3 MB?).
 *  - a6 (IN): E.g. Immediate value passed in cellGcmSys is 16. 
 *  - a7 (IN): E.g. Immediate value passed in cellGcmSys is 8.
 */
s32 sys_rsx_memory_allocate(be_t<u32>* mem_handle, be_t<u32>* mem_addr, u32 size, u64 flags, u64 a5, u64 a6, u64 a7)
{
    // LV1 Syscall: lv1_gpu_memory_allocate (0xD6)
    const u32 addr = nucleus.memory(SEG_RSX_LOCAL_MEMORY).alloc(size);
    if (!addr) {
        return CELL_EINVAL;
    }

    *mem_handle = addr; // HACK: On the PS3, this is: *mem_handle = id ^ 0x5A5A5A5A
    *mem_addr = addr;
    return CELL_OK;
}

/**
 * LV2 SysCall 669 (0x29D): sys_rsx_memory_free
 *  - mem_handle (OUT): Context / ID, for allocated local memory generated by sys_rsx_memory_allocate
 */
s32 sys_rsx_memory_free(u32 mem_handle)
{
    // LV1 Syscall: lv1_gpu_memory_free (0xD8)
    nucleus.memory(SEG_RSX_LOCAL_MEMORY).free(mem_handle);

    return CELL_OK;
}

/**
 * LV2 SysCall 670 (0x29E): sys_rsx_context_allocate
 *  - context_id (OUT): RSX context, E.g. 0x55555555 (in vsh.self)
 *  - lpar_dma_control (OUT): Control register area. E.g. 0x60100000 (in vsh.self)
 *  - lpar_driver_info (OUT): RSX data like frequencies, sizes, version... E.g. 0x60200000 (in vsh.self)
 *  - lpar_reports (OUT): Report data area. E.g. 0x60300000 (in vsh.self)
 *  - mem_ctx (IN): mem_ctx given by sys_rsx_memory_allocate
 *  - system_mode (IN):
 */
s32 sys_rsx_context_allocate(be_t<u32>* context_id, be_t<u64>* lpar_dma_control, be_t<u64>* lpar_driver_info, be_t<u64>* lpar_reports, u64 mem_ctx, u64 system_mode)
{
    *lpar_dma_control = nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60100000, 0x1000);
    *lpar_driver_info = nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60200000, 0x4000);
    *lpar_reports = nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60300000, 0x10000);

    nucleus.rsx.dma_control = nucleus.memory.ptr<rsx_dma_control_t>(*lpar_dma_control);
    nucleus.rsx.reports = nucleus.memory.ptr<rsx_report_t>(*lpar_reports);

    // TODO: ???
    auto* driver_info = (be_t<u32>*)((u64)nucleus.memory.getBaseAddr() + *lpar_driver_info);
    driver_info[0] = 0x211;

    *context_id = 0 ^ 0x55555555; // TODO
    return CELL_OK;
}

/**
 * LV2 SysCall 671 (0x29F): sys_rsx_context_free
 *  - context_id (IN): RSX context generated by sys_rsx_context_allocate to free the context.
 */
s32 sys_rsx_context_free(u32 context_id)
{
    return CELL_OK;
}

/**
 * LV2 SysCall 672 (0x2A0): sys_rsx_context_iomap
 *  - context_id (IN): RSX context, E.g. 0x55555555 (in vsh.self)
 *  - io (IN): IO offset mapping area. E.g. 0x00600000
 *  - ea (IN): Start address of mapping area. E.g. 0x20400000
 *  - size (IN): Size of mapping area in bytes. E.g. 0x00200000
 *  - flags (IN):
 */
s32 sys_rsx_context_iomap(u32 context_id, u32 io, u32 ea, u32 size, u64 flags)
{
    return CELL_OK;
}

/**
 * LV2 SysCall 673 (0x2A1): sys_rsx_context_iounmap
 *  - context_id (IN): RSX context, E.g. 0x55555555 (in vsh.self)
 *  - a2 (IN): ?
 *  - io_addr (IN): IO address. E.g. 0x00600000 (Start page 6)
 *  - size (IN): Size to unmap in byte. E.g. 0x00200000
 */
s32 sys_rsx_context_iounmap(u32 context_id, u32 a2, u32 io_addr, u32 size)
{
    return CELL_OK;
}

/**
 * LV2 SysCall 674 (0x2A2): sys_rsx_context_attribute
 *  - context_id (IN): RSX context, e.g. 0x55555555
 *  - package_id (IN): 
 *  - a3 (IN): 
 *  - a4 (IN): 
 *  - a5 (IN): 
 *  - a6 (IN): 
 */
s32 sys_rsx_context_attribute(s32 context_id, u32 package_id, u64 a3, u64 a4, u64 a5, u64 a6)
{
    switch (package_id) {
    case L1GPU_CONTEXT_ATTRIBUTE_FIFO_SETUP:
        nucleus.rsx.dma_control->get = 0; // TODO
        nucleus.rsx.dma_control->put = 0; // TODO
        nucleus.rsx.dma_control->ref = 0xFFFFFFFF; // TODO
        break;
    
    case 0x100: // Display mode set
        break;

    case 0x101: // Display sync
        break;

    case 0x102: // Display flip
        break;

    case 0x103: // ?
        break;

    case 0x104: // Display buffer
        break;

    case 0x106: // ? (Used by cellGcmInitPerfMon)
        break;

    case 0x10a: // ?
        break;

    case 0x300: // Tiles
        break;

    case 0x301: // Depth-buffer (Z-cull)
        break;

    case 0x600: // Framebuffer setup
        break;

    case 0x601: // Framebuffer blit
        break;

    case 0x602: // Framebuffer blit sync
        break;

    default:
        return CELL_EINVAL;
    }

    return CELL_OK;
}

/**
 * LV2 SysCall 675 (0x2A3): sys_rsx_device_map
 *  - a1 (OUT): For example: In vsh.self it is 0x60000000, global semaphore. For a game it is 0x40000000.
 *  - a2 (OUT): Unused?
 *  - dev_id (IN): An immediate value and always 8. (cellGcmInitPerfMon uses 11, 10, 9, 7, 12 successively).
 */
s32 sys_rsx_device_map(be_t<u32>* mapped_addr, be_t<u32>* a2, u32 dev_id)
{
    if (dev_id > 15) {
        return CELL_EINVAL;
    }

    if ((dev_id == 0 || dev_id > 8)/*&& !sub_52450()*/) {
        return CELL_EPERM;
    }

    // LV1 Syscall: lv1_gpu_device_map (0xD4)
    switch (dev_id) {
    case 8:
        *mapped_addr = nucleus.memory(SEG_RSX_MAP_MEMORY).allocFixed(0x60000000, 0x1000);
        break;
    }

    return CELL_OK;
}

/**
 * LV2 SysCall 676 (0x2A4): sys_rsx_device_unmap
 *  - dev_id (IN): An immediate value and always 8.
 */
s32 sys_rsx_device_unmap(u32 dev_id)
{
    return CELL_OK;
}

s32 sys_rsx_attribute()
{
    return CELL_OK;
}
