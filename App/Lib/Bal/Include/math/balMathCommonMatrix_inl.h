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

inline void MathCommonMatrix44::setRotateX(const Radian& radian)
{
    float sin = Math::Sin(radian);
    float cos = Math::Cos(radian);
    m[0][0] = 1.f; m[0][1] = 0.f; m[0][2] =  0.f;
    m[1][0] = 0.f; m[1][1] = cos; m[1][2] = -sin;
    m[2][0] = 0.f; m[2][1] = sin; m[2][2] =  cos;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setRotateY(const Radian& radian)
{
    float sin = Math::Sin(radian);
    float cos = Math::Cos(radian);
    m[0][0] =  cos; m[0][1] = 0.f; m[0][2] = sin;
    m[1][0] =  0.f; m[1][1] = 1.f; m[1][2] = 0.f;
    m[2][0] = -sin; m[2][1] = 0.f; m[2][2] = cos;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setRotateZ(const Radian& radian)
{
    float sin = Math::Sin(radian);
    float cos = Math::Cos(radian);
    m[0][0] = cos; m[0][1] = -sin; m[0][2] = 0.f;
    m[1][0] = sin; m[1][1] =  cos; m[1][2] = 0.f;
    m[2][0] = 0.f; m[2][1] =  0.f; m[2][2] = 1.f;
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

inline void MathCommonMatrix44::setLookAtRH(const MathVector3& eye_pt, const MathVector3& look_at, const MathVector3& up)
{
    // カメラの姿勢ベクトルを計算する
    MathVector3 x, y, z;
    z = (eye_pt - look_at).calcNormalize(); // 視線ベクトル
    y = up.calcNormalize();
    x = y.calcCross(z);
    y = z.calcCross(x); // 上方向ベクトルを再計算

    // 行列をセットします
    // View 行列は、姿勢ベクトルは逆行列（倒置）になり、移動量は反転する
    m[0][0] = x.getX(); m[0][1] = x.getY(); m[0][2] = x.getZ(); m[0][3] = -x.calcDot(eye_pt);
    m[1][0] = y.getX(); m[1][1] = y.getY(); m[1][2] = y.getZ(); m[1][3] = -y.calcDot(eye_pt);
    m[2][0] = z.getX(); m[2][1] = z.getY(); m[2][2] = z.getZ(); m[2][3] = -z.calcDot(eye_pt);
    m[3][0] = 0.f;      m[3][1] = 0.f;      m[3][2] = 0.f;      m[3][3] = 0.f;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setPerspectiveOrthoRH(float width, float height, float z_near, float z_far)
{
    float n = 1.f / (z_far - z_near); // 奥行の長さを 1 に正規化する係数

    m[0][0] = 2.f / width; m[0][1] = 0.f;          m[0][2] = 0.f; m[0][3] = 0.f;
    m[1][0] = 0.f;         m[1][1] = 2.f / height; m[1][2] = 0.f; m[1][3] = 0.f;
    m[2][0] = 0.f;         m[2][1] = 0.f;          m[2][2] = n;   m[2][3] = -z_near * n;
    m[3][0] = 0.f;         m[3][1] = 0.f;          m[3][2] = 0.f; m[3][3] = 1.f;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setPerspectiveProjectionRH(const Radian& fovy, float aspect, float z_near, float z_far)
{
    float f = 1.f / Math::Tan(fovy * 0.5f); // 距離 = 1 の時の Y 方向の大きさ
    float n = 1.f / (z_far - z_near);       // 奥行の長さを 1 に正規化する係数

    m[0][0] = f / aspect; m[0][1] = 0.f; m[0][2] =  0.f;       m[0][3] =  0.f;
    m[1][0] = 0.f;        m[1][1] = f;   m[1][2] =  0.f;       m[1][3] =  0.f;
    m[2][0] = 0.f;        m[2][1] = 0.f; m[2][2] = -z_far * n; m[2][3] = -z_far * z_near * n;
    m[3][0] = 0.f;        m[3][1] = 0.f; m[3][2] = -1.f;       m[3][3] =  0.f;
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setCol(int index, const MathVector4& v)
{
    std::memcpy(m[index], static_cast<const float*>(v), sizeof(float) * 4);
}

// ------------------------------------------------------------------------

inline void MathCommonMatrix44::setRow(int index, const MathVector4& v)
{
    m[0][index] = v[0];
    m[1][index] = v[1];
    m[2][index] = v[2];
    m[3][index] = v[3];
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

inline MathCommonVector4 MathCommonMatrix44::operator*(const MathCommonVector4& rhs) const
{
    MathCommonVector4 result;
    result[0] = m[0][0] * rhs[0] + m[0][1] * rhs[1] + m[0][2] * rhs[2] + m[0][3] * rhs[3];
    result[1] = m[1][0] * rhs[0] + m[1][1] * rhs[1] + m[1][2] * rhs[2] + m[1][3] * rhs[3];
    result[2] = m[2][0] * rhs[0] + m[2][1] * rhs[1] + m[2][2] * rhs[2] + m[2][3] * rhs[3];
    result[3] = m[3][0] * rhs[0] + m[3][1] * rhs[1] + m[3][2] * rhs[2] + m[3][3] * rhs[3];
    return result;
}

// ------------------------------------------------------------------------

}
