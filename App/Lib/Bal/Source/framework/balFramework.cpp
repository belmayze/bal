/*!
 * @file   balFramework.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <framework/balFramework.h>
#include <heap/balHeapManager.h>
#include <memory/balSingletonFinalizer.h>

namespace bal {

// ----------------------------------------------------------------------------

Framework::Framework()
{

}

// ----------------------------------------------------------------------------

Framework::~Framework()
{
    SingletonFinalizer::Finalize();
}

// ----------------------------------------------------------------------------

void Framework::initialize( const InitializeArg& arg )
{
    // ルートヒープを確保
    mpRootHeap = heap::RootBlock::Create(arg.mHeapSize);
    heap::Manager::GetInstance(mpRootHeap.get()).setCurrentHeap(mpRootHeap.get());
}

// ----------------------------------------------------------------------------

}
