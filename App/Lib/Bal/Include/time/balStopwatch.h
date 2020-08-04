/*!
 * @file   balStopwatch.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <time/balDateTime.h>
#include <time/balTimeSpan.h>

// ----------------------------------------------------------------------------
namespace bal {

class Stopwatch
{
public:
    /*!
     * 計測を開始します
     */
    void start();

    /*!
     * 計測を止めずに、ラップタイムを取得します
     */
    void lap();

    /*!
     * 計測を終了します
     */
    void stop();

    /*!
     * 計測結果を取得します
     */
    TimeSpan getDiff() const { return mTimeSpan; }

private:
    DateTime mDateTime;
    TimeSpan mTimeSpan;
};

}
