/*!
 * @file   balIGraphics.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

namespace bal { class FrameBuffer; }
namespace bal { class ICommandListDirect; }

// ----------------------------------------------------------------------------
namespace bal {

class IGraphics
{
public:
    struct InitializeArg
    {
        HWND     mHwnd;
        MathSize mRenderBufferSize;
        uint32_t mBufferCount = 2;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * 描画前処理
     */
    virtual void preDraw() = 0;

    /*!
     * 描画後処理
     */
    virtual void postDraw() = 0;

    /*!
     * GPUの実行終了を待機する
     */
    virtual void waitGPU() = 0;

    /*!
     * 破棄の処理を記述します
     */
    virtual bool destroy() = 0;

public:
    /*!
     * コマンドリストを取得する
     */
    virtual ICommandListDirect* getCommandList() = 0;

    /*!
     * スワップチェーンのフレームバッファを取得する
     */
    virtual FrameBuffer* getSwapChainFrameBuffer() = 0;

    /*!
     * デフォルトのフレームバッファを取得する
     */
    virtual FrameBuffer* getDefaultFrameBuffer() = 0;
};

}
