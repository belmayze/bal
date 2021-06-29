/*!
 * @file   balShaderArchive.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <io/balFile.h>

namespace bal { class StringPtr; }

// ----------------------------------------------------------------------------
namespace bal {

class ShaderArchive
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
        uint32_t mOffset                  = 0;
        uint32_t mNameSize                = 0;
        uint32_t mVertexShaderSize        = 0;
        uint32_t mGeometryShaderSize      = 0;
        uint32_t mPixelShaderSize         = 0;
        uint32_t mComputeShaderSize       = 0;
        uint32_t mDomainShaderSize        = 0;
        uint32_t mHullShaderSize          = 0;
        uint32_t mAmplificationShaderSize = 0;
        uint32_t mMeshShaderSize          = 0;
    };
    //! シェーダーのデータ
    struct ShaderData
    {
        uint32_t       mBufferSize = 0;
        const uint8_t* mBuffer     = nullptr;
    };
    //! シェーダーのコンテナ
    struct ShaderContainer
    {
        ShaderData mVertexShader;
        ShaderData mGeometryShader;
        ShaderData mPixelShader;
        ShaderData mComputeShader;
        ShaderData mDomainShader;
        ShaderData mHullShader;
        ShaderData mAmplificationShader;
        ShaderData mMeshShader;
    };

public:
    ShaderArchive() {}
    ~ShaderArchive() {}

    /*!
     * アーカイバを読み込みます
     * @param[in] file ファイル
     */
    bool loadArchiver(File&& file);

    /*!
     * プログラムを検索します
     * @param[in] name プログラム名
     */
    uint32_t findProgram(const StringPtr& name) const;

    /*!
     * シェーダーを取得します
     * @param[in] index シェーダーインデックス
     */
    const ShaderContainer& getShaderContainer(uint32_t index) const;

private:
    File                               mFile;
    std::uint16_t                      mNumProgram = 0;
    std::unique_ptr<StringPtr[]>       mNameTable;
    std::unique_ptr<ShaderContainer[]> mShaderDataList;
};

}
