/*!
 * @file   balDateTime.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include "time/balDateTime.h"

namespace bal
{

// ----------------------------------------------------------------------------

void DateTime::now()
{
    mTimePoint  = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(mTimePoint);

    tm* p = nullptr;
    localtime_s(p , &time);
    if (p)
    {
        mTime = *p;
    }
}

// ----------------------------------------------------------------------------

int DateTime::getYear() const
{
    return mTime.tm_year;
}

// ----------------------------------------------------------------------------

int DateTime::getMonth() const
{
    return mTime.tm_mon;
}

// ----------------------------------------------------------------------------

int DateTime::getDate() const
{
    return mTime.tm_mday;
}

// ----------------------------------------------------------------------------

int DateTime::getHours() const
{
    return mTime.tm_hour;
}

// ----------------------------------------------------------------------------

int DateTime::getMinutes() const
{
    return mTime.tm_min;
}

// ----------------------------------------------------------------------------

int DateTime::getSeconds() const
{
    return mTime.tm_sec;
}

// ----------------------------------------------------------------------------

int DateTime::getMilliseconds() const
{
    // @TODO:
    return 0;
}

// ----------------------------------------------------------------------------

}
