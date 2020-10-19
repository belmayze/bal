/*!
 * @file   gfxCustomModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/gfx/balIGfxCustomModule.h>

// ----------------------------------------------------------------------------
namespace app::mod::gfx {

class CustomModule : public bal::mod::gfx::ICustomModule
{
public:
    static constexpr uint32_t cColorRenderTargetMax = 8;

public:
    //! コンストラクター
    CustomModule();

    //! デストラクター
    virtual ~CustomModule();

    /*!
     * モジュールの初期化を行う関数です
     */
    virtual void initialize(const bal::mod::IModule::InitializeArg& arg) override;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const bal::FrameworkCallback::UpdateArg& arg) override;

    /*!
     * モジュールの描画を行う関数です
     * @param[in] arg 描画引数
     */
    virtual void onDraw(const bal::FrameworkCallback::DrawArg& arg) override;
};

}
