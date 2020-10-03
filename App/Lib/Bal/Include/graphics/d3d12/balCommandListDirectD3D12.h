/*!
 * @file   balCommandListD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balICommandListDirect.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class CommandListDirect : public ICommandListDirect
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) override;

    /*!
     * 記録の開始
     */
    virtual void reset() override;

    /*!
     * 記録の終了
     */
    virtual void close() override;

public:
    /*!
     * パイプラインの設定
     * @param[in] pipeline パイプライン
     */
    virtual void bindPipeline(const IPipeline& pipeline) override;

    /*!
     * デスクリプターテーブルをセットする
     * @param[in] index           インデックス
     * @param[in] descriptor_heap デスクリプターヒープ
     */
    virtual void setDescriptorHeap(uint32_t index, const IDescriptorHeap& descriptor_heap) override;

    /*!
     * メッシュを描画します
     * @param[in] mesh_buffer メッシュバッファ
     */
    virtual void drawMesh(const IMeshBuffer& mesh_buffer) override;

public:
    /*!
     * ビューポートを変更します
     * @param[in] vp ビューポート
     */
    virtual void setViewport(const Viewport& vp) override;

    /*!
     * フレームバッファを切り替えます
     * @param[in] frame_buffer フレームバッファ
     */
    virtual void bindFrameBuffer(const FrameBuffer& frame_buffer) override;

    /*!
     * フレームバッファをクリアします
     * @param[in] frame_buffer フレームバッファ
     * @param[in] clear_flag   クリアフラグ
     * @param[in] color        クリアカラー
     * @param[in] depth        クリアデプス
     * @param[in] stencil      クリアステンシル
     */
    virtual void clear(const FrameBuffer& frame_buffer, uint32_t clear_flag, const MathColor& color, float depth, uint8_t stencil) override;

    /*!
     * リソースのバリアを入れます
     * @param[in] texture     バリアするテクスチャー
     * @param[in] before_type 遷移前のステータス
     * @param[in] after_type  遷移後のステータス
     */
    virtual void resourceBarrier(const ITexture& texture, ResourceBarrierType before_type, ResourceBarrierType after_type) override;

    /*!
     * バンドルを実行します
     * @param[in] cmd_bundle バンドル
     */
    virtual void executeBundle(const ICommandListBundle& cmd_bundle) override;

public:
    //! コマンドリスト
    ID3D12CommandList* getCommandList() const { return mpCmdList.get(); }

private:
    /*!
     * リソースのバリアを入れます
     * @param[in] p_resource    バリア対象のリソース
     * @param[in] before_status 遷移前のステータス
     * @param[in] after_status  遷移後のステータス
     */
    void resourceBarrier_(ID3D12Resource* p_resource, int before_status, int after_status);

private:
    /*!
     * リソースバリアの種類をネイティブに変換
     * @param[in] type 種類
     */
    int convertResourceBarrierType_(ResourceBarrierType type);

private:
    // Com の deleter
    struct ComDeleter
    {
        void operator()(IUnknown* ptr) { ptr->Release(); }
    };

private:
    std::unique_ptr<ID3D12CommandAllocator, ComDeleter>    mpCmdAllocator;
    std::unique_ptr<ID3D12GraphicsCommandList, ComDeleter> mpCmdList;
};

}
