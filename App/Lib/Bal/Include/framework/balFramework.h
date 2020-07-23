/*!
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

// ----------------------------------------------------------------------------
namespace bal {

class Framework
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        StringPtr mTitle    = ""; //!< タイトル名
        size_t    mHeapSize = 0;  //!< アプリケーションが使用する全体メモリー量
        MathSize  mRenderSize;    //!< レンダリングサイズ
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

private:
    std::unique_ptr<heap::RootBlock, heap::RootBlock::Deleter> mpRootHeap; //!< ルートヒープ

    HWND mHwnd; //! ウィンドウハンドル
};

}
