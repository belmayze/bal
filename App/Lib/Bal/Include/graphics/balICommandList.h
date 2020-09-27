/*!
 * @file   balICommandList.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class IDescriptorTable; }
namespace bal { class IShapeBuffer; }
namespace bal { class IPipeline; }

// ----------------------------------------------------------------------------
namespace bal {

class ICommandList
{
public:
    //! デストラクター
    virtual ~ICommandList() {}

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
     * パイプラインの設定
     * @param[in] pipeline パイプライン
     */
    virtual void bindPipeline(const IPipeline& pipeline) = 0;

    /*!
     * デスクリプターテーブルをセットする
     * @param[in] index            インデックス
     * @param[in] descriptor_table デスクリプターテーブル
     */
    virtual void setDescriptorTable(uint32_t index, const IDescriptorTable& descriptor_table) = 0;

    /*!
     * シェイプを描画します
     * @param[in] shape_buffer シェイプバッファ
     */
    virtual void drawShape(const IShapeBuffer& shape_buffer) = 0;
};

}
