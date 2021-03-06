﻿/*!
 * @file   balDebugProcessTime.h
 * @brief  処理時間計測
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <debug/process/balDebugProcessHandle.h>
#include <time/balStopwatch.h>

namespace bal { class StringPtr; }

// ----------------------------------------------------------------------------
namespace bal::debug {

class ProcessTime
{
public:
    /*!
     * コンストラクター
     * @param[in] name  計測名
     * @param[in] color 描画色
     */
    ProcessTime(const StringPtr& name, const MathColor& color);

    /*!
     * デストラクター
     */
    virtual ~ProcessTime();

private:
    ProcessHandle mHandle;
    Stopwatch     mStopwatch;
};

}
