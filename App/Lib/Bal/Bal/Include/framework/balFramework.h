﻿/*!
 * @file   balFramework.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

class Framework
{
public:
    //! 初期化構造体
    struct InitializeArg
    {

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
};

}