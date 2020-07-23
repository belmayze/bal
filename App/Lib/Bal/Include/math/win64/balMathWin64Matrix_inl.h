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
    tmp0 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[0]), *reinterpret_cast<__m128*>(m[1]), _MM_SHUFFLE(1, 0, 1, 0)); // 1010
    // m[0][0] m[0][1] m[1][0] m[1][1]
    tmp2 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[0]), *reinterpret_cast<__m128*>(m[1]), _MM_SHUFFLE(3, 2, 3, 2)); // 3232
    // m[0][2] m[0][3] m[1][2] m[1][3]
    tmp1 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[2]), *reinterpret_cast<__m128*>(m[3]), _MM_SHUFFLE(1, 0, 1, 0)); // 1010
    // m[2][0] m[2][1] m[3][0] m[3][1]
    tmp3 = _mm_shuffle_ps(*reinterpret_cast<__m128*>(m[2]), *reinterpret_cast<__m128*>(m[3]), _MM_SHUFFLE(3, 2, 3, 2)); // 3232
    // m[2][2] m[2][3] m[3][2] m[3][3]

    *reinterpret_cast<__m128*>(m[0]) = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(2, 0, 2, 0)); // 2020
    // m[0][0] m[1][0] m[2][0] m[3][0]
    *reinterpret_cast<__m128*>(m[1]) = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3, 1, 3, 1)); // 3131
    // m[0][1] m[1][1] m[2][1] m[3][1]
    *reinterpret_cast<__m128*>(m[2]) = _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(2, 0, 2, 0)); // 2020
    // m[0][2] m[1][2] m[2][2] m[3][2]
    *reinterpret_cast<__m128*>(m[3]) = _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(3, 1, 3, 1)); // 3131
    // m[0][3] m[1][3] m[2][3] m[3][3]
}

// ------------------------------------------------------------------------
// operator -+*/
// ------------------------------------------------------------------------

inline MathWin64Matrix44 MathWin64Matrix44::operator*(const MathWin64Matrix44& rhs) const
{
    MathWin64Matrix44 result;

    // 倒置
    __m128 row0, row1, row2, row3;
    {
        __m128 tmp0, tmp1, tmp2, tmp3;

        tmp0 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[0]), *reinterpret_cast<const __m128*>(rhs.m[1]), _MM_SHUFFLE(1, 0, 1, 0));
        tmp2 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[0]), *reinterpret_cast<const __m128*>(rhs.m[1]), _MM_SHUFFLE(3, 2, 3, 2));
        tmp1 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[2]), *reinterpret_cast<const __m128*>(rhs.m[3]), _MM_SHUFFLE(1, 0, 1, 0));
        tmp3 = _mm_shuffle_ps(*reinterpret_cast<const __m128*>(rhs.m[2]), *reinterpret_cast<const __m128*>(rhs.m[3]), _MM_SHUFFLE(3, 2, 3, 2));

        row0 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(2, 0, 2, 0));
        row1 = _mm_shuffle_ps(tmp0, tmp1, _MM_SHUFFLE(3, 1, 3, 1));
        row2 = _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(2, 0, 2, 0));
        row3 = _mm_shuffle_ps(tmp2, tmp3, _MM_SHUFFLE(3, 1, 3, 1));
    }
    // 内積
    {
        result.m[0][0] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[0]), row0, 0xff).m128_f32[0];
        result.m[0][1] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[0]), row1, 0xff).m128_f32[0];
        result.m[0][2] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[0]), row2, 0xff).m128_f32[0];
        result.m[0][3] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[0]), row3, 0xff).m128_f32[0];

        result.m[1][0] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[1]), row0, 0xff).m128_f32[0];
        result.m[1][1] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[1]), row1, 0xff).m128_f32[0];
        result.m[1][2] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[1]), row2, 0xff).m128_f32[0];
        result.m[1][3] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[1]), row3, 0xff).m128_f32[0];

        result.m[2][0] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[2]), row0, 0xff).m128_f32[0];
        result.m[2][1] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[2]), row1, 0xff).m128_f32[0];
        result.m[2][2] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[2]), row2, 0xff).m128_f32[0];
        result.m[2][3] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[2]), row3, 0xff).m128_f32[0];

        result.m[3][0] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[3]), row0, 0xff).m128_f32[0];
        result.m[3][1] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[3]), row1, 0xff).m128_f32[0];
        result.m[3][2] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[3]), row2, 0xff).m128_f32[0];
        result.m[3][3] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[3]), row3, 0xff).m128_f32[0];
    }
    return result;
}

// ----------------------------------------------------------------------------

inline MathWin64Vector4 MathWin64Matrix44::operator*(const MathWin64Vector4& rhs) const
{
    MathWin64Vector4 result;

    result[0] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[0]), *reinterpret_cast<const __m128*>(static_cast<const float*>(rhs)), 0xff).m128_f32[0];
    result[1] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[1]), *reinterpret_cast<const __m128*>(static_cast<const float*>(rhs)), 0xff).m128_f32[0];
    result[2] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[2]), *reinterpret_cast<const __m128*>(static_cast<const float*>(rhs)), 0xff).m128_f32[0];
    result[3] = _mm_dp_ps(*reinterpret_cast<const __m128*>(m[3]), *reinterpret_cast<const __m128*>(static_cast<const float*>(rhs)), 0xff).m128_f32[0];

    return result;
}

// ----------------------------------------------------------------------------

}
