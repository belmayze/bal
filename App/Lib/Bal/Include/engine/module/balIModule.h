/*!
 * @file   balIModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balModuleType.h>
#include <framework/balFrameworkCallback.h>

namespace bal { class Engine; }

// ----------------------------------------------------------------------------
namespace bal::mod {

class IModule
{
public:
    //! モジュールの初期化構造体
    struct InitializeArg
    {
        const Engine* mpEngine = nullptr;
    };

public:
    IModule() {}
    virtual ~IModule() {}

    /*!
     * モジュールの初期化を行う関数です
     * @param[in] arg 初期化引数
     */
    virtual void initialize(const InitializeArg& arg) = 0;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const FrameworkCallback::UpdateArg& arg) = 0;

    /*!
     * モジュールの種類を取得する
     */
    virtual ModuleType getModuleType() const = 0;
};

}
