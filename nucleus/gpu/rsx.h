/**
 * (c) 2014 Nucleus project. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

#include "nucleus/common.h"
#include "nucleus/gpu/renderer.h"

#include <stack>
#include <thread>

struct rsx_dma_control_t {
    u8 unk[0x40];
    be_t<u32> put; // CellGcmControl
    be_t<u32> get; // CellGcmControl
    be_t<u32> ref; // CellGcmControl
};

struct rsx_driver_info_t {
    be_t<u32> version_driver;
    be_t<u32> version_gpu;
    be_t<u32> memory_size;
    be_t<u32> hardware_channel;
    be_t<u32> nvcore_frequency;
    be_t<u32> memory_frequency;
};

struct rsx_reports_t {
    u8 unk1[0x10C0];
    u8 unk2[0x40];
    u32 flip;
};

struct rsx_report_t {
    be_t<u64> timer;
    be_t<u32> value;
    be_t<u32> padding;
};

union rsx_method_t
{
#define FIELD(from, to, type) struct{ u32:(32-to-1); type:(to-from+1); u32:from; }

    u32 method;

    FIELD( 1,  1, u32 flag_ni);        // Non-Incrementing method
    FIELD( 2,  2, u32 flag_jump);      // Jump command
    FIELD(14, 14, u32 flag_ret);       // Return command
    FIELD(30, 30, u32 flag_call);      // Call command
    FIELD( 3, 13, u32 method_count);   // Method arguments count
    FIELD(16, 29, u32 method_offset);  // Method offset (>> 2)
    FIELD( 2, 31, u32 jump_offset);    // Jump IO offset (>> 2)
    FIELD( 0, 29, u32 call_offset);    // Call IO offset (>> 2)
    
#undef FIELD
};

class RSX
{
    // Renderer (Null, Software, OpenGL, DirectX)
    RSXRenderer* m_renderer;

    // Thread responsible of fetching methods and rendering
    std::thread* m_pfifo_thread;

    // Call stack
    std::stack<u32> m_pfifo_stack;

public:
    // Method registers: Note that method offsets need to be (>> 2)-shifted.
    u32 regs[0x4000];

    // RSX Local Memory (mapped into the user space)
    rsx_dma_control_t* dma_control;
    rsx_driver_info_t* driver_info;
    rsx_reports_t* reports;

    // IO Address (mapped into GPU memory through FlexIO)
    u32 io_address;

    void init();

    void task();

    void method(u32 offset, u32 count, const be_t<u32>* args);
};
