/*!
 * @file   balViewport.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// bal
#include <math/balMath.h>

namespace bal { class FrameBuffer; }

// ----------------------------------------------------------------------------
namespace bal {

class Viewport
{
public:
    /*!
     * コンストラクター
     */
    Viewport() {}

    /*!
     * フレームバッファを覆うビューポートを初期化します
     * @param[in] frame_buffer フレームバッファ
     */
    explicit Viewport(const FrameBuffer& frame_buffer);

    /*! 
     * コンストラクター
     */
    Viewport(const MathVector2& origin, const MathVector2& size, float min_depth, float max_depth)
        : mOrigin(origin), mSize(size), mMinDepth(min_depth), mMaxDepth(max_depth) {}

    /*!
     * 基準点を設定します
     */
    inline void setOrigin(const MathVector2& origin) { mOrigin = origin; }

    /*!
     * ビューポートのサイズを設定します
     */
    inline void setSize(const MathVector2& size) { mSize = size; }

    /*!
     * デプスのサイズを設定します
     */
    inline void setDepth(float min_depth, float max_depth) { mMinDepth = min_depth; mMaxDepth = max_depth; }

    //! 基準点
    inline const MathVector2& getOrigin() const { return mOrigin; }
    //! サイズ
    inline const MathVector2& getSize() const { return mSize; }
    //! 最小デプス
    inline float getMinDepth() const { return mMinDepth; }
    //! 最大デプス
    inline float getMaxDepth() const { return mMaxDepth; }

private:
    MathVector2 mOrigin;
    MathVector2 mSize;
    float       mMinDepth = 0.f;
    float       mMaxDepth = 1.f;
};

}
