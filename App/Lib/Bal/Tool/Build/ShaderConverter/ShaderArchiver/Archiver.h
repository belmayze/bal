/*!
 * @file   Archiver.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balStringPtr.h>
#include <io/balFile.h>
#include <graphics/archiver/balShaderArchiver.h>

namespace app
{

class Archiver
{
public:
    //! 初期化
    struct InitializeArg
    {
        uint32_t mNumShader = 0;
    };

    //! 構造体
    struct ShaderArg
    {
        uint32_t       mShaderIndex = 0;
        bal::StringPtr mShaderName;
        bal::StringPtr mVertexShaderFilepath;
        bal::StringPtr mGeometryShaderFilepath;
        bal::StringPtr mPixelShaderFilepath;
        bal::StringPtr mComputeShaderFilepath;
        bal::StringPtr mDomainShaderFilepath;
        bal::StringPtr mHullShaderFilepath;
        bal::StringPtr mAmplificationShaderFilepath;
        bal::StringPtr mMeshShaderFilepath;
    };

public:
    /*!
     * 初期化
     * @param[in] arg 引数
     */
    void initialize(const InitializeArg& arg);

    /*!
     * シェーダーをセットする
     * @param[in] arg 引数
     */
    void setShader(const ShaderArg& arg);

    /*!
     * アーカイブする
     */
    void archive(const bal::StringPtr& output_path);

private:
    struct ShaderContainer
    {
        bal::StringBuffer mProgramName;
        bal::File         mVertexShader;
        bal::File         mGeometryShader;
        bal::File         mPixelShader;
        bal::File         mComputeShader;
        bal::File         mDomainShader;
        bal::File         mHullShader;
        bal::File         mAmplificationShader;
        bal::File         mMeshShader;
    };

private:
    bal::gfx::ShaderArchiver::Header                 mHeader;
    std::unique_ptr<bal::gfx::ShaderArchiver::Tag[]> mTags;
    std::unique_ptr<ShaderContainer[]>               mShaders;
};

}
