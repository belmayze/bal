/*!
 * @file   balIDescriptorTable.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal::gfx { class IGraphics; }
namespace bal::gfx { class ITexture; }
namespace bal::gfx { class IConstantBuffer; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class IDescriptorTable
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*              mpGraphics         = nullptr;
        uint32_t                mNumTexture        = 0;
        const ITexture**        mpTextures         = nullptr;
        uint32_t                mNumConstantBuffer = 0;
        const IConstantBuffer** mpConstantBuffers  = nullptr;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
