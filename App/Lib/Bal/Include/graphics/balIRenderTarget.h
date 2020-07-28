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

namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class ITexture; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class IRenderTarget
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*                  mpGraphics = nullptr;
        ITexture* mpTexture;
    };

public:
    //! デストラクター
    virtual ~IRenderTarget() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

public:
    //! 設定されているテクスチャーを取得
    const ITexture* getTexture() const { return mpTexture; }

protected:
    ITexture* mpTexture = nullptr;
};

class IRenderTargetColor : public IRenderTarget
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

class IRenderTargetDepth : public IRenderTarget
{
public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
