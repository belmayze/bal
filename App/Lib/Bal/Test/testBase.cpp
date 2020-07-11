/*!
 * @file   testBase.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <io/balLog.h>
#include <time/balDateTime.h>
// test
#include "testBase.h"

namespace test {

void TestBase::exec()
{
    bal::Log::Print("====テスト開始 [%s]====\n", mName.c_str());
    bal::DateTime start;
    {
        exec_();
    }
    bal::DateTime end;
    bal::TimeSpan diff = start - end;
    bal::Log::Print("====テスト終了====\n");
    bal::Log::Print("処理時間 [%dms]\n", diff.getMilliseconds());
}

}
