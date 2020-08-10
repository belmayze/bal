/*!
 * @file   balFrameBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <io/balFile.h>

namespace bal { class File; }
namespace bal { class StringPtr; }

// ----------------------------------------------------------------------------
namespace bal::gfx {

class ShaderArchiver
{
    // ファイル構成
    //
    // + ヘッダー
    // + タグ0    (プログラム分)
    //   タグ1
    //   タグ2
    //   ...
    // + データ0  (プログラム分)
    //   データ1
    //   データ2
    //   ...

public:
    //! ヘッダー
    struct Header
    {
        uint8_t  mMagic[4]    = {'B', 'S', 'H', 'A'}; // bal shader archive
        uint16_t mVersion     = 1;
        uint16_t mNumPrograms = 0;
    };
    //! タグ
    struct Tag
    {
        uint32_t mOffset   = 0;
        uint32_t mNameSize = 0;
        uint32_t mVsSize   = 0; //! Vertex Shader
        uint32_t mGsSize   = 0; //! Geometry Shader
        uint32_t mPsSize   = 0; //! Pixel Shader
        uint32_t mCsSize   = 0; //! Compute Shader
        uint32_t mDsSize   = 0; //! Domain Shader
        uint32_t mHsSize   = 0; //! Hull Shader 
        uint32_t mAsSize   = 0; //! Amplification Shader
        uint32_t mMsSize   = 0; //! Mesh Shader
    };
    //! シェーダーのデータ
    struct ShaderData
    {
        uint32_t       mBufferSize = 0;
        const uint8_t* mBuffer     = nullptr;
    };

public:
    /*!
     * アーカイバを読み込みます
     * @param[in] file ファイル
     */
    void loadArchiver(File&& file);

    /*!
     * プログラムを検索します
     * @param[in] name プログラム名
     */
    uint32_t findProgram(const StringPtr& name) const;

    /*!
     * シェーダーを取得します
     * @param[in] index シェーダーインデックス
     */
    const ShaderData& getShaderData(uint32_t index) const;

private:
    File                          mFile;
    std::uint16_t                 mNumProgram = 0;
    std::unique_ptr<StringPtr[]>  mNameTable;
    std::unique_ptr<ShaderData[]> mShaderDataList;
};

}
