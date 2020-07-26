/*!
 * @file   balMathWin64Matrix.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
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
    inline void setTranspose();

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    /*! 行列同士の乗算 */
    inline MathWin64Matrix44 operator*(const MathWin64Matrix44& rhs) const;
    /*! ベクトルとの乗算 */
    inline MathWin64Vector4 operator*(const MathWin64Vector4& rhs) const;
}; 

}

#include <math/win64/balMathWin64Matrix_inl.h>
