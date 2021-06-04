/*!
 * @file   balMathCommonColor.h
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

class MathCommonColor
{
public:
    //! コンストラクター
    constexpr MathCommonColor() {}

    //! コンストラクター
    constexpr MathCommonColor(float r, float g, float b, float a = 1.f)
    {
        mColor[0] = r; mColor[1] = g; mColor[2] = b; mColor[3] = a;
    }

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    inline operator       float* ()       { return mColor; }
    inline operator const float* () const { return mColor; }

private:
    float mColor[4] = {0.f};
};

}
