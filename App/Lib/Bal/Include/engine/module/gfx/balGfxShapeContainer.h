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
    //! コンストラクター
    ShapeContainer();

    //! デストラクター
    virtual ~ShapeContainer();

    /*!
     * シェイプを作成します
     */
    void initialize();

private:
    std::unique_ptr<IShapeBuffer> mpQuadBuffer;
};

}
