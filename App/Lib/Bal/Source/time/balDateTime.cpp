/*!
 * @file   balDateTime.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <time/balDateTime.h>

namespace bal
{

// ----------------------------------------------------------------------------

void DateTime::now()
{
    mTimePoint  = std::chrono::system_clock::now();
    time_t time = std::chrono::system_clock::to_time_t(mTimePoint);

    localtime_s(&mTime, &time);
}

// ----------------------------------------------------------------------------

int DateTime::getMilliseconds() const
{
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(mTimePoint.time_since_epoch());
    return static_cast<int>(milliseconds.count() % 1000);
}

// ----------------------------------------------------------------------------

int DateTime::getMicroseconds() const
{
    std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(mTimePoint.time_since_epoch());
    return static_cast<int>(microseconds.count() % 1000);
}

// ----------------------------------------------------------------------------

int DateTime::getNanoseconds() const
{
    std::chrono::nanoseconds nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(mTimePoint.time_since_epoch());
    return static_cast<int>(nanoseconds.count() % 1000);
}

// ----------------------------------------------------------------------------

TimeSpan DateTime::operator-(const DateTime& rhs) const
{
    return TimeSpan(std::chrono::duration_cast<std::chrono::microseconds>(this->mTimePoint - rhs.mTimePoint));
}

// ----------------------------------------------------------------------------

}
