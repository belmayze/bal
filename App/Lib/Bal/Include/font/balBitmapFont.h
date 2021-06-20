/*!
 * @file   balBitmapFont.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class StringPtr; }

// ----------------------------------------------------------------------------
namespace bal::font {

class Bitmap
{
public:
    //! コンストラクター
    Bitmap();

    //! デストラクター
    ~Bitmap();

    /*!
     * 文字列の描画登録を行います
     * @param[in] str 文字列
     */
    void drawSetup(const StringPtr& str);

    /*!
     * 描画を行います
     */
    void draw() const;
};

} // bal::font
