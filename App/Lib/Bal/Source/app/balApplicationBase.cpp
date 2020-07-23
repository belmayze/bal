/*!
 * @file   balApplicationBase.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <app/balApplicationBase.h>

namespace bal
{

// ----------------------------------------------------------------------------

ApplicationBase::ApplicationBase()
{
    if (!gpApplication) { gpApplication = this; }
}

// ----------------------------------------------------------------------------

ApplicationBase::~ApplicationBase()
{
    gpApplication = nullptr;
}

// ----------------------------------------------------------------------------

bal::ApplicationBase* gpApplication = nullptr;

// ----------------------------------------------------------------------------

}
