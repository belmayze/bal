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

namespace bal { class IConstantBuffer; }
namespace bal { class IDescriptorHeap; }
namespace bal { class IPipeline; }
namespace bal { class ShaderArchive; }

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
    virtual void initialize(const bal::mod::IModule::InitializeArg& arg, const bal::mod::IModule& mod) override;

    /*!
     * モジュールの更新を行う関数です
     * @param[in] arg 更新引数
     */
    virtual void onUpdate(const bal::FrameworkCallback::UpdateArg& arg, const bal::mod::IModule& mod) override;

    /*!
     * モジュールの描画を行う関数です
     * @param[in] arg 描画引数
     */
    virtual void onDraw(const bal::FrameworkCallback::DrawArg& arg, const bal::mod::IModule& mod) override;

private:
    std::unique_ptr<bal::ShaderArchive> mpShaderArchive;

    std::unique_ptr<bal::IPipeline>       mpSamplePipeline;
    std::unique_ptr<bal::IDescriptorHeap> mpSampleDescriptorHeap;
    std::unique_ptr<bal::IConstantBuffer> mpSampleConstantBuffer;
};

}
