/*!
 * @file   balBitmapFont.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// std
#include <uchar.h>
// bal
#include <container/balStringPtr.h>
#include <font/balBitmapFont.h>

namespace bal::font {

// ----------------------------------------------------------------------------
Bitmap::Bitmap()
{

}
// ----------------------------------------------------------------------------
Bitmap::~Bitmap()
{

}
// ----------------------------------------------------------------------------
void Bitmap::drawSetup(const StringPtr& str)
{
    // テクスチャーの構成
    //
    // [  0x20～  0x7F] ASCII
    // [  0x80～  0xDF] 半角カナ（0x80～0x9Fは空白）
    // [0x8140～0x816F] JIS
    // [0x8170～0x819F] JIS
    // [0x81A0～0x81CF] JIS
    // [0x81D0～0x81FF] JIS
    // [0x8240～0x826F] JIS
    // ...
    // ...
    // [0x9FD0～0x9FFF] JIS
    // [0xE040～0xE06F] JIS
    // [0xE070～0xE09F] JIS
    // [0xE0A0～0xE0CF] JIS
    // [0xE0D0～0xE0FF] JIS
    // [0xE140～0xE16F] JIS
    // ...
    // ...
    // [0xFCD0～0xFCFF] JIS

    // SJIS 規格
    for (size_t i_str = 0; i_str < str.length(); ++i_str)
    {
        // 0x81～0x9F, 0xE0～0xFF だとマルチバイト文字
        if ((0x81 <= str.c_str()[i_str] && str.c_str()[i_str] <= 0x9F) ||
            (0xE0 <= str.c_str()[i_str] && str.c_str()[i_str] <= 0xFF))
        {
            char low = str.c_str()[i_str];
            ++i_str;
            // マルチバイトなのに次はNULL文字なら壊れている
            if (str.c_str()[i_str] == 0x0)
            {

            }
            else if (str.c_str()[i_str] < 0x40)
            {
                // 0x00～0x3F は予約領域
            }
            else if (str.c_str()[i_str] != 0x0)
            {
                char high = str.c_str()[i_str];
            }
        }
        // NULL文字
        else if (str.c_str()[i_str] == 0x0)
        {
        }
        // 表示不可文字
        else if (str.c_str()[i_str] < 0x20)
        {
        }
        // ASCII
        else
        {
            // 全角スペース
            if (str.c_str()[i_str] == 0x80)
            {
            }
            // 半角
            else
            {
            }
        }
    }
}
// ----------------------------------------------------------------------------
void Bitmap::draw() const
{

}
// ----------------------------------------------------------------------------

} // bal::font
