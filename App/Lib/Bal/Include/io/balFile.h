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

    /*!
     * ファイルを書き出します
     * @param[in] path        書き出しパス
     * @param[in] p_buffer    バッファ
     * @param[in] buffer_size 書き出しファイルサイズ
     */
    static bool WriteToFile(const StringPtr& path, uint8_t* p_buffer, size_t buffer_size);

public:
    // move
    File& operator=(File&& rhs);

private:
    std::unique_ptr<uint8_t[]> mpBuffer;
    size_t                     mBufferSize = 0;
};

}

// ----------------------------------------------------------------------------
