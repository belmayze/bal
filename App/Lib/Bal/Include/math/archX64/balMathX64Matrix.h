/*!
 * @file   balMathX64Matrix.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMathCommonMatrix.h>
#include <math/archX64/balMathX64Vector.h>

// ----------------------------------------------------------------------------
namespace bal {

class alignas(16) MathX64Matrix44 : public MathCommonMatrix44
{
public:
    //! コンストラクター
    using MathCommonMatrix44::MathCommonMatrix44;

    /*!
     * 行列を倒置にします
     */
    inline void setTranspose();

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*! 行列同士の乗算 */
    inline MathX64Matrix44 operator*(const MathX64Matrix44& rhs) const;
    /*! ベクトルとの乗算 */
    inline MathX64Vector4 operator*(const MathX64Vector4& rhs) const;
}; 

class MathX64Matrix34 : public MathCommonMatrix34
{
public:
    //! コンストラクター
    using MathCommonMatrix34::MathCommonMatrix34;
};

}

#include <math/archX64/balMathX64Matrix_inl.h>
