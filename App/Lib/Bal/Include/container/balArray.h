/*!
 * @file   balArray.h
 * @brief  
 * @author belmayze
 *
 * Copyright (c) 2020 belmayze. All rights reserved.
 */
#pragma once
// C++
#include <array>

// ----------------------------------------------------------------------------
namespace bal {

template <typename T, std::size_t N>
using Array = std::array<T, N>;

}
