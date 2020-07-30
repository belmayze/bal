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
#define BAL_PLATFORM_WIN32  (0x0)
#define BAL_PLATFORM_WIN64  (0x1)

#if defined(_WIN32)
#   if defined(_WIN64)
#       define BAL_PLATFORM BAL_PLATFORM_WIN64
#   else
#       define BAL_PLATFORM BAL_PLATFORM_WIN32
#   endif
#else
#   error "Unsupported platform."
#endif

#define BAL_PLATFORM_IS_WIN   (BAL_PLATFORM == BAL_PLATFORM_WIN32 || BAL_PLATFORM == BAL_PLATFORM_WIN64)
#define BAL_PLATFORM_IS_WIN32 (BAL_PLATFORM == BAL_PLATFORM_WIN32)
#define BAL_PLATFORM_IS_WIN64 (BAL_PLATFORM == BAL_PLATFORM_WIN64)

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
#define BAL_LOG_INFO(format, ...) bal::Log::Print(format, __VA_ARGS__)

// プラットフォーム別の必須 include
#if BAL_PLATFORM_IS_WIN
#   define NOMINMAX
#   include <Windows.h>
#   include <d3d12.h>
#   include <dxgi1_6.h>
#   include <tchar.h>
#   include <wrl.h>
#endif
// 使っていい C++ の include
#include <cstdint>
// bal
#include <io/balLog.h>
#include <memory/balUniquePtr.h>
