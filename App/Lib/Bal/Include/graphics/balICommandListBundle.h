/*!
 * @file   balICommandListBundle.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class IGraphics; }
namespace bal { class IPipeline; }

// ----------------------------------------------------------------------------
namespace bal {

class ICommandListBundle
{
public:
    // 初期化
    struct InitializeArg
    {
        IGraphics* mpGraphics = nullptr;
    };

public:
    /*!
     * 初期化の処理を記述します
     * @param[in] arg 初期化構造体
     */
    virtual bool initialize(const InitializeArg& arg) = 0;

};

}
