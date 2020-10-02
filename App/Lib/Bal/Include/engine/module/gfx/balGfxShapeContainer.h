/*!
 * @file   balGfxShapeContainer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <memory/balSingleton.h>

namespace bal { class IShapeBuffer; }

// ----------------------------------------------------------------------------
namespace bal::mod::gfx {

class ShapeContainer : public Singleton<ShapeContainer>
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
    ShapeContainer();

    //! デストラクター
    virtual ~ShapeContainer();

    /*!
     * シェイプを作成します
     */
    void initialize();

    //! 矩形
    const IShapeBuffer* getQuadBuffer() const { return mpQuadBuffer.get(); }

private:
    std::unique_ptr<IShapeBuffer> mpQuadBuffer;
};

}
