/*!
 * @file   balIGraphics.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

// ----------------------------------------------------------------------------
namespace bal::gfx {

class IGraphics
{
public:
    struct InitializeArg
    {
        HWND     mHwnd;
        MathSize mRenderBufferSize;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

    /*!
     * 破棄の処理を記述します
     */
    virtual bool destroy() = 0;
};

}
