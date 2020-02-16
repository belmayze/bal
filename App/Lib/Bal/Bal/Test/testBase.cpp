/*!
 * @file   testBase.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <io/balLog.h>
// test
#include "testBase.h"

namespace test {

void TestBase::exec()
{
    bal::Log::Print("====テスト開始 [%s]====\n", mName.c_str());
    exec_();
    bal::Log::Print("====テスト終了====\n");
}

}
