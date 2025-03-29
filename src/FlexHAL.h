/**
 * @file FlexHAL.h
 * @brief FlexHAL (Flexible Hardware Abstraction Layer) main header
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 * 
 * @details
 * This is the main header file for the FlexHAL library.
 * It provides a flexible hardware abstraction layer for multiple platforms.
 */

#pragma once

// Check if C++ compiler is used
#ifdef __cplusplus

// Check C++ version (require C++14 or later)
#if __cplusplus >= 201402L

// Include the C++ implementation
#include "FlexHAL.hpp"

#else // __cplusplus < 201402L
#error "FlexHAL requires C++14 or later. Please enable C++14 in your compiler settings."
#endif // __cplusplus check

#else // not __cplusplus
#error "FlexHAL requires a C++ compiler. C API is not implemented yet."
#endif // __cplusplus check
