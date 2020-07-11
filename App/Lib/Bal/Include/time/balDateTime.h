/*!
 * @file   balDateTime.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
 // std
#include <chrono>
// bal
#include <time/balTimeSpan.h>

// ----------------------------------------------------------------------------
namespace bal {

class DateTime
{
public:
    //! 現在の時刻で初期化
    DateTime() { now(); }

    /*!
     * 現在の時間を設定します
     */
    void now();

    /*!
     * 年を取得します
     */
    inline int getYear() const { return mTime.tm_year + 1900; }

    /*!
     * 月を取得します
     * @note 0～11 の間で返します
     */
    inline int getMonth() const { return mTime.tm_mon; }

    /*!
     * 日を取得します
     * @note 1～31 の間で返します
     */
    inline int getDate() const { return mTime.tm_mday; }

    /*!
     * 時を取得します
     * @note 0～23 の間で返します
     */
    inline int getHours() const { return mTime.tm_hour; }

    /*!
     * 分を取得します
     * @note 0～59 の間で返します
     */
    inline int getMinutes() const { return mTime.tm_min; }

    /*!
     * 秒を取得します
     * @note 0～60 の間で返します
     */
    inline int getSeconds() const { return mTime.tm_sec; }

    /*!
     * ミリ秒を取得します
     * @note 0～999 の間で返します
     */
    int getMilliseconds() const;

    /*!
     * マイクロ秒を取得します
     * @note 0～999 の間で返します
     */
    int getMicroseconds() const;

    /*!
     * ナノ秒を取得します
     * @note 0～999 の間で返します
     */
    int getNanoseconds() const;

public:
    /*!
     * DateTime の差を求めます
     */
    TimeSpan operator-(const DateTime& rhs) const;

private:
    std::chrono::system_clock::time_point mTimePoint; //!< 現在の時刻
    tm                                    mTime;      //!< 現在の時刻（C）
};

}
