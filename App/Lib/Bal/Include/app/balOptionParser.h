/*!
 * @file   balOptionParser.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>

namespace bal
{

class OptionParser
{
public:
    /*!
     * 文字列からオプションをパースします
     * @param[in] argc
     * @param[in] argv
     */
    void parse(int argc, const char* argv[]);

public:
    //! オプションの個数を取得
    uint32_t getNumOption() const { return mNumOption; }
    //! オプションを取得
    const std::pair<StringPtr, StringPtr>* getOptions() const { return mpOptions.get(); }

private:
    uint32_t                                           mNumOption = 0;
    std::unique_ptr<std::pair<StringPtr, StringPtr>[]> mpOptions;
};

}
