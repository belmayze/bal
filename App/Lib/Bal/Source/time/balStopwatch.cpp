/*!
 * @file   balStopwatch.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <time/balStopwatch.h>

namespace bal
{

// ----------------------------------------------------------------------------

void Stopwatch::start()
{
    mDateTime.now();
}

// ----------------------------------------------------------------------------

void Stopwatch::lap()
{
    DateTime now;
    mTimeSpan = now - mDateTime;
}

// ----------------------------------------------------------------------------

void Stopwatch::stop()
{
    DateTime now;
    mTimeSpan = now - mDateTime;
}

// ----------------------------------------------------------------------------

}
