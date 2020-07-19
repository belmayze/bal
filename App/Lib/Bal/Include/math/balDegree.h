/*!
 * @file   balDegree.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class Radian; }

// ----------------------------------------------------------------------------

namespace bal
{

class Degree
{
public:
    //! コンストラクター
    Degree() {}

    /*!
     * 角度を指定して初期化
     */
    explicit Degree(float f) : mValue(f) {}

    /*!
     * ラジアンを指定して初期化
     * @param[in] radian ラジアン
     */
    Degree(const Radian& radian);

    /*!
     * ラジアンへ変換
     */
    Radian toRadian() const;

    /*!
     * 数値へ変換します
     */
    operator float() const { return mValue; }

private:
    float mValue = 0.f;
};

}

// ----------------------------------------------------------------------------
