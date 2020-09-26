/*!
 * @file   balFrameworkCallback.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <time/balTimeSpan.h>

namespace bal { class FrameBuffer; }
namespace bal { class ICommandListDirect; }

// ----------------------------------------------------------------------------
namespace bal {

class FrameworkCallback
{
public:
    //! 計算コールバック構造体
    struct UpdateArg
    {
        TimeSpan mDiff;
    };
    //! 描画コールバック構造体
    struct DrawArg
    {
        FrameBuffer*        mpSwapChainFrameBuffer = nullptr;
        FrameBuffer*        mpDefaultFrameBuffer   = nullptr;
        ICommandListDirect* mpCommandList          = nullptr;
    };

public:
    /*!
     * 計算処理するコールバック
     */
    virtual void onUpdate(const UpdateArg& arg) = 0;

    /*!
     * 描画処理するコールバック
     */
    virtual void onDraw(const DrawArg& arg) = 0;
};

}
