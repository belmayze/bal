/*!
 * @file   balFile.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

namespace bal { class StringPtr; }

// ----------------------------------------------------------------------------
namespace bal {

class File
{
public:
    /*!
     * ファイルを読み込みます
     * @param[in] path ファイルパス
     */
    bool loadFromFile(const StringPtr& path);

    /*!
     * ファイルバッファを取得します
     */
    const uint8_t* const getBuffer() const { return mpBuffer.get(); }

    /*!
     * バッファサイズを取得します
     */
    const size_t getBufferSize() const { return mBufferSize; }

private:
    std::unique_ptr<uint8_t[]> mpBuffer;
    size_t                     mBufferSize = 0;
};

}

// ----------------------------------------------------------------------------
