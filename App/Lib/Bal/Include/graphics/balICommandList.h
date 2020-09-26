/*!
 * @file   balICommandList.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class IDescriptorTable; }
namespace bal { class IModelBuffer; }
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
     * モデルを描画します
     * @param[in] model_buffer モデルバッファ
     */
    virtual void drawModel(const IModelBuffer& model_buffer) = 0;
};

}
