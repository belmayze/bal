/*!
 * @file   balFrameBuffer.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <container/balArray.h>
#include <math/balMath.h>

namespace bal { class ICommandList; }
namespace bal { class IRenderTargetColor; }
namespace bal { class IRenderTargetDepth; }

// ----------------------------------------------------------------------------
namespace bal {

class FrameBuffer
{
public:
    static constexpr uint32_t cColorRenderTargetMax = 8;

public:
    //! コンストラクター
    FrameBuffer();

    //! デストラクター
    virtual ~FrameBuffer() {}

    /*!
     * レンダーターゲットをセットします
     * @param[in] index           番号
     * @param[in] p_render_target レンダーターゲット
     */
    void setRenderTargetColor(int index, IRenderTargetColor* p_render_target);

    /*!
     * レンダーターゲットをセットします
     * @param[in] p_render_target レンダーターゲット
     */
    void setRenderTargetDepth(IRenderTargetDepth* p_render_target);

    /*!
     * フレームバッファの解像度を設定します
     */
    void setResolution(const MathSize& size);

public:
    //! 解像度
    inline const MathSize& getResolution() const { return mResolution; }
    //! レンダーターゲット
    inline const Array<IRenderTargetColor*, cColorRenderTargetMax>& getRenderTargetColors() const { return mpRenderTargetColors; }
    //! レンダーターゲット
    inline const IRenderTargetDepth* getRenderTargetDepth() const { return mpRenderTargetDepth; }

private:
    Array<IRenderTargetColor*, cColorRenderTargetMax> mpRenderTargetColors;
    IRenderTargetDepth*                               mpRenderTargetDepth = nullptr;
    MathSize                                          mResolution;
};

}
