/*!
 * @file   balCommandListBundleD3D12.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <graphics/balICommandList.h>
#include <graphics/balICommandListBundle.h>

// ----------------------------------------------------------------------------
namespace bal::d3d12 {

class CommandListBundle : public ICommandListBundle
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
     * デスクリプターヒープをセットする
     * @param[in] descriptor_heap デスクリプターヒープ
     */
    virtual void setDescriptorHeap(const IDescriptorHeap& descriptor_heap) override;

    /*!
     * デスクリプターテーブルをセットする
     * @param[in] index           テーブル番号
     * @param[in] descriptor_heap デスクリプターヒープ
     */
    virtual void setDescriptorTable(uint32_t index, const IDescriptorHeap& descriptor_heap) override;

    /*!
     * 定数バッファをセットする
     * @param[in] index           テーブル番号
     * @param[in] constant_buffer 定数バッファ
     */
    virtual void setConstantBufferView(uint32_t index, const IConstantBuffer& constant_buffer) override;

    /*!
     * メッシュを描画します
     * @param[in] mesh_buffer    メッシュバッファ
     * @param[in] instance_count インスタンス数
     */
    virtual void drawMesh(const IMeshBuffer& mesh_buffer, uint32_t instance_count) override;

public:
    //! コマンドリスト
    ID3D12GraphicsCommandList* getCommandList() const { return mpCmdList.get(); }

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
