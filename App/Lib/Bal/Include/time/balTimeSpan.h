/*!
 * @file   balTimeSpan.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
 // std
#include <chrono>

// ----------------------------------------------------------------------------
namespace bal {

class TimeSpan
{
public:
    //! コンストラクター
    TimeSpan() {}

    //! コンストラクター
    TimeSpan(std::chrono::microseconds microseconds) :mMicroseconds(microseconds) {}

    /*!
     * 秒を取得します
     */
    inline int getSeconds() const { return static_cast<int>(mMicroseconds.count() / 1000000); }

    /*!
     * ミリ秒を取得します
     */
    inline int getMilliseconds() const { return static_cast<int>(mMicroseconds.count() / 1000); }

    /*!
     * マイクロ秒を取得します
     */
    inline int getMicroseconds() const { return static_cast<int>(mMicroseconds.count()); }

private:
    std::chrono::microseconds mMicroseconds;
};

}
