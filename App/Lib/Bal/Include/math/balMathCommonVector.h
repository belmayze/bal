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

class MathCommonVector3
{
public:
    //! コンストラクター
    MathCommonVector3()
    {
        m[0] = 1.f; m[1] = 0.f; m[2] = 0.f;
    }
    //! コンストラクター
    explicit MathCommonVector3(float x, float y, float z)
    {
        m[0] = x; m[1] = y; m[2] = z;
    }

    /*!
     * ベクトルを正規化します
     * @return float 正規化前のベクトルの大きさを返します
     */
    inline float setNormalize();

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
    //! ベクトル同士の加算
    inline MathCommonVector3 operator +(const MathCommonVector3& rhs) const;
    //! ベクトル同士の減算
    inline MathCommonVector3 operator -(const MathCommonVector3& rhs) const;

    //! ベクトル同士の加算
    inline const MathCommonVector3& operator +=(const MathCommonVector3& rhs);
    //! ベクトル同士の減算
    inline const MathCommonVector3& operator -=(const MathCommonVector3& rhs);

    // ------------------------------------------------------------------------
    // getter
    // ------------------------------------------------------------------------
    inline float getX() const { return m[0]; }
    inline float getY() const { return m[1]; }
    inline float getZ() const { return m[2]; }

protected:
    float m[3];
};

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
        m[0] = v.getX(); m[1] = v.getY(); m[2] = v.getZ();
    }

    // ------------------------------------------------------------------------
    // getter
    // ------------------------------------------------------------------------
    inline float getX() const { return m[0]; }
    inline float getY() const { return m[1]; }
    inline float getZ() const { return m[2]; }
    inline float getW() const { return m[3]; }

    // ------------------------------------------------------------------------
    // operator at
    // ------------------------------------------------------------------------
    inline       float& operator[](int x)       { return m[x]; }
    inline const float& operator[](int x) const { return m[x]; }

protected:
    float m[4];
};

}

#include <math/balMathCommonVector_inl.h>
