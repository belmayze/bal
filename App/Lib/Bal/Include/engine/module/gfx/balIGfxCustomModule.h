/*!
 * @file   balIGfxCustomModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/gfx/balGfxModule.h>
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
    virtual void onDraw(const FrameworkCallback::DrawArg& arg, const IModule& mod) = 0;

    /*!
     * シーン用定数バッファのサイズを取得します
     */
    virtual size_t getSceneConstantBufferSize() const { return sizeof(Module::SceneConstantBufferBase); }

    /*!
     * メッシュ用定数バッファのサイズを取得します
     */
    virtual size_t getMeshConstantBufferSize() const { return sizeof(Module::MeshConstantBufferBase); }
};

}
