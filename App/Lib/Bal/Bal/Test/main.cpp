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
#include <container/balString.h>
// test
#include "testHeap.h"

int main()
{
    bal::Framework framework;

    test::TestHeap test("TestHeap"); test.exec();

    return 0;
}
