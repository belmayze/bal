/*!
 * @file   balGfxMeshContainer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>
#include <memory/balSingleton.h>

namespace bal { class IMeshBuffer; }

// ----------------------------------------------------------------------------
namespace bal::mod::gfx {

class MeshContainer : public Singleton<MeshContainer>
{
public:
    struct Vertex
    {
        MathVector3 mPosition;
        MathVector3 mNormal;
        MathVector2 mTexcoord;
    };

    constexpr static size_t cOffsetPosition = offsetof(Vertex, mPosition);
    constexpr static size_t cOffsetNormal   = offsetof(Vertex, mNormal);
    constexpr static size_t cOffsetTexcoord = offsetof(Vertex, mTexcoord);

public:
    //! コンストラクター
    MeshContainer();

    //! デストラクター
    virtual ~MeshContainer();

    /*!
     * シェイプを作成します
     */
    void initialize();

    //! 矩形
    const IMeshBuffer* getQuadBuffer() const { return mpQuadBuffer.get(); }

private:
    std::unique_ptr<IMeshBuffer> mpQuadBuffer;
};

}
