/*!
 * @file   balLog.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

class Log
{
public:
    //! ログの種類
    enum EType
    {
        Debug,   //!< デバッグ
        Info,    //!< 情報
        Warning, //!< 警告
        Error    //!< エラー
    };

    /*!
     * ログを出力する
     * @param[in] str     文字列
     * @param[in] formats フォーマット
     */
    template <typename... Formats>
    static void Print(Formats... args);

private:
    /*!
     * ログを出力する
     * @param[in] str 文字列
     */
    static void Print_(char* str);
};

}

#include <io/balLog_inl.h>
