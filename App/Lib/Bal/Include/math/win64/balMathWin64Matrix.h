/*!
 * @file   balMathWin64Matrix.h
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
#include <math/win64/balMathWin64Vector.h>

// ----------------------------------------------------------------------------
namespace bal {

class alignas(16) MathWin64Matrix44 : public MathCommonMatrix44
{
public:
    //! コンストラクター
    using MathCommonMatrix44::MathCommonMatrix44;

    /*!
     * 行列を倒置にします
     */
    inline void setTranspose()
    {
        __m128 tmp0, tmp1, tmp2, tmp3;
        tmp0 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[0]), *reinterpret_cast<__m128*>(m[1]), 0x44);
        tmp2 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[0]), *reinterpret_cast<__m128*>(m[1]), 0xEE);
        tmp1 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[2]), *reinterpret_cast<__m128*>(m[3]), 0x44);
        tmp3 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[2]), *reinterpret_cast<__m128*>(m[3]), 0xEE);

        *reinterpret_cast<__m128*>(m[0]) = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        *reinterpret_cast<__m128*>(m[1]) = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
        *reinterpret_cast<__m128*>(m[2]) = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        *reinterpret_cast<__m128*>(m[3]) = _mm_shuffle_ps(tmp2, tmp3, 0xDD);
    }

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*!
     * 行列同士の乗算
     */
    MathWin64Matrix44 operator*(const MathWin64Matrix44& rhs) const
    {
        // まずは行列を倒置
        __m128 tmp0, tmp1, tmp2, tmp3;
        tmp0 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[0]), *reinterpret_cast<const __m128*>(rhs.m[1]), 0x44);
        tmp2 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[0]), *reinterpret_cast<const __m128*>(rhs.m[1]), 0xEE);
        tmp1 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[2]), *reinterpret_cast<const __m128*>(rhs.m[3]), 0x44);
        tmp3 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[2]), *reinterpret_cast<const __m128*>(rhs.m[3]), 0xEE);

        __m128 row0, row1, row2, row3;
        row0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        row1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
        row2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        row3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);

        // 乗算 + 加算
        MathWin64Matrix44 result;
        alignas(16) float tmp[4];

        tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row0);
        tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row1);
        tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row2);
        tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row3);
        _mm_store_ps(tmp, tmp0); result.m[0][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[0][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[0][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[0][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row0);
        tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row1);
        tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row2);
        tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row3);
        _mm_store_ps(tmp, tmp0); result.m[1][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[1][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[1][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[1][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row0);
        tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row1);
        tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row2);
        tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row3);
        _mm_store_ps(tmp, tmp0); result.m[2][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[2][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[2][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[2][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row0);
        tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row1);
        tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row2);
        tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row3);
        _mm_store_ps(tmp, tmp0); result.m[3][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[3][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[3][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[3][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        return result;
    }
}; 

}
