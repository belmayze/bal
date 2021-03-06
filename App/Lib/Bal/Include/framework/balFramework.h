﻿/*!
 * @file   balFramework.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>
#include <heap/balHeapRootBlock.h>
#include <math/balMath.h>
#include <memory/balUniquePtr.h>

namespace bal { class ApiEntry; }
namespace bal { class ApplicationBase; }
namespace bal { class FrameworkCallback; }
namespace bal { class IGraphics; }

// ----------------------------------------------------------------------------
namespace bal {

class Framework
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        StringPtr        mTitle             = "";      //!< タイトル名
        MathSize         mRenderSize;                  //!< レンダリングサイズ
        uint32_t         mRenderBufferCount = 2;       //!< レンダリングバッファー数
        ApplicationBase* mpApplication      = nullptr; //!< アプリケーション
    };

public:
    /*!
     * コンストラクター
     */
    Framework();

    /*!
     * デストラクター
     */
    ~Framework();

    /*!
     * 初期化
     * @param[in] api_entry 起動状態
     * @param[in] arg       初期化引数構造体
     */
    void initialize(const ApiEntry& api_entry, const InitializeArg& arg);

    /*!
     * アプリケーションループに入ります
     * @param[in] p_callback コールバック
     * @return 終了コードを返します
     */
    int enterApplicationLoop(FrameworkCallback* p_callback);

public:
    /*! グラフィックス取得 */
    IGraphics* getGraphics() { return mpGraphics.get(); }

private:
    /*!
     * グラフィックスのアプリケーションループ
     */
    void applicationLoop_();

private:
    heap::RootBlock*           mpRootHeap = nullptr; //!< ルートヒープ

    ApplicationBase*           mpApplication = nullptr;
    FrameworkCallback*         mpCallback    = nullptr;
    std::unique_ptr<IGraphics> mpGraphics;
    float                      mFrameRate          = 0.f;
    float                      mFrameRatePerMinute = 0.f;

    HWND mHwnd;                //!< ウィンドウハンドル
    bool mInitialized = false; //!< 初期化完了フラグ
    bool mEnableLoop  = false; //!< ループ処理可能フラグ
};

}
