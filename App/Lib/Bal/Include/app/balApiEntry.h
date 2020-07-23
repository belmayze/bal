/*!
 * @file   balApiEntry.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// std
#include <memory>
// bal
#include <container/balStringPtr.h>

namespace bal { class ApplicationBase; }

namespace bal
{

class ApiEntry
{
public:
    //! 起動モード
    enum class BootMode
    {
        Console,
        Application
    };

    //! 起動引数
    struct BootArg
    {
        BootMode mBootMode = BootMode::Console; //!< 起動モード
    };

public:
    /*!
     * 起動オプションを解析します
     * @param[in] argc オプションの数
     * @param[in] argv オプションのリスト
     */
    void setupOptions(int argc, char* argv[]);

    /*!
     * ブート開始
     * @param[in] p_application アプリケーション
     */
    int boot(ApplicationBase* p_application);

public:
    //! 起動モードを取得します
    BootMode getBootMode() const { return mBootArg.mBootMode; }

private:
    std::unique_ptr<std::pair<StringPtr, std::unique_ptr<StringPtr[]>>[]> mOptions; //!< 起動オプション
    BootArg                                                               mBootArg; //!< 起動引数
};

}
