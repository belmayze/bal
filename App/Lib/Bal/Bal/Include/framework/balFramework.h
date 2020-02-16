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
#include <memory/balUniquePtr.h>

// 前方宣言
namespace bal { class HeapBase; }

// ----------------------------------------------------------------------------
namespace bal {

class Framework
{
public:
    //! 初期化構造体
    struct InitializeArg
    {
        const StringPtr mTitle;
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
     * @param[in] arg 初期化引数構造体
     */
    void initialize(const InitializeArg& arg);

private:
    UniquePtr<HeapBase> mpMainHeap;
};

}
