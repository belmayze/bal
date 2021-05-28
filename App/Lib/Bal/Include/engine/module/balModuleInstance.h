/*!
 * @file   balModuleInstance.h
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

template <typename T>
class ModuleInstance : public IModule
{
public:
    ModuleInstance() { sModule = this; }
    virtual ~ModuleInstance() { sModule = nullptr; }

public:
    //! モジュールを取得
    static T* GetModule() { return static_cast<T*>(sModule); }

private:
    inline static ModuleInstance* sModule = nullptr;
};

}
