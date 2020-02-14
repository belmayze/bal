/*!
 * @file   balFramework.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <framework/balFramework.h>
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

}
