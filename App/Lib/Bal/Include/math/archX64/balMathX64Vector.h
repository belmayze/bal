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

class MathX64Vector2 : public MathCommonVector2
{
public:
    //! コンストラクター
    using MathCommonVector2::MathCommonVector2;

    //! キャスト
    MathX64Vector2(const MathCommonVector2& v)
    {
        *this = v;
    }
};

class MathX64Vector3 : public MathCommonVector3
{
public:
    //! コンストラクター
    using MathCommonVector3::MathCommonVector3;

    //! キャスト
    MathX64Vector3(const MathCommonVector3& v)
    {
        m[0] = v.getX();
        m[1] = v.getY();
        m[2] = v.getZ();
    }
};

class alignas(16) MathX64Vector4 : public MathCommonVector4
{
public:
    //! コンストラクター
    using MathCommonVector4::MathCommonVector4;
    
    //! キャスト
    MathX64Vector4(const MathCommonVector4& v)
    {
        *this = v;
    }
};

}

#include <math/archX64/balMathX64Vector_inl.h>
