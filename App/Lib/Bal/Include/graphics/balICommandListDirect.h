/*!
 * @file   balICommandList.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balICommandList.h>
#include <math/balMath.h>

namespace bal { class ICommandListBundle; }
namespace bal { class IGraphics; }
namespace bal { class IRenderTargetColor; }
namespace bal { class IRenderTargetDepth; }
namespace bal { class ITexture; }
namespace bal { class FrameBuffer; }
namespace bal { class Viewport; }

// ----------------------------------------------------------------------------
namespace bal {

class ICommandListDirect : public ICommandList
{
public:
    // クリアフラグ
    enum ClearFlag
    {
        Color   = 0x1,
        Depth   = 0x2,
        Stencil = 0x4
    };
    // バリアタイプ
    enum class ResourceBarrierType
    {
        Present,           //!< 画面反映
        RenderTargetColor, //!< カラーのレンダーターゲット
        RenderTargetDepth, //!< デプスのレンダーターゲット
        GenericRead,       //!< 読み込み可能
        CopyDst            //!< コピー可能
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
     * パイプラインの設定
     * @param[in] pipeline パイプライン
     */
    virtual void bindPipeline(const IPipeline& pipeline) = 0;

    /*!
     * デスクリプターヒープをセットする
     * @param[in] descriptor_heap デスクリプターヒープ
     */
    virtual void setDescriptorHeap(const IDescriptorHeap& descriptor_heap) = 0;

    /*!
     * デスクリプターテーブルをセットする
     * @param[in] index           テーブル番号
     * @param[in] descriptor_heap デスクリプターヒープ
     */
    virtual void setDescriptorTable(uint32_t index, const IDescriptorHeap& descriptor_heap) = 0;

    /*!
     * 定数バッファをセットする
     * @param[in] index           テーブル番号
     * @param[in] constant_buffer 定数バッファ
     */
    virtual void setConstantBufferView(uint32_t index, const IConstantBuffer& constant_buffer) = 0;

    /*!
     * メッシュを描画します
     * @param[in] mesh_buffer    メッシュバッファ
     * @param[in] instance_count インスタンス数
     */
    virtual void drawMesh(const IMeshBuffer& mesh_buffer, uint32_t instance_count = 1) = 0;

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
     * @param[in] texture     バリアするテクスチャー
     * @param[in] before_type 遷移前のステータス
     * @param[in] after_type  遷移後のステータス
     */
    virtual void resourceBarrier(const ITexture& texture, ResourceBarrierType before_type, ResourceBarrierType after_type) = 0;

    /*!
     * バンドルを実行します
     * @param[in] cmd_bundle バンドル
     */
    virtual void executeBundle(const ICommandListBundle& cmd_bundle) = 0;

    /*!
     * イベントの開始
     * @param[in] color  計測色
     * @param[in] string 計測名
     */
    virtual void beginEvent(uint64_t color, const StringPtr& string) = 0;

    /*!
     * イベントの終了
     */
    virtual void endEvent() = 0;
};

}
