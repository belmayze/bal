/*!
 * @file   balCntlModule.h
 * @brief  コントローラーモジュール
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balIModule.h>

// ----------------------------------------------------------------------------
namespace bal::mod::cntl {

class Module : public IModule
{
public:
    /*!
     * コンストラクター
     */
    Module();

    /*!
     * デストラクター
     */
    virtual ~Module();

    /*!
     * カスタムモジュールを登録する
     * @param[in] p_custom_module カスタムモジュール
     */
    virtual void setCustomModule(std::unique_ptr<mod::ICustomModule>&& p_custom_module) override;

    /*!
     * モジュールの初期化を行う関数です
     * @param[in] arg 初期化引数
     */
    virtual void initialize(const InitializeArg& arg) override;

    /*!
     * モジュールの終了処理を行う関数です
     */
    virtual void finalize() override;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const FrameworkCallback::UpdateArg& arg) override;

    /*!
     * モジュールの種類を取得する
     */
    virtual ModuleType getModuleType() const { return ModuleType::Controller; }
};

}
