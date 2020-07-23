/*!
 * @file   balMathCommonSize.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

// ----------------------------------------------------------------------------
namespace bal {

// ----------------------------------------------------------------------------

class MathCommonSize
{
public:
    //! コンストラクター
    MathCommonSize() {}

    //! コンストラクター
    MathCommonSize(int width, int height) :mWidth(width), mHeight(height) {}

    //! 横幅
    inline void setWidth(int width) { mWidth = width; }
    inline int  getWidth() const    { return mWidth; }

    //! 高さ
    inline void setHeight(int height) { mHeight = height; }
    inline int  getHeight() const     { return mHeight; }

private:
    int mWidth  = 0;
    int mHeight = 0;
};

}
