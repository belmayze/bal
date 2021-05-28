/*!
 * @file   balDebugProcessTimeHolder.h
 * @brief  処理時間計測結果を保持
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// std
#include <mutex>
#include <shared_mutex>
// bal
#include <container/balList.h>
#include <container/balTreeMap.h>
#include <container/balString.h>
#include <debug/process/balDebugProcessHandle.h>

namespace bal { class StringPtr; }
namespace bal { class TimeSpan; }

// ----------------------------------------------------------------------------
namespace bal::debug {

class ProcessTimeHolder
{
public:
    //! 初期化引数
    struct InitializeArg
    {
        size_t maxNode   = 1024;
        size_t maxThread = 128;
    };

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
     * 初期化
     * @param[in] arg 引数
     */
    void initialize(const InitializeArg& arg);

    /*!
     * 処理負荷をクリアします
     * @note 1フレームに1回、処理の開始時に呼びます
     */
    void clear();

    /*!
     * スレッドを追加します
     * @param[in] name スレッド名
     */
    ProcessHandle addThread(const StringPtr& name);

    /*!
     * ノードを追加します
     * @param[in] name 計測名
     */
    ProcessHandle addNode(const StringPtr& name);

    /*!
     * 処理負荷を追加します
     * @param[in] handle ハンドル
     * @param[in] diff   処理時間
     */
    void end(const ProcessHandle& handle, const TimeSpan& diff);

private:
    TreeMap<ProcessHandle::Data>    mTreeMap;
    List<ProcessHandle::ThreadInfo> mThreadInfoList;
    std::mutex                      mMutexTreeMap;
    std::shared_mutex               mMutexList;
};

}
