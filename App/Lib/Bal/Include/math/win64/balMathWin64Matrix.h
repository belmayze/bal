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

class alignas(16) MathWin64Matrix44
{
    using Matrix44 = MathWin64Matrix44;
    using Vector3  = MathWin64Vector3;

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
     * 回転とスケールをセットします
     */
    void setRotateScale(const Vector3& rot, const Vector3& scale)
    {
        // @TODO: rotate
        m[0][0] = scale.getX(); m[0][1] = 0.f;          m[0][2] = 0.f;
        m[1][0] = 0.f;          m[1][1] = scale.getY(); m[1][2] = 0.f;
        m[2][0] = 0.f;          m[2][1] = 0.f;          m[2][2] = scale.getZ();
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

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*!
     * 行列同士の乗算
     */
    Matrix44 operator*(const Matrix44& rhs) const
    {
        // まずは行列を倒置
        __m128 tmp0, tmp1, tmp2, tmp3;
        tmp0 = _mm_shuffle_ps(rhs.mVec[0], rhs.mVec[1], 0x44);
        tmp2 = _mm_shuffle_ps(rhs.mVec[0], rhs.mVec[1], 0xEE);
        tmp1 = _mm_shuffle_ps(rhs.mVec[2], rhs.mVec[3], 0x44);
        tmp3 = _mm_shuffle_ps(rhs.mVec[2], rhs.mVec[3], 0xEE);

        __m128 row0, row1, row2, row3;
        row0 = _mm_shuffle_ps(tmp0, tmp1, 0x88);
        row1 = _mm_shuffle_ps(tmp0, tmp1, 0xDD);
        row2 = _mm_shuffle_ps(tmp2, tmp3, 0x88);
        row3 = _mm_shuffle_ps(tmp2, tmp3, 0xDD);

        // 乗算 + 加算
        Matrix44 result;
        alignas(32) float tmp[4];

        tmp0 = _mm_mul_ps(mVec[0], row0);
        tmp1 = _mm_mul_ps(mVec[0], row1);
        tmp2 = _mm_mul_ps(mVec[0], row2);
        tmp3 = _mm_mul_ps(mVec[0], row3);
        _mm_store_ps(tmp, tmp0); result.m[0][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[0][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[0][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[0][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        tmp0 = _mm_mul_ps(mVec[1], row0);
        tmp1 = _mm_mul_ps(mVec[1], row1);
        tmp2 = _mm_mul_ps(mVec[1], row2);
        tmp3 = _mm_mul_ps(mVec[1], row3);
        _mm_store_ps(tmp, tmp0); result.m[1][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[1][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[1][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[1][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        tmp0 = _mm_mul_ps(mVec[2], row0);
        tmp1 = _mm_mul_ps(mVec[2], row1);
        tmp2 = _mm_mul_ps(mVec[2], row2);
        tmp3 = _mm_mul_ps(mVec[2], row3);
        _mm_store_ps(tmp, tmp0); result.m[2][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[2][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[2][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[2][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        tmp0 = _mm_mul_ps(mVec[3], row0);
        tmp1 = _mm_mul_ps(mVec[3], row1);
        tmp2 = _mm_mul_ps(mVec[3], row2);
        tmp3 = _mm_mul_ps(mVec[3], row3);
        _mm_store_ps(tmp, tmp0); result.m[3][0] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp1); result.m[3][1] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp2); result.m[3][2] = tmp[0] + tmp[1] + tmp[2] + tmp[3];
        _mm_store_ps(tmp, tmp3); result.m[3][3] = tmp[0] + tmp[1] + tmp[2] + tmp[3];

        return result;
    }

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    const float* operator[](int row) const { return m[row]; }
          float* operator[](int row)       { return m[row]; }

private:
    union
    {
        float  m[4][4];
        __m128 mVec[4];
    };
}; 

}
