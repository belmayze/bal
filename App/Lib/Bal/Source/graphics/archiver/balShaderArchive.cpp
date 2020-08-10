/*!
 * @file   balShaderArchive.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// bal
#include <graphics/archiver/balShaderArchive.h>

namespace bal::gfx {

// ----------------------------------------------------------------------------

bool ShaderArchive::loadArchiver(File&& file)
{
    // 保持
    mFile = std::move(file);

    // ヘッダー読み込み
    Header header;
    std::memcpy(&header, mFile.getBuffer(), sizeof(Header));

    // マジックチェック
    if (header.mMagic[0] != 'B' || header.mMagic[1] != 'S' || header.mMagic[2] != 'H' || header.mMagic[3] != 'A')
    {
        return false;
    }
    
    // メモリー確保
    mNumProgram     = header.mNumPrograms;
    mNameTable      = make_unique<StringPtr[]>(nullptr, mNumProgram);
    mShaderDataList = make_unique<ShaderContainer[]>(nullptr, mNumProgram);

    // タグ確認
    const uint8_t* ptr = mFile.getBuffer() + sizeof(Header);
    std::unique_ptr<Tag[]> p_tags = make_unique<Tag[]>(nullptr, mNumProgram);
    for (uint32_t i_tag = 0; i_tag < mNumProgram; ++i_tag)
    {
        std::memcpy(&p_tags[i_tag], ptr, sizeof(Tag));
        ptr += sizeof(Tag);
    }

    // テーブル作成
    for (uint32_t i_program = 0; i_program < mNumProgram; ++i_program)
    {
        // データ
        const Tag&       tag      = p_tags[i_program];
        ShaderContainer* p_out    = &mShaderDataList[i_program];
        const uint8_t*   data_ptr = mFile.getBuffer() + tag.mOffset;

        // 名前
        mNameTable[i_program] = StringPtr(reinterpret_cast<const char*>(data_ptr), tag.mNameSize - 1);
        data_ptr += tag.mNameSize;

        // 各種プログラム
        if (tag.mVertexShaderSize > 0)
        {
            p_out->mVertexShader.mBuffer     = data_ptr;
            p_out->mVertexShader.mBufferSize = tag.mVertexShaderSize;
            data_ptr += tag.mVertexShaderSize;
        }
        if (tag.mGeometryShaderSize > 0)
        {
            p_out->mGeometryShader.mBuffer     = data_ptr;
            p_out->mGeometryShader.mBufferSize = tag.mGeometryShaderSize;
            data_ptr += tag.mGeometryShaderSize;
        }
        if (tag.mPixelShaderSize > 0)
        {
            p_out->mPixelShader.mBuffer     = data_ptr;
            p_out->mPixelShader.mBufferSize = tag.mPixelShaderSize;
            data_ptr += tag.mPixelShaderSize;
        }
        if (tag.mComputeShaderSize > 0)
        {
            p_out->mComputeShader.mBuffer     = data_ptr;
            p_out->mComputeShader.mBufferSize = tag.mComputeShaderSize;
            data_ptr += tag.mComputeShaderSize;
        }
        if (tag.mDomainShaderSize > 0)
        {
            p_out->mDomainShader.mBuffer     = data_ptr;
            p_out->mDomainShader.mBufferSize = tag.mDomainShaderSize;
            data_ptr += tag.mDomainShaderSize;
        }
        if (tag.mHullShaderSize > 0)
        {
            p_out->mHullShader.mBuffer     = data_ptr;
            p_out->mHullShader.mBufferSize = tag.mHullShaderSize;
            data_ptr += tag.mHullShaderSize;
        }
        if (tag.mAmplificationShaderSize > 0)
        {
            p_out->mAmplificationShader.mBuffer     = data_ptr;
            p_out->mAmplificationShader.mBufferSize = tag.mAmplificationShaderSize;
            data_ptr += tag.mAmplificationShaderSize;
        }
        if (tag.mMeshShaderSize > 0)
        {
            p_out->mMeshShader.mBuffer     = data_ptr;
            p_out->mMeshShader.mBufferSize = tag.mMeshShaderSize;
            data_ptr += tag.mMeshShaderSize;
        }
    }

    return true;
}

// ----------------------------------------------------------------------------

uint32_t ShaderArchive::findProgram(const StringPtr& name) const
{
    for (uint32_t i_program = 0; i_program < mNumProgram; ++i_program)
    {
        if (mNameTable[i_program].isEquals(name))
        {
            return i_program;
        }
    }
    return -1;
}

// ----------------------------------------------------------------------------

const ShaderArchive::ShaderContainer& ShaderArchive::getShaderContainer(uint32_t index) const
{
    return mShaderDataList[index];
}

// ----------------------------------------------------------------------------

}
