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
namespace bal::mod { class ICustomModule; }

// ----------------------------------------------------------------------------
namespace bal::mod {

class IModule
{
public:
    //! モジュールの初期化構造体
    struct InitializeArg
    {
        void* mpDummy = nullptr;
    };

public:
    IModule() {}
    virtual ~IModule() {}

    /*!
     * カスタムモジュールを登録する
     * @note 初期化前に指定してください
     * @param[in] p_custom_module カスタムモジュール
     */
    virtual void setCustomModule(std::unique_ptr<ICustomModule>&& p_custom_module) = 0;

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
