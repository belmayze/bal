/*!
 * @file   balICustomModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balIModule.h>

// ----------------------------------------------------------------------------
namespace bal::mod {

class ICustomModule
{
public:
    ICustomModule() {}
    virtual ~ICustomModule() {}

    /*!
     * モジュールの初期化を行う関数です
     */
    virtual void initialize(const IModule::InitializeArg& arg) = 0;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const FrameworkCallback::UpdateArg& arg) = 0;
};

}
