/*!
 * @file   balMathX64Color.h
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

class MathX64Color : public MathCommonColor
{
public:
    //! コンストラクター
    using MathCommonColor::MathCommonColor;

    //! キャスト
    MathX64Color(const MathCommonColor& color)
    {
        *this = color;
    }
};

}
