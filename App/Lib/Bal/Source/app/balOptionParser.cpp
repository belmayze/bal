/*!
 * @file   balOptionParser.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <app/balOptionParser.h>

namespace bal
{

// ----------------------------------------------------------------------------

void OptionParser::parse(int argc, const char* argv[])
{
    // 解析
    uint32_t option_index = 0;
    std::unique_ptr<std::unique_ptr<std::pair<StringPtr, StringPtr>>[]> arg_list = make_unique<std::unique_ptr<std::pair<StringPtr, StringPtr>>[]>(nullptr, argc);

    const char* p_token = nullptr;
    for (int i_arg = 0; i_arg < argc; ++i_arg)
    {
        if (*argv[i_arg] == '-')
        {
            // - から始まればオプションキー
            if (p_token)
            {
                // token があれば、無指定でオプション追加
                arg_list[option_index] = make_unique<std::pair<StringPtr, StringPtr>>(nullptr, p_token, StringPtr());
                option_index++;
            }

            p_token = argv[i_arg];
        }
        else
        {
            // それ以外はオプションバリュー
            arg_list[option_index] = make_unique<std::pair<StringPtr, StringPtr>>(nullptr, p_token ? p_token : StringPtr(), argv[i_arg]);
            option_index++;
            p_token = nullptr;
        }
    }

    // 解析結果を入れる
    mNumOption = option_index;
    mpOptions = make_unique<std::pair<StringPtr, StringPtr>[]>(nullptr, option_index);
    for (uint32_t i_option = 0; i_option < mNumOption; ++i_option)
    {
        mpOptions[i_option] = *arg_list[i_option];
    }
}

// ----------------------------------------------------------------------------

}
