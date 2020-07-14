/*!
 * @file   balMathCommonMatrix.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// c++
#include <cmath>
// bal
#include <math/balMathCommonVector.h>

// ----------------------------------------------------------------------------
namespace bal {

class MathCommonMatrix44
{
    using Matrix44 = MathCommonMatrix44;
    using Vector3  = MathCommonVector3;

public:
    //! コンストラクター
    MathCommonMatrix44()
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
        float tmp;

        tmp = m[0][1]; m[0][1] = m[1][0]; m[1][0] = tmp;
        tmp = m[0][2]; m[0][2] = m[2][0]; m[2][0] = tmp;
        tmp = m[0][3]; m[0][3] = m[3][0]; m[3][0] = tmp;

        tmp = m[1][2]; m[1][2] = m[2][1]; m[2][1] = tmp;
        tmp = m[1][3]; m[1][3] = m[3][1]; m[3][1] = tmp;

        tmp = m[2][3]; m[2][3] = m[3][2]; m[3][2] = tmp;
    }

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*!
     * 行列同士の乗算
     */
    Matrix44 operator*(const Matrix44& rhs) const
    {
        Matrix44 result;
        result.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
        result.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
        result.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
        result.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

        result.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
        result.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
        result.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
        result.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

        result.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
        result.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
        result.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
        result.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

        result.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
        result.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
        result.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
        result.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
        return result;
    }

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    const float* operator[](int row) const { return m[row]; }
          float* operator[](int row)       { return m[row]; }

private:
    float m[4][4];
}; 

}
