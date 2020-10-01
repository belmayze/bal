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
    BAL_LOG_INFO("====テスト開始 [%s]====\n", mName.c_str());
    bal::DateTime start;
    {
        exec_();
    }
    bal::DateTime end;
    bal::TimeSpan diff = end - start;
    BAL_LOG_INFO("====テスト終了====\n");
    BAL_LOG_INFO("処理時間 [%dms]\n", diff.getMilliseconds());
}

}
