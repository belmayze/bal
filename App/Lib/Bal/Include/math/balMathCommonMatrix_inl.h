/*!
 * @file   balMathCommonMatrix_inl.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMathCommonMatrix.h>

// ----------------------------------------------------------------------------
namespace bal {

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setScale(const MathVector3& scale)
{
    m[0][0] = scale.getX(); m[0][1] = 0.f;          m[0][2] = 0.f;
    m[1][0] = 0.f;          m[1][1] = scale.getY(); m[1][2] = 0.f;
    m[2][0] = 0.f;          m[2][1] = 0.f;          m[2][2] = scale.getZ();
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setRotate(const Radian& x, const Radian& y, const Radian& z)
{
    float sinX = Math::Sin(x), sinY = Math::Sin(y), sinZ = Math::Sin(z);
    float cosX = Math::Cos(x), cosY = Math::Cos(y), cosZ = Math::Cos(z);

    //         | cosZ -sinZ 0 | |  cosY 0 sinY | | 1  0     0   |
    // ZYX   = | sinZ  cosZ 0 | |   0   1  0   | | 0 cosX -sinX |
    //         |  0     0   1 | | -sinY 0 cosY | | 0 sinX  cosX |
    //
    //         | cosYcosZ -sinZ sinYcosZ | | 1  0     0   |
    // (ZY)X = | cosYsinZ  cosZ sinYsinZ | | 0 cosX -sinX |
    //         |  -sinY     0     cosY   | | 0 sinX  cosX |
    //
    //         | cosYcosZ sinXsinYcosZ-cosXsinZ cosXsinYcosZ+sinXsinZ |
    // (ZYX) = | cosYsinZ sinXsinYsinZ+cosXcosZ cosXsinYsinZ-sinXcosZ |
    //         |  -sinY           sinXcosY              cosXcosY      |

    m[0][0] = cosY * cosZ; m[0][1] = sinX * sinY * cosZ - cosX * sinZ; m[0][2] = cosX * sinY * cosZ + sinX * sinZ;
    m[1][0] = cosY * sinZ; m[1][1] = sinX * sinY * sinZ + cosX * cosZ; m[1][2] = cosX * sinY * sinZ - sinX * cosZ;
    m[2][0] =       -sinY; m[2][1] =                      sinX * cosY; m[2][2] =                      cosX * cosY;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setYawPitchRoll(const Radian& yaw, const Radian& pitch, const Radian& roll)
{
    float sinX = Math::Sin(pitch), sinY = Math::Sin(yaw), sinZ = Math::Sin(roll);
    float cosX = Math::Cos(pitch), cosY = Math::Cos(yaw), cosZ = Math::Cos(roll);

    //         | cosZ -sinZ 0 | | 1  0     0   | |  cosY 0 sinY |
    // ZXY   = | sinZ  cosZ 0 | | 0 cosX -sinX | |   0   1  0   |
    //         |  0     0   1 | | 0 sinX  cosX | | -sinY 0 cosY |
    //
    //         | cosZ -cosXsinZ  sinXsinZ | |  cosY 0 sinY |
    // (ZX)Y = | sinZ  cosXcosZ -sinXcosZ | |   0   1  0   |
    //         |  0      sinX      cosX   | | -sinY 0 cosY |
    //
    //         | cosYcosZ-sinXsinYsinZ -cosXsinZ sinYcosZ+sinXcosYsinZ |
    // (ZXY) = | cosYsinZ+sinXsinYcosZ  cosXcosZ sinYsinZ-sinXcosYcosZ |
    //         |      -cosXsinY           sinX         cosXcosY        |
        
    m[0][0] = cosY * cosZ - sinX * sinY * sinZ; m[0][1] = -cosX * sinZ; m[0][2] = sinY * cosZ + sinX * cosY * sinZ;
    m[1][0] = cosY * sinZ + sinX * sinY * cosZ; m[1][1] =  cosX * cosZ; m[1][2] = sinY * sinZ - sinX * cosY * cosZ;
    m[2][0] =                     -cosX * sinY; m[2][1] =         sinX; m[2][2] =                      cosX * cosY;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setTranslate(const MathVector3& translate)
{
    m[0][3] = translate.getX(); m[1][3] = translate.getY(); m[2][3] = translate.getZ();
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setTranspose()
{
    float tmp;

    tmp = m[0][1]; m[0][1] = m[1][0]; m[1][0] = tmp;
    tmp = m[0][2]; m[0][2] = m[2][0]; m[2][0] = tmp;
    tmp = m[0][3]; m[0][3] = m[3][0]; m[3][0] = tmp;

    tmp = m[1][2]; m[1][2] = m[2][1]; m[2][1] = tmp;
    tmp = m[1][3]; m[1][3] = m[3][1]; m[3][1] = tmp;

    tmp = m[2][3]; m[2][3] = m[3][2]; m[3][2] = tmp;
}

// ------------------------------------------------------------------------
// operator -+*/
// ------------------------------------------------------------------------
inline MathCommonMatrix44 MathCommonMatrix44::operator*(const MathCommonMatrix44& rhs) const
{
    MathCommonMatrix44 result;
    result.m[0][0] = m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0] + m[0][2] * rhs.m[2][0] + m[0][3] * rhs.m[3][0];
    result.m[0][1] = m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1] + m[0][2] * rhs.m[2][1] + m[0][3] * rhs.m[3][1];
    result.m[0][2] = m[0][0] * rhs.m[0][2] + m[0][1] * rhs.m[1][2] + m[0][2] * rhs.m[2][2] + m[0][3] * rhs.m[3][2];
    result.m[0][3] = m[0][0] * rhs.m[0][3] + m[0][1] * rhs.m[1][3] + m[0][2] * rhs.m[2][3] + m[0][3] * rhs.m[3][3];

    result.m[1][0] = m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0] + m[1][2] * rhs.m[2][0] + m[1][3] * rhs.m[3][0];
    result.m[1][1] = m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1] + m[1][2] * rhs.m[2][1] + m[1][3] * rhs.m[3][1];
    result.m[1][2] = m[1][0] * rhs.m[0][2] + m[1][1] * rhs.m[1][2] + m[1][2] * rhs.m[2][2] + m[1][3] * rhs.m[3][2];
    result.m[1][3] = m[1][0] * rhs.m[0][3] + m[1][1] * rhs.m[1][3] + m[1][2] * rhs.m[2][3] + m[1][3] * rhs.m[3][3];

    result.m[2][0] = m[2][0] * rhs.m[0][0] + m[2][1] * rhs.m[1][0] + m[2][2] * rhs.m[2][0] + m[2][3] * rhs.m[3][0];
    result.m[2][1] = m[2][0] * rhs.m[0][1] + m[2][1] * rhs.m[1][1] + m[2][2] * rhs.m[2][1] + m[2][3] * rhs.m[3][1];
    result.m[2][2] = m[2][0] * rhs.m[0][2] + m[2][1] * rhs.m[1][2] + m[2][2] * rhs.m[2][2] + m[2][3] * rhs.m[3][2];
    result.m[2][3] = m[2][0] * rhs.m[0][3] + m[2][1] * rhs.m[1][3] + m[2][2] * rhs.m[2][3] + m[2][3] * rhs.m[3][3];

    result.m[3][0] = m[3][0] * rhs.m[0][0] + m[3][1] * rhs.m[1][0] + m[3][2] * rhs.m[2][0] + m[3][3] * rhs.m[3][0];
    result.m[3][1] = m[3][0] * rhs.m[0][1] + m[3][1] * rhs.m[1][1] + m[3][2] * rhs.m[2][1] + m[3][3] * rhs.m[3][1];
    result.m[3][2] = m[3][0] * rhs.m[0][2] + m[3][1] * rhs.m[1][2] + m[3][2] * rhs.m[2][2] + m[3][3] * rhs.m[3][2];
    result.m[3][3] = m[3][0] * rhs.m[0][3] + m[3][1] * rhs.m[1][3] + m[3][2] * rhs.m[2][3] + m[3][3] * rhs.m[3][3];
    return result;
}

// ------------------------------------------------------------------------

}
