/*!
 * @file   balMathWin64Vector.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// std
#include <intrin.h>
// bal
#include <math/balMathCommonVector.h>

// ----------------------------------------------------------------------------
namespace bal {

class alignas(16) MathWin64Vector4 : public MathCommonVector4
{
public:
    //! コンストラクター
    using MathCommonVector4::MathCommonVector4;

};

class MathWin64Vector3 : public MathCommonVector3
{
public:
    //! コンストラクター
    using MathCommonVector3::MathCommonVector3;
};

}
