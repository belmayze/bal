/*!
 * @file   balMathWin64Vector.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMathCommonVector.h>

// ----------------------------------------------------------------------------
namespace bal {

class MathWin64Vector2 : public MathCommonVector2
{
public:
    //! コンストラクター
    using MathCommonVector2::MathCommonVector2;

    //! キャスト
    MathWin64Vector2(const MathCommonVector2& v)
    {
        *this = v;
    }
};

class MathWin64Vector3 : public MathCommonVector3
{
public:
    //! コンストラクター
    using MathCommonVector3::MathCommonVector3;

    //! キャスト
    MathWin64Vector3(const MathCommonVector3& v)
    {
        *this = v;
    }
};

class alignas(16) MathWin64Vector4 : public MathCommonVector4
{
public:
    //! コンストラクター
    using MathCommonVector4::MathCommonVector4;
    
    //! キャスト
    MathWin64Vector4(const MathCommonVector4& v)
    {
        *this = v;
    }
};

}

#include <math/win64/balMathWin64Vector_inl.h>
