﻿/*!
 * @file   testHeap.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapOS.h>
#include <io/balLog.h>
// test
#include "testHeap.h"

namespace test {

void TestHeap::exec_()
{
    auto heap = bal::HeapOS::Create("Test");
    {
        void* ptr = heap->alloc(100);
        bal::Log::Print("Alloc [%p]\n", ptr);
        free(ptr);
    }
}

}
