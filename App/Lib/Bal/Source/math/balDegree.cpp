/*!
 * @file   balDegree.cpp
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

Degree::Degree(const Radian& radian)
    : mValue(radian.toDegree()) {}

// ----------------------------------------------------------------------------

Radian Degree::toRadian() const
{
    return Radian(mValue * (Math::Pi() / 180.f));
}

}
