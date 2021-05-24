/*!
 * @file   balDebugProcessTimeHolder.h
 * @brief  処理時間計測結果を保持
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <debug/process/balDebugProcessHandle.h>

namespace bal { class StringPtr; }
namespace bal { class TimeSpan; }

// ----------------------------------------------------------------------------
namespace bal::debug {

class ProcessTimeHolder
{
public:
    /*!
     * コンストラクター
     */
    ProcessTimeHolder();

    /*!
     * デストラクター
     */
    virtual ~ProcessTimeHolder();

    /*!
     * 処理負荷をクリアします
     * @note 1フレームに1回、処理の開始時に呼びます
     */
    void clear();

    /*!
     * ノードを追加します
     * @param[in] name     計測名
     * @param[in] p_parent 親ノード
     */
    ProcessHandle addNode(const StringPtr& name, const ProcessHandle* p_parent);

    /*!
     * 処理負荷を追加します
     * @param[in] handle ハンドル
     * @param[in] diff   処理時間
     */
    void setTime(const ProcessHandle& handle, const TimeSpan& diff);
};

}
