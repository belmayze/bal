/*!
 * @file   balMathWin64Matrix_inl.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/win64/balMathWin64Matrix_inl.h>

namespace bal
{

// ----------------------------------------------------------------------------

inline void MathWin64Matrix44::setTranspose()
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

inline MathWin64Matrix44 MathWin64Matrix44::operator*(const MathWin64Matrix44& rhs) const
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

    tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row0);
    tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row1);
    tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row2);
    tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), row3);
    result.m[0][0] = tmp0.m128_f32[0] + tmp0.m128_f32[1] + tmp0.m128_f32[2] + tmp0.m128_f32[3];
    result.m[0][1] = tmp1.m128_f32[0] + tmp1.m128_f32[1] + tmp1.m128_f32[2] + tmp1.m128_f32[3];
    result.m[0][2] = tmp2.m128_f32[0] + tmp2.m128_f32[1] + tmp2.m128_f32[2] + tmp2.m128_f32[3];
    result.m[0][3] = tmp3.m128_f32[0] + tmp3.m128_f32[1] + tmp3.m128_f32[2] + tmp3.m128_f32[3];

    tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row0);
    tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row1);
    tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row2);
    tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), row3);
    result.m[1][0] = tmp0.m128_f32[0] + tmp0.m128_f32[1] + tmp0.m128_f32[2] + tmp0.m128_f32[3];
    result.m[1][1] = tmp1.m128_f32[0] + tmp1.m128_f32[1] + tmp1.m128_f32[2] + tmp1.m128_f32[3];
    result.m[1][2] = tmp2.m128_f32[0] + tmp2.m128_f32[1] + tmp2.m128_f32[2] + tmp2.m128_f32[3];
    result.m[1][3] = tmp3.m128_f32[0] + tmp3.m128_f32[1] + tmp3.m128_f32[2] + tmp3.m128_f32[3];

    tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row0);
    tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row1);
    tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row2);
    tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), row3);
    result.m[2][0] = tmp0.m128_f32[0] + tmp0.m128_f32[1] + tmp0.m128_f32[2] + tmp0.m128_f32[3];
    result.m[2][1] = tmp1.m128_f32[0] + tmp1.m128_f32[1] + tmp1.m128_f32[2] + tmp1.m128_f32[3];
    result.m[2][2] = tmp2.m128_f32[0] + tmp2.m128_f32[1] + tmp2.m128_f32[2] + tmp2.m128_f32[3];
    result.m[2][3] = tmp3.m128_f32[0] + tmp3.m128_f32[1] + tmp3.m128_f32[2] + tmp3.m128_f32[3];

    tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row0);
    tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row1);
    tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row2);
    tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), row3);
    result.m[3][0] = tmp0.m128_f32[0] + tmp0.m128_f32[1] + tmp0.m128_f32[2] + tmp0.m128_f32[3];
    result.m[3][1] = tmp1.m128_f32[0] + tmp1.m128_f32[1] + tmp1.m128_f32[2] + tmp1.m128_f32[3];
    result.m[3][2] = tmp2.m128_f32[0] + tmp2.m128_f32[1] + tmp2.m128_f32[2] + tmp2.m128_f32[3];
    result.m[3][3] = tmp3.m128_f32[0] + tmp3.m128_f32[1] + tmp3.m128_f32[2] + tmp3.m128_f32[3];

    return result;
}

// ----------------------------------------------------------------------------

inline MathWin64Vector4 MathWin64Matrix44::operator*(const MathWin64Vector4& rhs) const
{
    __m128 tmp0, tmp1, tmp2, tmp3;
    MathWin64Vector4 result;
    const float* p_rhs = static_cast<const float*>(rhs);

    tmp0 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[0]), *reinterpret_cast<const __m128*>(p_rhs));
    tmp1 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[1]), *reinterpret_cast<const __m128*>(p_rhs));
    tmp2 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[2]), *reinterpret_cast<const __m128*>(p_rhs));
    tmp3 = _mm_mul_ps(*reinterpret_cast<const __m128*>(m[3]), *reinterpret_cast<const __m128*>(p_rhs));

    result[0] = tmp0.m128_f32[0] + tmp0.m128_f32[1] + tmp0.m128_f32[2] + tmp0.m128_f32[3];
    result[1] = tmp1.m128_f32[0] + tmp1.m128_f32[1] + tmp1.m128_f32[2] + tmp1.m128_f32[3];
    result[2] = tmp2.m128_f32[0] + tmp2.m128_f32[1] + tmp2.m128_f32[2] + tmp2.m128_f32[3];
    result[3] = tmp3.m128_f32[0] + tmp3.m128_f32[1] + tmp3.m128_f32[2] + tmp3.m128_f32[3];

    return result;
}

// ----------------------------------------------------------------------------

}
