/*!
 * @file   balSingletonGfxFinalizer.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <memory/balSingletonGfxFinalizer.h>

namespace bal {

// ----------------------------------------------------------------------------

int SingletonGfxFinalizer::mIndex = 0;
Array<SingletonGfxFinalizer::DestroyFunc, SingletonGfxFinalizer::cMaxFinalizerNum> SingletonGfxFinalizer::mFunctions;

// ----------------------------------------------------------------------------

void SingletonGfxFinalizer::AddFunc(DestroyFunc func)
{
    BAL_ASSERT(mIndex < cMaxFinalizerNum);
    mFunctions[mIndex++] = func;
}

// ----------------------------------------------------------------------------

void SingletonGfxFinalizer::Finalize()
{
    for (int i = mIndex - 1; i >= 0; --i)
    {
        (*mFunctions[i])();
    }
    mIndex = 0;
}

// ----------------------------------------------------------------------------

}
