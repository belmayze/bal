/*!
 * @file   balMathCommon.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// std
#include <intrin.h>
// bal
#include <math/balMathCommonMatrix.h>

// ----------------------------------------------------------------------------
namespace bal {

class alignas(16) MathWin64Matrix44
{
public:
    union
    {
        float  m[4][4];
        __m128 mVec[4];
    };

public:
    //! コンストラクター
    MathWin64Matrix44()
    {
        m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] = 0.f; m[0][3] = 0.f;
        m[1][0] = 0.f; m[1][1] = 1.f; m[1][2] = 0.f; m[1][3] = 0.f;
        m[2][0] = 0.f; m[2][1] = 0.f; m[2][2] = 1.f; m[2][3] = 0.f;
        m[3][0] = 0.f; m[3][1] = 0.f; m[3][2] = 0.f; m[3][3] = 1.f;
    }

    /*!
     * 平行移動をセットします
     */
    void setTranslate(float x, float y, float z)
    {
        m[0][3] = x; m[1][3] = y; m[2][3] = z;
    }

    /*!
     * 行列を倒置にします
     */
    void setTranspose()
    {
        __m128 tmp0, tmp1, tmp2, tmp3;
        tmp0 = _mm_shuffle_ps(mVec[0], mVec[1], 0x44);
        tmp2 = _mm_shuffle_ps(mVec[0], mVec[1], 0xEE);
        tmp1 = _mm_shuffle_ps(mVec[2], mVec[3], 0x44);
        tmp3 = _mm_shuffle_ps(mVec[2], mVec[3], 0xEE);

        mVec[0] = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        mVec[1] = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
        mVec[2] = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        mVec[3] = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
    }
}; 

}
