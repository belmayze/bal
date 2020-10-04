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
#include <graphics/balIMeshBuffer.h>
#include <graphics/balITexture.h>

namespace bal { class File; }
namespace bal { class IDescriptorHeap; }
namespace bal { class IGraphics; }
namespace bal { class IInputLayout; }

// ----------------------------------------------------------------------------
namespace bal {

class IPipeline
{
public:
    //! デプス設定
    struct DepthSettings
    {
        bool mEnableTest  = false;
        bool mEnableWrite = false;
    };
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*                                                  mpGraphics         = nullptr;
        uint32_t                                                    mNumOutput         = 0;
        Array<ITexture::Format, FrameBuffer::cColorRenderTargetMax> mOutputFormats;
        ITexture::Format                                            mDepthFormat       = ITexture::Format::D32_FLOAT;
        const IInputLayout*                                         mpInputLayout      = nullptr;
        IMeshBuffer::PrimitiveTopology                              mPrimitiveTopology = IMeshBuffer::PrimitiveTopology::Triangles;

        DepthSettings           mDepthSettings;

        uint32_t                mNumDescriptorHeap = 0;       //!< デスクリプターヒープの数
        const IDescriptorHeap** mpDescriptorHeaps  = nullptr; //!< デスクリプターヒープ

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
    //! デストラクター
    virtual ~IPipeline() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
