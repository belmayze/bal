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
    int getYear() const;

    /*!
     * 月を取得します
     */
    int getMonth() const;

    /*!
     * 日を取得します
     */
    int getDate() const;

    /*!
     * 時を取得します
     */
    int getHours() const;

    /*!
     * 分を取得します
     */
    int getMinutes() const;

    /*!
     * 秒を取得します
     */
    int getSeconds() const;

    /*!
     * ミリ秒を取得します
     */
    int getMilliseconds() const;

private:
    std::chrono::system_clock::time_point mTimePoint;
    tm                                    mTime;
};

}
