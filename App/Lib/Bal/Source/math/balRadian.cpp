/*!
 * @file   balRadian.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <math/balDegree.h>
#include <math/balMath.h>
#include <math/balRadian.h>

namespace bal {

// ----------------------------------------------------------------------------

Radian::Radian(const Degree& degree)
    : mValue(degree.toRadian()) {}

// ----------------------------------------------------------------------------

Degree Radian::toDegree() const
{
    return Degree(mValue * (180.f / Math::Pi()));
}

}
