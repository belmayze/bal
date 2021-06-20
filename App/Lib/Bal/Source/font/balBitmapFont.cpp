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
    int row_index, col_index;
    float space = 1.f;

    // テクスチャーの構成
    // [0x20～0x7F] ASCII
    // [0xA0～0xE0] 半角
    // [0x8140～]
    // ...
    // [0x84FF]
    // SJIS 規格
    for (size_t i_str = 0; i_str < str.length(); ++i_str)
    {
        uint32_t c0 = str.c_str()[i_str];

        // 0x81～0x9F, 0xE0～0xFF だとマルチバイト文字
        if ((0x81 <= c0 && c0 <= 0x9F) ||
            (0xE0 <= c0 && c0 <= 0xFF))
        {
            i_str++;
            uint32_t c1 = str.c_str()[i_str];

            // マルチバイトなのに次はNULL文字なら壊れている
            if (c1 < 0x40)
            {
                // 0x00～0x3F は未使用領域
            }
            else if (c0 == 0x81 && c1 == 0x40)
            {
                // 全角スペース
            }
            else
            {
                // 全角 @TODO:
            }
        }
        // NULL文字
        else if (c0 == 0x0)
        {
        }
        // 制御文字
        else if (c0 < 0x20)
        {
            // @TODO: \n \t など
        }
        // ASCII
        else
        {
            space = 0.5f;

            // 半角スペース
            if (c0 == 0x20)
            {
            }
            // 半角
            else
            {
                // 0x80、0xA0 などはそのまま使用しても半角スペースにできるので計算する
                row_index = (c0 - 0x20) / 0x80;
                col_index = (c0 - 0x20) % 0x80;
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
