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
#include <heap/balHeapOS.h>
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
    // ヒープマネージャーに登録
    mpMainHeap = HeapOS::Create(); // 仮
    HeapManager::GetInstance().setCurrentHeap(mpMainHeap.get());
}

// ----------------------------------------------------------------------------

}
