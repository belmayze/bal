/*!
 * @file   balSingletonFinalizer.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <memory/balSingletonFinalizer.h>

namespace bal {

// ----------------------------------------------------------------------------

int SingletonFinalizer::mIndex = 0;
std::array<SingletonFinalizer::DestroyFunc, SingletonFinalizer::cMaxFinalizerNum> SingletonFinalizer::mFunctions;

// ----------------------------------------------------------------------------

void SingletonFinalizer::AddFunc(DestroyFunc func)
{
    BAL_ASSERT(mIndex < cMaxFinalizerNum);
    mFunctions[mIndex++] = func;
}

// ----------------------------------------------------------------------------

void SingletonFinalizer::Finalize()
{
    for (int i = mIndex - 1; i >= 0; --i)
    {
        (*mFunctions[i])();
    }
    mIndex = 0;
}

// ----------------------------------------------------------------------------

}
