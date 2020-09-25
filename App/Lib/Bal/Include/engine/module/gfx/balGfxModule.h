/*!
 * @file   balGfxModule.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <engine/module/balIModule.h>

namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class IRenderTargetColor; }
namespace bal::gfx { class IRenderTargetDepth; }
namespace bal::gfx { class FrameBuffer; }
namespace bal::gfx { class ShaderArchive; }

// ----------------------------------------------------------------------------
namespace bal::mod::gfx {

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
     * モジュールの初期化を行う関数です
     * @param[in] arg 初期化引数
     */
    virtual void initialize(const InitializeArg& arg) override;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const FrameworkCallback::UpdateArg& arg) override;

    /*!
     * モジュールの種類を取得する
     */
    virtual ModuleType getModuleType() const { return ModuleType::Graphics; }

public:
    /*!
     * 描画を行う関数です
     * @param[in] arg 描画引数
     */
    void onDraw(const FrameworkCallback::DrawArg& arg);

private:
    std::unique_ptr<bal::gfx::IRenderTargetColor> mpRenderTargetColor;
    std::unique_ptr<bal::gfx::IRenderTargetDepth> mpRenderTargetDepth;
    std::unique_ptr<bal::gfx::FrameBuffer>        mpFrameBuffer;
    std::unique_ptr<bal::gfx::ShaderArchive>      mpShaderArchive;
};

}
