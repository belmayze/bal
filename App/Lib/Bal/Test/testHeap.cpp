/*!
 * @file   testHeap.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <heap/balHeapBlock.h>
#include <heap/balHeapManager.h>
#include <io/balLog.h>
#include <math/balMath.h>
#include <time/balDateTime.h>
// test
#include "testHeap.h"

namespace test {

void TestHeap::exec_()
{
    bal::heap::BlockBase* p_root_heap = bal::heap::Manager::GetInstance().getCurrentHeap();

    auto heap = bal::heap::Block::Create(1 * 1024, "Test", p_root_heap);
    {
        void* ptr = heap->alloc(100);
        BAL_LOG_INFO("Alloc [%p]\n", ptr);
        delete ptr;
    }

    // 行列乗算
    {
        bal::DateTime start;
        {
            bal::MathMatrix44 m0, m1, m2;
            bal::MathVector4  v0, v1;
            m0.setScale(bal::MathVector3(1.f, 2.f, 3.f));
            m1.setTranslate(bal::MathVector3(2.f, 2.f, 2.f));
            v0 = bal::MathVector4(1.f, 2.f, 3.f, 1.f);
            for (int i = 0; i < 50000000; ++i)
            {
                m2 = m0 * m1;
            }
            //for (int i = 0; i < 10000000; ++i)
            //{
            //    v1 = m2 * v0;
            //}

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
            //bal::Log::Print(
            //    "Vector: %.1f, %.1f, %.1f, %.1f\n",
            //    v1[0], v1[1], v1[2], v1[3]
            //);
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        BAL_LOG_INFO("Calc SSE: %dms\n", diff.getMilliseconds());
    }
    {
        bal::DateTime start;
        {
            bal::MathCommonMatrix44 m0, m1, m2;
            bal::MathCommonVector4  v0, v1;
            m0.setScale(bal::MathVector3(1.f, 2.f, 3.f));
            m1.setTranslate(bal::MathVector3(2.f, 2.f, 2.f));
            v0 = bal::MathCommonVector4(1.f, 2.f, 3.f, 1.f);
            for (int i = 0; i < 50000000; ++i)
            {
                m2 = m0 * m1;
            }
            //for (int i = 0; i < 10000000; ++i)
            //{
            //    v1 = m2 * v0;
            //}

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
            //bal::Log::Print(
            //    "Vector: %.1f, %.1f, %.1f, %.1f\n",
            //    v1[0], v1[1], v1[2], v1[3]
            //);
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        BAL_LOG_INFO("Calc Common: %dms\n", diff.getMilliseconds());
    }

#if 1
    // 倒置行列
    {
        bal::DateTime start;
        {
            bal::MathMatrix44 m;
            m.setTranslate(bal::MathVector3(1.f, 2.f, 3.f));
            for (int i = 0; i < 100000000; ++i)
            {
                m.setTranspose();
            }
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        BAL_LOG_INFO("Calc SSE: %dms\n", diff.getMilliseconds());
    }
    {
        bal::DateTime start;
        {
            bal::MathCommonMatrix44 m;
            m.setTranslate(bal::MathVector3(1.f, 2.f, 3.f));
            for (int i = 0; i < 100000000; ++i)
            {
                m.setTranspose();
            }
        }
        bal::DateTime end;
        bal::TimeSpan diff = end - start;
        BAL_LOG_INFO("Calc Common: %dms\n", diff.getMilliseconds());
    }
#endif
}

}
