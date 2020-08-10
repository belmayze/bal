/*!
 * @file   balApiEntry.cpp
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
// app
#include "Archiver.h"

namespace app
{

// ----------------------------------------------------------------------------

void Archiver::initialize(const InitializeArg& arg)
{
    mHeader.mNumPrograms = arg.mNumShader;
    mTags                = bal::make_unique<bal::gfx::ShaderArchiver::Tag[]>(nullptr, mHeader.mNumPrograms);
    mShaders             = bal::make_unique<ShaderContainer[]>(nullptr, mHeader.mNumPrograms);
}

// ----------------------------------------------------------------------------

void Archiver::setShader(const ShaderArg& arg)
{
    ShaderContainer& container = mShaders[arg.mShaderIndex];
    bal::gfx::ShaderArchiver::Tag& tag = mTags[arg.mShaderIndex];

    // プログラム名
    container.mProgramName = bal::StringBuffer(arg.mShaderName.c_str(), arg.mShaderName.size());
    tag.mNameSize = static_cast<uint32_t>(container.mProgramName.size() + 1);

    // ファイル読み込み
    if (!arg.mVertexShaderFilepath.isEmpty())
    {
        container.mVertexShader.loadFromFile(arg.mVertexShaderFilepath);
        tag.mVertexShaderSize = static_cast<uint32_t>(container.mVertexShader.getBufferSize());
    }
    if (!arg.mGeometryShaderFilepath.isEmpty())
    {
        container.mGeometryShader.loadFromFile(arg.mGeometryShaderFilepath);
        tag.mGeometryShaderSize = static_cast<uint32_t>(container.mGeometryShader.getBufferSize());
    }
    if (!arg.mPixelShaderFilepath.isEmpty())
    {
        container.mPixelShader.loadFromFile(arg.mPixelShaderFilepath);
        tag.mPixelShaderSize = static_cast<uint32_t>(container.mPixelShader.getBufferSize());
    }
    if (!arg.mComputeShaderFilepath.isEmpty())
    {
        container.mComputeShader.loadFromFile(arg.mComputeShaderFilepath);
        tag.mComputeShaderSize = static_cast<uint32_t>(container.mComputeShader.getBufferSize());
    }
    if (!arg.mDomainShaderFilepath.isEmpty())
    {
        container.mDomainShader.loadFromFile(arg.mDomainShaderFilepath);
        tag.mDomainShaderSize = static_cast<uint32_t>(container.mDomainShader.getBufferSize());
    }
    if (!arg.mHullShaderFilepath.isEmpty())
    {
        container.mHullShader.loadFromFile(arg.mHullShaderFilepath);
        tag.mHullShaderSize = static_cast<uint32_t>(container.mHullShader.getBufferSize());
    }
    if (!arg.mAmplificationShaderFilepath.isEmpty())
    {
        container.mAmplificationShader.loadFromFile(arg.mAmplificationShaderFilepath);
        tag.mAmplificationShaderSize = static_cast<uint32_t>(container.mAmplificationShader.getBufferSize());
    }
    if (!arg.mMeshShaderFilepath.isEmpty())
    {
        container.mMeshShader.loadFromFile(arg.mMeshShaderFilepath);
        tag.mMeshShaderSize = static_cast<uint32_t>(container.mMeshShader.getBufferSize());
    }
}

// ----------------------------------------------------------------------------

void Archiver::archive(const bal::StringPtr& output_path)
{
    // ファイルサイズを計算
    size_t memory_size = sizeof(bal::gfx::ShaderArchiver::Header)
                       + sizeof(bal::gfx::ShaderArchiver::Tag) * mHeader.mNumPrograms;
    for (int i_program = 0; i_program < mHeader.mNumPrograms; ++i_program)
    {
        // オフセット計算しつつ、メモリーサイズも求める
        bal::gfx::ShaderArchiver::Tag& tag = mTags[i_program];
        tag.mOffset = static_cast<uint32_t>(memory_size);
        memory_size += tag.mNameSize
                    +  tag.mVertexShaderSize
                    +  tag.mGeometryShaderSize
                    +  tag.mPixelShaderSize
                    +  tag.mComputeShaderSize
                    +  tag.mDomainShaderSize
                    +  tag.mHullShaderSize
                    +  tag.mAmplificationShaderSize
                    +  tag.mMeshShaderSize;
    }

    // ファイルバッファを確保
    std::unique_ptr<uint8_t[]> buffer = bal::make_unique<uint8_t[]>(nullptr, memory_size);

    // ヘッダー書き出し
    uint8_t* ptr = buffer.get();
    std::memcpy(ptr, &mHeader, sizeof(bal::gfx::ShaderArchiver::Header));
    ptr += sizeof(bal::gfx::ShaderArchiver::Header);
    std::memcpy(ptr, mTags.get(), sizeof(bal::gfx::ShaderArchiver::Tag) * mHeader.mNumPrograms);
    ptr += sizeof(bal::gfx::ShaderArchiver::Tag) * mHeader.mNumPrograms;

    // シェーダー書き出し
    for (int i_program = 0; i_program < mHeader.mNumPrograms; ++i_program)
    {
        const ShaderContainer& container = mShaders[i_program];
        const bal::gfx::ShaderArchiver::Tag& tag = mTags[i_program];
        
        // 名前
        std::memcpy(ptr, container.mProgramName.c_str(), tag.mNameSize);
        ptr += tag.mNameSize;

        if (tag.mVertexShaderSize > 0)
        {
            std::memcpy(ptr, container.mVertexShader.getBuffer(), container.mVertexShader.getBufferSize());
            ptr += container.mVertexShader.getBufferSize();
        }
        if (tag.mGeometryShaderSize > 0)
        {
            std::memcpy(ptr, container.mGeometryShader.getBuffer(), container.mGeometryShader.getBufferSize());
            ptr += container.mGeometryShader.getBufferSize();
        }
        if (tag.mPixelShaderSize > 0)
        {
            std::memcpy(ptr, container.mPixelShader.getBuffer(), container.mPixelShader.getBufferSize());
            ptr += container.mPixelShader.getBufferSize();
        }
        if (tag.mComputeShaderSize > 0)
        {
            std::memcpy(ptr, container.mComputeShader.getBuffer(), container.mComputeShader.getBufferSize());
            ptr += container.mComputeShader.getBufferSize();
        }
        if (tag.mDomainShaderSize > 0)
        {
            std::memcpy(ptr, container.mDomainShader.getBuffer(), container.mDomainShader.getBufferSize());
            ptr += container.mDomainShader.getBufferSize();
        }
        if (tag.mHullShaderSize > 0)
        {
            std::memcpy(ptr, container.mHullShader.getBuffer(), container.mHullShader.getBufferSize());
            ptr += container.mHullShader.getBufferSize();
        }
        if (tag.mAmplificationShaderSize > 0)
        {
            std::memcpy(ptr, container.mAmplificationShader.getBuffer(), container.mAmplificationShader.getBufferSize());
            ptr += container.mAmplificationShader.getBufferSize();
        }
        if (tag.mMeshShaderSize > 0)
        {
            std::memcpy(ptr, container.mMeshShader.getBuffer(), container.mMeshShader.getBufferSize());
            ptr += container.mMeshShader.getBufferSize();
        }
    }

    // ファイル書き出し
    bal::File::WriteToFile(output_path, buffer.get(), memory_size);
}

// ----------------------------------------------------------------------------

}
