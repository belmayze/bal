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

// ----------------------------------------------------------------------------
namespace bal {

class Engine : public FrameworkCallback
{
public:
    //! 初期化構造体
    struct InitializeArg
    {

    };

public:
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
};

}
