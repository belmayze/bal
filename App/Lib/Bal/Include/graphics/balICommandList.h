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

namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class IRenderTargetColor; }
namespace bal::gfx { class IRenderTargetDepth; }
namespace bal::gfx { class Viewport; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class ICommandList
{
public:
    struct InitializeArg
    {
        IGraphics* mpGraphics = nullptr;
    };
    enum class DepthClearFlag
    {
        Depth,
        Stencil,
        DepthAndStencil
    };

public:
    //! デストラクター
    virtual ~ICommandList() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * 記録の開始
     */
    virtual void reset() = 0;

    /*!
     * 記録の終了
     */
    virtual void close() = 0;

public:
    /*!
     * ビューポートを変更します
     * @param[in] vp ビューポート
     */
    virtual void setViewport(const Viewport& vp) = 0;

    /*!
     * レンダーバッファをクリアします
     * @param[in] p_render_target レンダーターゲット
     * @param[in] color           クリアカラー
     */
    virtual void clearColor(IRenderTargetColor* p_render_target, const MathColor& color) = 0;

    /*!
     * デプスバッファをクリアします
     * @param[in] p_render_target レンダーターゲット
     * @param[in] clear_flag      クリアフラグ
     * @param[in] depth           クリアデプス
     * @param[in] stencil         クリアステンシル
     */
    virtual void clearDepthStencil(IRenderTargetDepth* p_render_target, DepthClearFlag clear_flag, float depth, uint32_t stencil) = 0;

    /*!
     * リソースのバリアを入れます
     * @param[in] p_resource    バリア対象のリソース
     * @param[in] before_status 遷移前のステータス
     * @param[in] after_status  遷移後のステータス
     */
    virtual void resourceBarrier(void* p_resource, int before_status, int after_status) = 0;
};

}
