/*!
 * @file   balICommandListBundle.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

#include <graphics/balICommandList.h>

namespace bal { class IGraphics; }
namespace bal { class IPipeline; }

// ----------------------------------------------------------------------------
namespace bal {

class ICommandListBundle : public ICommandList
{
public:
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

};

}
