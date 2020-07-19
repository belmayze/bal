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

private:
    float mValue = 0.f;
};

}

// ----------------------------------------------------------------------------
