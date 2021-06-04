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
#include <framework/balFrameworkCallback.h>

namespace bal { class IConstantBuffer; }
namespace bal { class IDescriptorHeap; }
namespace bal { class IPipeline; }
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
     * @param[in] name  計測名
     * @param[in] color 描画色
     */
    ProcessHandle addNode(const StringPtr& name, const MathColor& color);

    /*!
     * 処理負荷を追加します
     * @param[in] handle ハンドル
     * @param[in] diff   処理時間
     */
    void end(const ProcessHandle& handle, const TimeSpan& diff);

    /*!
     * 更新
     * @param[in] arg 引数
     */
    void update(const FrameworkCallback::UpdateArg& arg);

    /*!
     * 描画
     * @param[in] arg 引数
     */
    void draw(const FrameworkCallback::DrawArg& arg);

private:
    //! 頂点情報
    struct Vertex
    {
        MathVector3 mPosition;
        MathColor   mColor;
    };
    //! 定数バッファ
    struct ConstantBufferData
    {
        MathVector2 mPosition;
        MathVector2 mSize;
        MathColor   mColor;
    };
    //! 計測結果情報
    struct ProcessTimeInfo
    {
        String<128> mName;
        TimeSpan    mDiff;
        MathColor   mColor;
    };

private:
    TreeMap<ProcessHandle::Data>    mTreeMap;
    TreeMap<ProcessTimeInfo>        mTreeMapInfo;
    List<ProcessHandle::ThreadInfo> mThreadInfoList;
    std::mutex                      mMutexTreeMap;
    std::shared_mutex               mMutexList;

    uint32_t                         mNumDraw = 0;
    std::unique_ptr<IPipeline>       mpGfxPipeline;
    std::unique_ptr<IConstantBuffer> mpGfxConstantBuffer;
    std::unique_ptr<IDescriptorHeap> mpGfxDescriptorHeap;
};

}
