/*!
 * @file   balDynamicArray.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// C++
#include <cassert>
#include <stdexcept>

// ----------------------------------------------------------------------------
namespace bal {

template <typename T>
class DynamicArray
{
    using value_type      = T;
    using pointer         = T*;
    using const_pointer   = const T*;
    using reference       = T&;
    using const_reference = const T&;

public:
    //! バッファを作成します
    void create(size_t n) {
        mpBuffer   = make_unique<T[]>(nullptr, n);
        mBufferNum = n;
    }

    //! バッファを破棄します
    void destroy() {
        mpBuffer.reset();
        mBufferNum = 0;
    }

    //! 特定の値で埋めます
    void fill(const_reference v) {
        std::fill_n(mpBuffer.get(), mBufferNum, v);
    }

    //! サイズを返します
    constexpr size_t size() const {
        return mBufferNum;
    }

    //! 値を取得します
    constexpr reference at(size_t n) {
        if (mBufferNum <= n) { std::out_of_range("invalid DynamicArray<T>::at(N) subscript"); }
        return mpBuffer[n];
    }
    constexpr const_reference at(size_t n) const {
        if (mBufferNum <= n) { std::out_of_range("invalid DynamicArray<T>::at(N) subscript"); }
        return mpBuffer[n];
    }

    //! 先頭
    constexpr reference front() {
        return mpBuffer[0];
    }
    constexpr const_reference front() const {
        return mpBuffer[0];
    }

    //! 末尾
    constexpr reference back() {
        return mpBuffer[mBufferNum - 1];
    }
    constexpr const_reference back() const {
        return mpBuffer[mBufferNum - 1];
    }

    //! データ
    constexpr pointer data() {
        return mpBuffer.get();
    }
    constexpr const_pointer data() const {
        return mpBuffer.get();
    }

    // @TODO: イテレーターの実装

    //! operator
    constexpr reference operator[](size_t n) {
#       if BAL_BUILD_UNDER_DEVELOPMENT
        assert(n < mBufferNum);
#       endif
        return mpBuffer[n];
    }
    constexpr const_reference operator[](size_t n) const {
#       if BAL_BUILD_UNDER_DEVELOPMENT
        assert(n < mBufferNum);
#       endif
        return mpBuffer[n];
    }

private:
    std::unique_ptr<T[]> mpBuffer;
    size_t               mBufferNum = 0;
};

}
