/*!
 * @file   main.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <framework/balFramework.h>
#include <io/balLog.h>
#include <container/balArray.h>
#include <container/balString.h>
// test
#include "testHeap.h"

int main()
{
    bal::Framework framework;
    {
        bal::Framework::InitializeArg init_arg;
        init_arg.mTitle    = "";
        init_arg.mHeapSize = 100 * 1024 * 1024;
        framework.initialize(init_arg);
    }

    bal::Array<std::unique_ptr<test::TestBase>, 1> arr = {
        std::make_unique<test::TestHeap>("TestHeap")
    };
    for (auto& test : arr)
    {
        test->exec();
    }

    return 0;
}
