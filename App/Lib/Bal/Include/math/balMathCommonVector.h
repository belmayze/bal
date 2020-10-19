/*!
 * @file   balMathCommonVector.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

// ----------------------------------------------------------------------------
namespace bal {

// ----------------------------------------------------------------------------

class MathCommonVector2
{
public:
    //! コンストラクター
    MathCommonVector2()
    {
        m[0] = 0.f; m[1] = 0.f;
    }
    //! コンストラクター
    MathCommonVector2(float x, float y)
    {
        m[0] = x; m[1] = y;
    }

    /*!
     * ベクトルを正規化します
     * @return float 正規化前のベクトルの大きさを返します
     */
    inline float setNormalize();

    /*!
     * ベクトルの大きさの二乗を返します
     */
    inline float calcLengthSq() const;

    /*!
     * ベクトルの大きさを返します
     */
    inline float calcLength() const;

    /*!
     * 正規化したベクトルを返します
     * @return MathCommonVector2 正規化したベクトル
     */
    inline MathCommonVector2 calcNormalize() const;

    /*!
     * ベクトル同士の内積を求めます
     * @param[in] v ベクトル
     */
    inline float calcDot(const MathCommonVector2& v) const;

    /*!
     * ベクトル同士の外積を求めます
     * @param[in] v ベクトル
     */
    inline float calcCross(const MathCommonVector2& v) const;

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    inline MathCommonVector2 operator +() const { return *this; }
    inline MathCommonVector2 operator -() const { return MathCommonVector2(-m[0], -m[1]); }

    inline MathCommonVector2 operator +(const MathCommonVector2& rhs) const { return MathCommonVector2(m[0] + rhs[0], m[1] + rhs[1]); }
    inline MathCommonVector2 operator -(const MathCommonVector2& rhs) const { return MathCommonVector2(m[0] - rhs[0], m[1] - rhs[1]); }
    inline MathCommonVector2 operator *(const MathCommonVector2& rhs) const { return MathCommonVector2(m[0] * rhs[0], m[1] * rhs[1]); }
    inline MathCommonVector2 operator /(const MathCommonVector2& rhs) const { return MathCommonVector2(m[0] / rhs[0], m[1] / rhs[1]); }
    inline MathCommonVector2 operator *(float rhs) const { return MathCommonVector2(m[0] * rhs, m[1] * rhs); }
    inline MathCommonVector2 operator /(float rhs) const { float f_inv = 1.f / rhs; return MathCommonVector2(m[0] * f_inv, m[1] * f_inv); }

    inline MathCommonVector2& operator +=(const MathCommonVector2& rhs) { m[0] += rhs[0]; m[1] += rhs[1]; return *this; }
    inline MathCommonVector2& operator -=(const MathCommonVector2& rhs) { m[0] -= rhs[0]; m[1] -= rhs[1]; return *this; }
    inline MathCommonVector2& operator *=(const MathCommonVector2& rhs) { m[0] *= rhs[0]; m[1] *= rhs[1]; return *this; }
    inline MathCommonVector2& operator /=(const MathCommonVector2& rhs) { m[0] /= rhs[0]; m[1] /= rhs[1]; return *this; }
    inline MathCommonVector2& operator *=(float rhs) { m[0] *= rhs; m[1] *= rhs; return *this; }
    inline MathCommonVector2& operator /=(float rhs) { float f_inv = 1.f / rhs; m[0] *= f_inv; m[1] *= f_inv; return *this; }

    inline bool operator ==(const MathCommonVector2& rhs) const { return (m[0] == rhs[0] && m[1] == rhs[1]); }
    inline bool operator !=(const MathCommonVector2& rhs) const { return (m[0] != rhs[0] || m[1] != rhs[1]); }

    // ------------------------------------------------------------------------
    // getter
    // ------------------------------------------------------------------------
    inline float getX() const { return m[0]; }
    inline float getY() const { return m[1]; }

    // ------------------------------------------------------------------------
    // setter
    // ------------------------------------------------------------------------
    inline void setX(float x) { m[0] = x; }
    inline void setY(float y) { m[1] = y; }

    // ------------------------------------------------------------------------
    // operator at
    // ------------------------------------------------------------------------
    inline       float& operator[](int x)       { return m[x]; }
    inline const float& operator[](int x) const { return m[x]; }

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    explicit operator       float* ()       { return m; }
    explicit operator const float* () const { return m; }

private:
    float m[2];
};

inline MathCommonVector2 operator *(float lhs, const MathCommonVector2& rhs) { return MathCommonVector2(lhs * rhs[0], lhs * rhs[1]); }
inline MathCommonVector2 operator /(float lhs, const MathCommonVector2& rhs) { return MathCommonVector2(lhs / rhs[0], lhs / rhs[1]); }

// ----------------------------------------------------------------------------

class MathCommonVector3
{
public:
    //! コンストラクター
    MathCommonVector3()
    {
        m[0] = 0.f; m[1] = 0.f; m[2] = 0.f;
    }
    //! コンストラクター
    MathCommonVector3(float x, float y, float z)
    {
        m[0] = x; m[1] = y; m[2] = z;
    }
    //! コンストラクター
    MathCommonVector3(const MathCommonVector2& v, float z)
    {
        m[0] = v[0]; m[1] = v[1]; m[2] = z;
    }

    /*!
     * ベクトルを正規化します
     * @return float 正規化前のベクトルの大きさを返します
     */
    inline float setNormalize();

    /*!
     * ベクトルの大きさの二乗を返します
     */
    inline float calcLengthSq() const;

    /*!
     * ベクトルの大きさを求めます
     */
    inline float calcLength() const;

    /*!
     * 正規化したベクトルを返します
     * @return MathCommonVector3 正規化したベクトル
     */
    inline MathCommonVector3 calcNormalize() const;

    /*!
     * ベクトル同士の内積を求めます
     * @param[in] v ベクトル
     */
    inline float calcDot(const MathCommonVector3& v) const;

    /*!
     * ベクトル同士の外積を求めます
     * @param[in] v ベクトル
     */
    inline MathCommonVector3 calcCross(const MathCommonVector3& v) const;

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    inline MathCommonVector3 operator +() const { return *this; }
    inline MathCommonVector3 operator -() const { return MathCommonVector3(-m[0], -m[1], -m[2]); }

    inline MathCommonVector3 operator +(const MathCommonVector3& rhs) const { return MathCommonVector3(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2]); }
    inline MathCommonVector3 operator -(const MathCommonVector3& rhs) const { return MathCommonVector3(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2]); }
    inline MathCommonVector3 operator *(const MathCommonVector3& rhs) const { return MathCommonVector3(m[0] * rhs[0], m[1] * rhs[1], m[2] * rhs[2]); }
    inline MathCommonVector3 operator /(const MathCommonVector3& rhs) const { return MathCommonVector3(m[0] / rhs[0], m[1] / rhs[1], m[2] / rhs[2]); }
    inline MathCommonVector3 operator *(float rhs) const { return MathCommonVector3(m[0] * rhs, m[1] * rhs, m[2] * rhs); }
    inline MathCommonVector3 operator /(float rhs) const { float f_inv = 1.f / rhs; return MathCommonVector3(m[0] * f_inv, m[1] * f_inv, m[2] * f_inv); }

    inline MathCommonVector3& operator +=(const MathCommonVector3& rhs) { m[0] += rhs[0]; m[1] += rhs[1]; m[2] += rhs[2]; return *this; }
    inline MathCommonVector3& operator -=(const MathCommonVector3& rhs) { m[0] -= rhs[0]; m[1] -= rhs[1]; m[2] -= rhs[2]; return *this; }
    inline MathCommonVector3& operator *=(const MathCommonVector3& rhs) { m[0] *= rhs[0]; m[1] *= rhs[1]; m[2] *= rhs[2]; return *this; }
    inline MathCommonVector3& operator /=(const MathCommonVector3& rhs) { m[0] /= rhs[0]; m[1] /= rhs[1]; m[2] /= rhs[2]; return *this; }
    inline MathCommonVector3& operator *=(float rhs) { m[0] *= rhs; m[1] *= rhs; m[2] *= rhs; return *this; }
    inline MathCommonVector3& operator /=(float rhs) { float f_inv = 1.f / rhs; m[0] *= f_inv; m[1] *= f_inv; m[2] *= f_inv; return *this; }

    inline bool operator ==(const MathCommonVector3& rhs) const { return (m[0] == rhs[0] && m[1] == rhs[1] && m[2] == rhs[2]); }
    inline bool operator !=(const MathCommonVector3& rhs) const { return (m[0] != rhs[0] || m[1] != rhs[1] || m[2] != rhs[2]); }

    // ------------------------------------------------------------------------
    // getter
    // ------------------------------------------------------------------------
    inline float getX() const { return m[0]; }
    inline float getY() const { return m[1]; }
    inline float getZ() const { return m[2]; }

    // ------------------------------------------------------------------------
    // setter
    // ------------------------------------------------------------------------
    inline void setX(float x) { m[0] = x; }
    inline void setY(float y) { m[1] = y; }
    inline void setZ(float z) { m[2] = z; }

    // ------------------------------------------------------------------------
    // operator at
    // ------------------------------------------------------------------------
    inline       float& operator[](int x)       { return m[x]; }
    inline const float& operator[](int x) const { return m[x]; }

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    explicit operator       float* ()       { return m; }
    explicit operator const float* () const { return m; }

protected:
    float m[3];
};

inline MathCommonVector3 operator *(float lhs, const MathCommonVector3& rhs) { return MathCommonVector3(lhs * rhs[0], lhs * rhs[1], lhs * rhs[2]); }
inline MathCommonVector3 operator /(float lhs, const MathCommonVector3& rhs) { return MathCommonVector3(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2]); }

// ----------------------------------------------------------------------------

class alignas(16) MathCommonVector4
{
public:
    //! コンストラクター
    MathCommonVector4()
    {
        m[0] = 0.f; m[1] = 0.f; m[2] = 0.f; m[3] = 0.f;
    }
    //! コンストラクター
    MathCommonVector4(float x, float y, float z, float w)
    {
        m[0] = x; m[1] = y; m[2] = z; m[3] = w;
    }
    //! コンストラクター
    MathCommonVector4(const MathCommonVector3& v, float w)
    {
        m[0] = v[0]; m[1] = v[1]; m[2] = v[2]; m[3] = w;
    }

    // ------------------------------------------------------------------------
    // operator -+*/
    // ------------------------------------------------------------------------
    inline MathCommonVector4 operator +() const { return *this; }
    inline MathCommonVector4 operator -() const { return MathCommonVector4(-m[0], -m[1], -m[2], -m[3]); }

    inline MathCommonVector4 operator +(const MathCommonVector4& rhs) const { return MathCommonVector4(m[0] + rhs[0], m[1] + rhs[1], m[2] + rhs[2], m[3] + rhs[3]); }
    inline MathCommonVector4 operator -(const MathCommonVector4& rhs) const { return MathCommonVector4(m[0] - rhs[0], m[1] - rhs[1], m[2] - rhs[2], m[3] - rhs[3]); }
    inline MathCommonVector4 operator *(const MathCommonVector4& rhs) const { return MathCommonVector4(m[0] * rhs[0], m[1] * rhs[1], m[2] * rhs[2], m[3] * rhs[3]); }
    inline MathCommonVector4 operator /(const MathCommonVector4& rhs) const { return MathCommonVector4(m[0] / rhs[0], m[1] / rhs[1], m[2] / rhs[2], m[3] / rhs[3]); }
    inline MathCommonVector4 operator *(float rhs) const { return MathCommonVector4(m[0] * rhs, m[1] * rhs, m[2] * rhs, m[3] * rhs); }
    inline MathCommonVector4 operator /(float rhs) const { float f_inv = 1.f / rhs; return MathCommonVector4(m[0] * f_inv, m[1] * f_inv, m[2] * f_inv, m[3] * f_inv); }

    inline MathCommonVector4& operator +=(const MathCommonVector4& rhs) { m[0] += rhs[0]; m[1] += rhs[1]; m[2] += rhs[2]; m[3] += rhs[3]; return *this; }
    inline MathCommonVector4& operator -=(const MathCommonVector4& rhs) { m[0] -= rhs[0]; m[1] -= rhs[1]; m[2] -= rhs[2]; m[3] -= rhs[3]; return *this; }
    inline MathCommonVector4& operator *=(const MathCommonVector4& rhs) { m[0] *= rhs[0]; m[1] *= rhs[1]; m[2] *= rhs[2]; m[3] *= rhs[3]; return *this; }
    inline MathCommonVector4& operator /=(const MathCommonVector4& rhs) { m[0] /= rhs[0]; m[1] /= rhs[1]; m[2] /= rhs[2]; m[3] /= rhs[3]; return *this; }
    inline MathCommonVector4& operator *=(float rhs) { m[0] *= rhs; m[1] *= rhs; m[2] *= rhs; m[3] *= rhs; return *this; }
    inline MathCommonVector4& operator /=(float rhs) { float f_inv = 1.f / rhs; m[0] *= f_inv; m[1] *= f_inv; m[2] *= f_inv; m[3] *= f_inv; return *this; }

    inline bool operator ==(const MathCommonVector4& rhs) const { return (m[0] == rhs[0] && m[1] == rhs[1] && m[2] == rhs[2] && m[3] == rhs[3]); }
    inline bool operator !=(const MathCommonVector4& rhs) const { return (m[0] != rhs[0] || m[1] != rhs[1] || m[2] != rhs[2] || m[3] != rhs[3]); }

    // ------------------------------------------------------------------------
    // getter
    // ------------------------------------------------------------------------
    inline float getX() const { return m[0]; }
    inline float getY() const { return m[1]; }
    inline float getZ() const { return m[2]; }
    inline float getW() const { return m[3]; }
    inline MathCommonVector3 getXYZ() const { return MathCommonVector3(m[0], m[1], m[2]); }

    // ------------------------------------------------------------------------
    // setter
    // ------------------------------------------------------------------------
    inline void setX(float x) { m[0] = x; }
    inline void setY(float y) { m[1] = y; }
    inline void setZ(float z) { m[2] = z; }
    inline void setW(float w) { m[3] = w; }

    // ------------------------------------------------------------------------
    // operator at
    // ------------------------------------------------------------------------
    inline       float& operator[](int x)       { return m[x]; }
    inline const float& operator[](int x) const { return m[x]; }

    // ------------------------------------------------------------------------
    // operator cast
    // ------------------------------------------------------------------------
    explicit operator       float* ()       { return m; }
    explicit operator const float* () const { return m; }

protected:
    float m[4];
};

inline MathCommonVector4 operator *(float lhs, const MathCommonVector4& rhs) { return MathCommonVector4(lhs * rhs[0], lhs * rhs[1], lhs * rhs[2], lhs * rhs[3]); }
inline MathCommonVector4 operator /(float lhs, const MathCommonVector4& rhs) { return MathCommonVector4(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2], lhs / rhs[3]); }

}

#include <math/balMathCommonVector_inl.h>
