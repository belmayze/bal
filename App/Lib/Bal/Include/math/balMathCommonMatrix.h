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
#include <math/balMath.h>
#include <math/balMathCommonVector.h>

// ----------------------------------------------------------------------------
namespace bal {

class alignas(16) MathCommonMatrix44
{
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
     * スケールをセットします
     * @param[in] scale スケール値
     */
    inline void setScale(const MathVector3& scale);

    /*!
     * 回転をセットします
     * @note Z * Y * X を計算します
     */
    inline void setRotate(const Radian& x, const Radian& y, const Radian& z);

    /*!
     * ヨー・ピッチ・ロールをセットします
     * @note Z * X * Y を計算します
     */
    inline void setYawPitchRoll(const Radian& yaw, const Radian& pitch, const Radian& roll);

    /*!
     * 平行移動をセットします
     * @param[in] translate 平行移動量
     */
    inline void setTranslate(const MathVector3& translate);

    /*!
     * 行列を倒置にします
     */
    inline void setTranspose();

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*! 行列同士の乗算 */
    inline MathCommonMatrix44 operator*(const MathCommonMatrix44& rhs) const;

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    const float* operator[](int row) const { return m[row]; }
          float* operator[](int row)       { return m[row]; }

protected:
    float m[4][4];
};

}

#include <math/balMathCommonMatrix_inl.h>
