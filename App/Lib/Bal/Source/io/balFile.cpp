/*!
 * @file   balFile.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// std
#include <fstream>
// bal
#include <io/balFile.h>

namespace bal {

// ----------------------------------------------------------------------------

bool File::loadFromFile(const StringPtr& path)
{
    // ファイルをバイナリとして読み込み
    std::ifstream stream(path.c_str(), std::ios::binary);
    if (!stream) { return false; }

    // ファイルサイズを取得
    stream.seekg(0, std::ios::end);
    std::streampos end_pos = stream.tellg();
    stream.seekg(0, std::ios::beg);

    // ファイル読み込み
    std::unique_ptr<uint8_t[]> buffer = make_unique<uint8_t[]>(nullptr, end_pos);
    if (!buffer) { return false; }
    stream.read(reinterpret_cast<char*>(buffer.get()), end_pos);

    // ファイルを閉じる
    stream.close();

    // 保持
    mpBuffer    = std::move(buffer);
    mBufferSize = end_pos;

    return true;
}

// ----------------------------------------------------------------------------

bool File::WriteToFile(const StringPtr& path, uint8_t* p_buffer, size_t buffer_size)
{
    // ファイルをバイナリとして読み込み
    std::ofstream stream(path.c_str(), std::ios::binary);
    if (!stream) { return false; }

    // ファイルを書き出し
    stream.write(reinterpret_cast<const char*>(p_buffer), buffer_size);

    // ファイルを閉じる
    stream.close();

    return true;
}

// ----------------------------------------------------------------------------

File& File::operator=(File&& rhs)
{
    mpBuffer    = std::move(rhs.mpBuffer);
    mBufferSize = rhs.mBufferSize;
    return *this;
}

// ----------------------------------------------------------------------------

}
