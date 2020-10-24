/*!
 * @file   balIInputLayout.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>
#include <container/balStringPtr.h>

namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal {

class IInputLayout
{
public:
    //! インプットレイアウトの種類
    enum class Type
    {
        Int,
        IVec2,
        IVec3,
        IVec4,
        UInt,
        UVec2,
        UVec3,
        UVec4,
        Float,
        Vec2,
        Vec3,
        Vec4
    };

    //! インプットレイアウト
    struct InputLayoutDesc
    {
        StringPtr mName                 = "";
        int       mSemanticIndex        = 0;
        Type      mType                 = Type::Vec3;
        int       mSlotIndex            = 0;
        uint32_t  mOffset               = 0;
        uint32_t  mInstanceDataStepRate = 0;
    };

    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*             mpGraphics      = nullptr;
        const InputLayoutDesc* mpInputLayouts  = nullptr;
        uint32_t               mNumInputLayout = 0;
    };

public:
    //! デストラクター
    virtual ~IInputLayout() {}

    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
