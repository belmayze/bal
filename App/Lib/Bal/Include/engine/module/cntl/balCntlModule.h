﻿/*!
 * @file   balCntlModule.h
 * @brief  コントローラーモジュール
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balModuleInstance.h>

namespace bal::mod::cntl { class Controller; }

// ----------------------------------------------------------------------------
namespace bal::mod::cntl {

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
    //! コントローラーを取得
    const Controller& getController(uint32_t index) const;

private:
    std::unique_ptr<Controller[]> mpControllers;
    uint32_t                      mNumController = 0;
};

}
