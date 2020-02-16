/*!
 * @file   balStringBase.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

/*!
 * 文字列の基底クラス
 */
class StringBase
{
public:
    /*! デストラクター */
    virtual ~StringBase() {}

    /*!
     * 文字列ポインター取得
     */
    virtual const char* c_str() const = 0;

    /*!
     * 文字列の長さを取得
     */
    virtual std::size_t size() const = 0;

    /*!
     * 文字列の長さを取得
     */
    virtual std::size_t length() const = 0;

    /*!
     * 空文字かチェック
     */
    virtual bool empty() const = 0;

};

}
