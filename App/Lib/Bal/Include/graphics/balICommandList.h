/*!
 * @file   balICommandList.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class IModelBuffer; }
namespace bal::gfx { class IPipeline; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class ICommandList
{
public:
    struct InitializeArg
    {
        IGraphics* mpGraphics = nullptr;
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
     * パイプラインの設定
     * @param[in] pipeline パイプライン
     */
    virtual void bindPipeline(const IPipeline& pipeline) = 0;

    /*!
     * モデルを描画します
     * @param[in] model_buffer モデルバッファ
     */
    virtual void drawModel(const IModelBuffer& model_buffer) = 0;
};

}
