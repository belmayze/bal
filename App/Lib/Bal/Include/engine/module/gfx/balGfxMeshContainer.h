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

/*!
 * よく使用するメッシュ情報を入れるコンテナ
 * すべてのメッシュは -1 ～ 1 までの大きさ 2 です
 */
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

    enum class Type
    {
        Quad,
        Cube,
        Sphere,
    };
    constexpr static size_t cTypeNum = static_cast<size_t>(Type::Sphere) + 1;

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
    const IMeshBuffer* getBuffer(Type type) const { return mpBuffers[static_cast<int>(type)].get(); }

private:
    std::unique_ptr<std::unique_ptr<IMeshBuffer>[]> mpBuffers;
};

}
