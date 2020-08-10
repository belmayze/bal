/*!
 * @file   balIPipeline.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>
#include <graphics/balFrameBuffer.h>
#include <graphics/balITexture.h>

namespace bal { class File; }
namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class IInputLayout; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class IPipeline
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*                                                  mpGraphics    = nullptr;
        uint32_t                                                    mNumOutput    = 0;
        Array<ITexture::Format, FrameBuffer::cColorRenderTargetMax> mOutputFormats;
        const IInputLayout*                                         mpInputLayout = nullptr;

        const uint8_t* mpVertexShaderBuffer        = nullptr;
        const uint8_t* mpGeometryShaderBuffer      = nullptr;
        const uint8_t* mpPixelShaderBuffer         = nullptr;
        const uint8_t* mpComputeShaderBuffer       = nullptr;
        const uint8_t* mpDomainShaderBuffer        = nullptr;
        const uint8_t* mpHullShaderBuffer          = nullptr;
        const uint8_t* mpAmplificationShaderBuffer = nullptr;
        const uint8_t* mpMeshShaderBuffer          = nullptr;
        uint32_t       mVertexShaderBufferSize        = 0;
        uint32_t       mGeometryShaderBufferSize      = 0;
        uint32_t       mPixelShaderBufferSize         = 0;
        uint32_t       mComputeShaderBufferSize       = 0;
        uint32_t       mDomainShaderBufferSize        = 0;
        uint32_t       mHullShaderBufferSize          = 0;
        uint32_t       mAmplificationShaderBufferSize = 0;
        uint32_t       mMeshShaderBufferSize          = 0;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
