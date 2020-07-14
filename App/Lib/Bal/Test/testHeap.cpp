/*!
 * @file   testHeap.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapBlockOS.h>
#include <io/balLog.h>
#include <math/balMath.h>
#include <math/balMathCommonMatrix.h>
#include <time/balDateTime.h>
// test
#include "testHeap.h"

namespace test {

void TestHeap::exec_()
{
    auto heap = bal::heap::BlockOS::Create("Test");
    {
        void* ptr = heap->alloc(100);
        bal::Log::Print("Alloc [%p]\n", ptr);
        free(ptr);
    }

    // 倒置行列
    {
        bal::DateTime start;
        {
            bal::MathMatrix44 m;
            m.setTranslate(1.f, 2.f, 3.f);
            for (int i = 0; i < 10000000; ++i)
            {
                m.setTranspose();
            }
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        bal::Log::Print("Calc SSE: %dms\n", diff.getMilliseconds());
    }
    {
        bal::DateTime start;
        {
            bal::MathCommonMatrix44 m;
            m.setTranslate(1.f, 2.f, 3.f);
            for (int i = 0; i < 10000000; ++i)
            {
                m.setTranspose();
            }
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        bal::Log::Print("Calc Common: %dms\n", diff.getMilliseconds());
    }

}

}
