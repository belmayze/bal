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

namespace bal { class File; }
namespace bal::gfx { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class IPipeline
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        IGraphics*  mpGraphics = nullptr;
        const File* mpVSFile   = nullptr;
        const File* mpPSFile   = nullptr;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;
};

}
