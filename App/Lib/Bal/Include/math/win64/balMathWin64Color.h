/*!
 * @file   balMathWin64Color.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMathCommonColor.h>

// ----------------------------------------------------------------------------
namespace bal {

class MathWin64Color : public MathCommonColor
{
public:
    //! コンストラクター
    using MathCommonColor::MathCommonColor;

    //! キャスト
    MathWin64Color(const MathCommonColor& color)
    {
        *this = color;
    }
};

}
