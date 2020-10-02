/*!
 * @file   balArray.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once

// ----------------------------------------------------------------------------
namespace bal {

template <typename T, size_t N>
class Array : public std::array<T, N>
{
};

// 推論補助
template <typename T, typename... U>
Array(T, U...) -> Array<T, 1 + sizeof...(U)>;

}
