/*!
 * @file   balIRenderTarget.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

namespace bal { class IGraphics; }
namespace bal { class ITexture; }

// ----------------------------------------------------------------------------
namespace bal {

class IRenderTarget
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics* mpGraphics = nullptr;
    };

public:
    //! コンストラクター
    IRenderTarget();

    //! デストラクター
    virtual ~IRenderTarget();

    /*!
     * 初期化の処理を記述します
     * @param[in] arg     初期化構造体
     * @param[in] texture テクスチャー
     */
    virtual bool initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture) = 0;

public:
    //! 設定されているテクスチャーを取得
    const ITexture* getTexture() const { return mpTexture.get(); }

protected:
    std::unique_ptr<ITexture> mpTexture;
};

class IRenderTargetColor : public IRenderTarget
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg     初期化構造体
     * @param[in] texture テクスチャー
     */
    virtual bool initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture) = 0;
};

class IRenderTargetDepth : public IRenderTarget
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg     初期化構造体
     * @param[in] texture テクスチャー
     */
    virtual bool initialize(const InitializeArg& arg, std::unique_ptr<ITexture>&& texture) = 0;
};

}
