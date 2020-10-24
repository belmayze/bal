/*!
 * @file   balIGfxCustomModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balICustomModule.h>

// ----------------------------------------------------------------------------
namespace bal::mod::gfx {

class ICustomModule : public bal::mod::ICustomModule
{
public:
    /*!
     * モジュールの描画を行う関数です
     * @param[in] arg 描画引数
     */
    virtual void onDraw(const FrameworkCallback::DrawArg& arg, const IModule& module) = 0;
};

}
