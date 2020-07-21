/*!
 * @file   balRadian.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class Degree; }

// ----------------------------------------------------------------------------

namespace bal
{

class Radian
{
public:
    //! コンストラクター
    Radian() {}

    /*!
     * 角度を指定して初期化
     */
    explicit Radian(float f) : mValue(f) {}

    /*!
     * ラジアンを指定して初期化
     * @param[in] degree 角度
     */
    Radian(const Degree& degree);

    /*!
     * ラジアンへ変換
     */
    Degree toDegree() const;

    /*!
     * 数値へ変換します
     */
    operator float() const { return mValue; }

    // ------------------------------------------------------------------------
    // operator +-*/
    // ------------------------------------------------------------------------
    //! 足し算
    Radian operator +(const Radian& rhs) const { return Radian(mValue + rhs.mValue); }
    //! 引き算
    Radian operator -(const Radian& rhs) const { return Radian(mValue - rhs.mValue); }
    //! 掛け算
    Radian operator *(float rhs) const { return Radian(mValue * rhs); }
    //! 割り算
    Radian operator /(float rhs) const { return Radian(mValue / rhs); }

    //! 足し算
    Radian& operator *=(const Radian& rhs) { mValue += rhs.mValue; return *this; }
    //! 引き算
    Radian& operator -=(const Radian& rhs) { mValue -= rhs.mValue; return *this; }
    //! 掛け算
    Radian& operator *=(float rhs) { mValue *= rhs; return *this; }
    //! 割り算
    Radian& operator /=(float rhs) { mValue /= rhs; return *this; }

    //! 変化
    Radian operator -() const { return Radian(-mValue); }

private:
    float mValue = 0.f;
};

}

// ----------------------------------------------------------------------------
