/*!
 * @file   balEngine.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <framework/balFrameworkCallback.h>
#include <math/balMath.h>

namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class IRenderTargetColor; }
namespace bal::gfx { class IRenderTargetDepth; }
namespace bal::gfx { class FrameBuffer; }
namespace bal::gfx { class ShaderArchive; }

// ----------------------------------------------------------------------------
namespace bal {

class Engine : public FrameworkCallback
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        gfx::IGraphics* mpGraphics = nullptr;
        MathSize mRenderBufferSize;
    };

public:
    /*! コンストラクター */
    Engine();

    /*! デストラクター */
    virtual ~Engine();

    /*!
     * エンジンの初期化
     * @param[in] arg 引数構造体
     */
    bool initialize(const InitializeArg& arg);

public:
    /*!
     * 計算処理するコールバック
     */
    virtual void onUpdate(const UpdateArg& arg) override;

    /*!
     * 描画処理するコールバック
     */
    virtual void onDraw(const DrawArg& arg) override;

private:
    std::unique_ptr<gfx::IRenderTargetColor> mpRenderTargetColor;
    std::unique_ptr<gfx::IRenderTargetDepth> mpRenderTargetDepth;
    std::unique_ptr<gfx::FrameBuffer>        mpFrameBuffer;
    std::unique_ptr<gfx::ShaderArchive>      mpShaderArchive;
};

}
