﻿/*!
 * @file   balMathCommonVector_inl.h
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

// ----------------------------------------------------------------------------
// Vector3
// ----------------------------------------------------------------------------

inline float MathCommonVector3::setNormalize()
{
    float length     = calcLength();
    float length_inv = 1.f / length;

    m[0] *= length_inv; m[1] *= length_inv; m[2] *= length_inv;
    return length;
}

// ----------------------------------------------------------------------------

inline float MathCommonVector3::calcLength() const
{
    return Math::Sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
}

// ----------------------------------------------------------------------------

inline MathCommonVector3 MathCommonVector3::calcNormalize() const
{
    MathCommonVector3 v = *this;
    v.setNormalize();
    return v;
}


// ----------------------------------------------------------------------------

inline float MathCommonVector3::calcDot(const MathCommonVector3& v) const
{
    return m[0] * v.m[0] + m[1] * v.m[1] + m[2] * v.m[2];
}

// ----------------------------------------------------------------------------

inline MathCommonVector3 MathCommonVector3::calcCross(const MathCommonVector3& v) const
{
    return MathCommonVector3(
        m[1] * v.m[2] - m[2] * v.m[1],
        m[2] * v.m[0] - m[0] * v.m[2],
        m[0] * v.m[1] - m[1] * v.m[0]
    );
}

// ----------------------------------------------------------------------------
// operator -+*/
// ----------------------------------------------------------------------------

MathCommonVector3 MathCommonVector3::operator +(const MathCommonVector3& rhs) const
{
    return MathCommonVector3(
        m[0] + rhs.m[0],
        m[1] + rhs.m[1],
        m[2] + rhs.m[2]
    );
}

// ----------------------------------------------------------------------------

MathCommonVector3 MathCommonVector3::operator -(const MathCommonVector3& rhs) const
{
    return MathCommonVector3(
        m[0] - rhs.m[0],
        m[1] - rhs.m[1],
        m[2] - rhs.m[2]
    );
}

// ----------------------------------------------------------------------------

const MathCommonVector3& MathCommonVector3::operator +=(const MathCommonVector3& rhs)
{
    m[0] += rhs.m[0]; m[1] += rhs.m[1]; m[2] += rhs.m[2];
    return *this;
}

// ----------------------------------------------------------------------------

const MathCommonVector3& MathCommonVector3::operator -=(const MathCommonVector3& rhs)
{
    m[0] -= rhs.m[0]; m[1] -= rhs.m[1]; m[2] -= rhs.m[2];
    return *this;
}

// ----------------------------------------------------------------------------

}
