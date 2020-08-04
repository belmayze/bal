/*!
 * @file   balICommandList.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

namespace bal::gfx { class ICommandListBundle; }
namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class IRenderTargetColor; }
namespace bal::gfx { class IRenderTargetDepth; }
namespace bal::gfx { class ITexture; }
namespace bal::gfx { class FrameBuffer; }
namespace bal::gfx { class Viewport; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class ICommandListDirect
{
public:
    enum ClearFlag
    {
        Color   = 0x1,
        Depth   = 0x2,
        Stencil = 0x4
    };
    // 初期化
    struct InitializeArg
    {
        IGraphics* mpGraphics = nullptr;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * ビューポートを変更します
     * @param[in] vp ビューポート
     */
    virtual void setViewport(const Viewport& vp) = 0;

    /*!
     * フレームバッファを切り替えます
     * @param[in] frame_buffer フレームバッファ
     */
    virtual void bindFrameBuffer(const FrameBuffer& frame_buffer) = 0;
    /*!
     * フレームバッファをクリアします
     * @param[in] frame_buffer フレームバッファ
     * @param[in] clear_flag   クリアフラグ
     * @param[in] color        クリアカラー
     * @param[in] depth        クリアデプス
     * @param[in] stencil      クリアステンシル
     */
    virtual void clear(const FrameBuffer& frame_buffer, uint32_t clear_flag, const MathColor& color, float depth, uint8_t stencil) = 0;

    /*!
     * リソースのバリアを入れます
     * @param[in] texture       バリアするテクスチャー
     * @param[in] before_status 遷移前のステータス
     * @param[in] after_status  遷移後のステータス
     */
    virtual void resourceBarrier(const ITexture& texture, int before_status, int after_status) = 0;

    /*!
     * バンドルを実行します
     * @param[in] cmd_bundle バンドル
     */
    virtual void executeBundle(const ICommandListBundle& cmd_bundle) = 0;
};

}
