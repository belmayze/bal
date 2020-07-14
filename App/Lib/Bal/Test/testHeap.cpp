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

    // 行列乗算
    {
        bal::DateTime start;
        {
            bal::MathMatrix44 m0, m1, m2;
            m0.setRotateScale(bal::MathVector3(), bal::MathVector3(1.f, 2.f, 3.f));
            m1.setTranslate(2.f, 2.f, 2.f);
            for (int i = 0; i < 10000000; ++i)
            {
                m2 = m0 * m1;
            }

            //bal::Log::Print(
            //    "Matrix: %.1f, %.1f, %.1f, %.1f\n"
            //    "        %.1f, %.1f, %.1f, %.1f\n"
            //    "        %.1f, %.1f, %.1f, %.1f\n"
            //    "        %.1f, %.1f, %.1f, %.1f\n",
            //    m2[0][0], m2[0][1], m2[0][2], m2[0][3],
            //    m2[1][0], m2[1][1], m2[1][2], m2[1][3],
            //    m2[2][0], m2[2][1], m2[2][2], m2[2][3],
            //    m2[3][0], m2[3][1], m2[3][2], m2[3][3]
            //);
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        bal::Log::Print("Calc SSE: %dms\n", diff.getMilliseconds());
    }
    {
        bal::DateTime start;
        {
            bal::MathCommonMatrix44 m0, m1, m2;
            m0.setRotateScale(bal::MathVector3(), bal::MathVector3(1.f, 2.f, 3.f));
            m1.setTranslate(2.f, 2.f, 2.f);
            for (int i = 0; i < 10000000; ++i)
            {
                m2 = m0 * m1;
            }

            //bal::Log::Print(
            //    "Matrix: %.1f, %.1f, %.1f, %.1f\n"
            //    "        %.1f, %.1f, %.1f, %.1f\n"
            //    "        %.1f, %.1f, %.1f, %.1f\n"
            //    "        %.1f, %.1f, %.1f, %.1f\n",
            //    m2[0][0], m2[0][1], m2[0][2], m2[0][3],
            //    m2[1][0], m2[1][1], m2[1][2], m2[1][3],
            //    m2[2][0], m2[2][1], m2[2][2], m2[2][3],
            //    m2[3][0], m2[3][1], m2[3][2], m2[3][3]
            //);
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        bal::Log::Print("Calc Common: %dms\n", diff.getMilliseconds());
    }

#if 1
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
#endif
}

}
