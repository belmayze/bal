/*!
 * @file   balMathWin64Size.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// std
#include <intrin.h>
// bal
#include <math/balMathCommonSize.h>

// ----------------------------------------------------------------------------
namespace bal {

class MathWin64Size : public MathCommonSize
{
public:
    //! コンストラクター
    using MathCommonSize::MathCommonSize;

    //! キャスト
    MathWin64Size(const MathCommonSize& s)
    {
        *this = s;
    }
};

}
