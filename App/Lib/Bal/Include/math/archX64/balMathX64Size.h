/*!
 * @file   balMathX64Size.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMathCommonSize.h>

// ----------------------------------------------------------------------------
namespace bal {

class MathX64Size : public MathCommonSize
{
public:
    //! コンストラクター
    using MathCommonSize::MathCommonSize;

    //! キャスト
    MathX64Size(const MathCommonSize& s)
    {
        *this = s;
    }
};

}
