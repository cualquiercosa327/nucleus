/**
 * (c) 2014-2016 Alexandro Sanchez Bach. All rights reserved.
 * Released under GPL v2 license. Read LICENSE for more details.
 */

#pragma once

// CPU backends
#ifdef _NUCLEUS_FEATURE_CPUBACKEND_ARM
#define NUCLEUS_FEATURE_CPUBACKEND_ARM
#endif
#ifdef _NUCLEUS_FEATURE_CPUBACKEND_X86
#define NUCLEUS_FEATURE_CPUBACKEND_X86
#endif

// Graphics backends
#ifdef _NUCLEUS_FEATURE_GFXBACKEND_DIRECT3D11
#define NUCLEUS_FEATURE_GFXBACKEND_DIRECT3D11
#endif
#ifdef _NUCLEUS_FEATURE_GFXBACKEND_DIRECT3D12
#define NUCLEUS_FEATURE_GFXBACKEND_DIRECT3D12
#endif
#ifdef _NUCLEUS_FEATURE_GFXBACKEND_OPENGL
#define NUCLEUS_FEATURE_GFXBACKEND_OPENGL
#endif

