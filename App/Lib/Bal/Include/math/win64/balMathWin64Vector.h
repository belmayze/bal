/*!
 * @file   balMathWin64Vector.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// std
#include <intrin.h>
// bal
#include <math/balMathCommonVector.h>

// ----------------------------------------------------------------------------
namespace bal {

class alignas(16) MathWin64Vector4
{
public:
    //! コンストラクター
    MathWin64Vector4()
    {
        m[0] = 0.f; m[1] = 0.f; m[2] = 0.f; m[3] = 0.f;
    }

private:
    union
    {
        float  m[4];
        __m128 mVec;
    };
};

using MathWin64Vector3 = MathCommonVector3;

}
