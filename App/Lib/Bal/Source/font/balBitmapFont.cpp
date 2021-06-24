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
    // [0x20～0x7F] ASCII
    // [0xA0～0xE0] 半角
    // [0x8140～]
    // ...
    // [0x84FF]
    // SJIS 規格
    BAL_LOG_INFO("[%s]\n", str.c_str());
    for (size_t i_str = 0; i_str < str.length(); ++i_str)
    {
        int row_index = -1, col_index = -1;
        float space = 1.f;

        uint8_t c0 = str.c_str()[i_str];
        uint8_t c1 = 0x0;

        // 0x81～0x9F, 0xE0～0xFF だとマルチバイト文字
        if ((0x81 <= c0 && c0 <= 0x9F) ||
            (0xE0 <= c0 && c0 <= 0xFF))
        {
            i_str++;
            c1 = str.c_str()[i_str];

            // マルチバイトなのに次はNULL文字なら壊れている
            if (c1 < 0x40)
            {
                // 0x00～0x3F は未使用領域
            }
            else if (c0 == 0x81 && c1 == 0x40)
            {
                // 全角スペース
                row_index = 2;
                col_index = 0;
            }
            else
            {
                // 全角
                // 0x8100～0x84FF
                // ただし0x00～0x3Fは未使用のため飛ばす
                // 改行は 0x7F, 0xBF, 0xFF で行われる
                if (0x81 <= c0 && c0 <= 0x84)
                {
                    // 全角数字、ひらカタ、ギリシャ文字、ロシア文字、罫線
                    row_index = 2 + (c0 - 0x81) * 3 + (c1 - 0x40) / 0x40;
                    col_index = (c1 - 0x40) % 0x40;
                }
                else if (0x87 == c0)
                {
                    // 囲み英数字、ローマ字、単位、省略文字、数字記号
                    row_index = 14 + (c1 - 0x40) / 0x40;
                    col_index = (c1 - 0x40) % 0x40;
                }
                else if (0x88 <= c0 && c0 <= 0x9F)
                {
                    // 0x88A0～ 第一水準漢字
                    // 0x98A0～ 第二水準漢字
                    row_index = 17 + (c0 - 0x88) * 3 + (c1 - 0x40) / 0x40;
                    col_index = (c1 - 0x40) % 0x40;
                }
                else if (0xE0 <= c0 && c0 <= 0xEA)
                {
                    // 第二水準漢字
                    row_index = 89 + (c0 - 0xE0) * 3 + (c1 - 0x40) / 0x40;
                    col_index = (c1 - 0x40) % 0x40;
                }
                else if (0xFA <= c0 && c0 <= 0xFC)
                {
                    // IBM拡張文字
                    row_index = 122 + (c0 - 0xFA) * 3 + (c1 - 0x40) / 0x40;
                    col_index = (c1 - 0x40) % 0x40;
                }
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
                row_index = 0;
                col_index = 0;
            }
            // 半角
            else
            {
                // 0x80、0xA0 などはそのまま使用しても半角スペースにできるので計算する
                row_index = (c0 - 0x20) / 0x80;
                col_index = (c0 - 0x20) % 0x80;
            }
        }

        // @TODO: row_index, col_index を定数バッファに入れてレンダリングする
    }
}
// ----------------------------------------------------------------------------
void Bitmap::draw() const
{

}
// ----------------------------------------------------------------------------

} // bal::font
