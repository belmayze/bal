/*!
 * @file   balDef.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// C++
#include <cassert>

// ----------------------------------------------------------------------------
// プラットフォーム
#define BAL_PLATFORM_WIN  (0x0)

#if defined(_WIN32)
#   define BAL_PLATFORM BAL_PLATFORM_WIN
#else
#   error "Unsupported platform."
#endif
#define BAL_PLATFORM_IS_WIN   (BAL_PLATFORM == BAL_PLATFORM_WIN)

// アーキテクチャー
#define BAL_ARCH_X86    (0x00)
#define BAL_ARCH_X64    (0x10)
#define BAL_ARCH_ARM32  (0x20)
#define BAL_ARCH_ARM64  (0x30)

#if BAL_PLATFORM_IS_WIN
#   if defined(_M_ARM64)
#       define BAL_ARCH     BAL_ARCH_ARM64
#   elif defined(_M_ARM)
#       define BAL_ARCH     BAL_ARCH_ARM32
#   elif defined(_M_X64)
#       define BAL_ARCH     BAL_ARCH_X64
#   else
#       define BAL_ARCH     BAL_ARCH_X86
#   endif
#endif
#define BAL_ARCH_IS_X86     (BAL_ARCH == BAL_ARCH_X86)
#define BAL_ARCH_IS_X64     (BAL_ARCH == BAL_ARCH_X64)
#define BAL_ARCH_IS_ARM32   (BAL_ARCH == BAL_ARCH_ARM32)
#define BAL_ARCH_IS_ARM64   (BAL_ARCH == BAL_ARCH_ARM64)

// ライブラリ
#define BAL_GRAPHICS_D3D12  (0x0)

#if BAL_PLATFORM_IS_WIN
#   define BAL_GRAPHICS BAL_GRAPHICS_D3D12
#endif

#define BAL_GRAPHICS_IS_D3D12 (BAL_GRAPHICS == BAL_GRAPHICS_D3D12)

// ビルドターゲット
#define BAL_BUILD_DEBUG       (0x00)
#define BAL_BUILD_DEVELOPMENT (0x10)
#define BAL_BUILD_TEST        (0x20)
#define BAL_BUILD_SHIPPING    (0x30)

#define BAL_BUILD_IS_DEBUG       (BAL_BUILD_TARGET == BAL_BUILD_DEBUG)
#define BAL_BUILD_IS_DEVELOPMENT (BAL_BUILD_TARGET == BAL_BUILD_DEVELOPMENT)
#define BAL_BUILD_IS_TEST        (BAL_BUILD_TARGET == BAL_BUILD_TEST)
#define BAL_BUILD_IS_SHIPPING    (BAL_BUILD_TARGET == BAL_BUILD_SHIPPING)

#define BAL_BUILD_UNDER_DEBUG       (BAL_BUILD_TARGET <= BAL_BUILD_DEBUG)
#define BAL_BUILD_UNDER_DEVELOPMENT (BAL_BUILD_TARGET <= BAL_BUILD_DEVELOPMENT)
#define BAL_BUILD_UNDER_TEST        (BAL_BUILD_TARGET <= BAL_BUILD_TEST)
#define BAL_BUILD_UNDER_SHIPPING    (BAL_BUILD_TARGET <= BAL_BUILD_SHIPPING)

// アサート
#if BAL_BUILD_UNDER_TEST
#   define BAL_ASSERT(cond) assert(cond)
#else
#   define BAL_ASSERT(cond) ((void)0)
#endif // BAL_BUILD_IS_TEST

#define BAL_STATIC_ASSERT(cond, ...) static_assert(cond, __VA_ARGS__)

// ログ
#define BAL_LOG_INFO(format, ...)  bal::Log::Print(bal::Log::EType::Info, format, __VA_ARGS__)
#define BAL_LOG_ERROR(format, ...) bal::Log::Print(bal::Log::EType::Error, format, __VA_ARGS__)

// プラットフォーム別の必須 include
#if BAL_PLATFORM_IS_WIN
#   define NOMINMAX
#   include <Windows.h>
#   include <d3d12.h>
#   include <dxgi1_6.h>
#   include <tchar.h>
#   include <wrl.h>
#   include <Xinput.h>
#endif
// 使っていい C++ の include
#include <array>
#include <bitset>
#include <cstdint>
#include <memory>
#include <tuple>
// bal
#include <io/balLog.h>
#include <memory/balUniquePtr.h>
// gfx
#include <graphics/balGraphicsDefine.h>
