/*!
 * @file   balDebugModule.h
 * @brief  デバッグモジュール
 * @author belmayze
 *
 * Copyright (c) 2021 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balModuleInstance.h>

namespace bal::debug { class ProcessTimeHolder; }

// ----------------------------------------------------------------------------
namespace bal::mod::debug {

class Module : public ModuleInstance<Module>
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
     * 描画を行う関数です
     * @param[in] arg 描画引数
     */
    virtual void onDraw(const FrameworkCallback::DrawArg& arg) override {}

public:
    //! ProcessTimeHolder
    bal::debug::ProcessTimeHolder* getProcessTimeHolder() { return mpProcessTimeHolder.get(); }

private:
    std::unique_ptr<bal::debug::ProcessTimeHolder> mpProcessTimeHolder;
};

}
