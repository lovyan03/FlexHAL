/**
 * @file factory.h
 * @brief GPIO factory for creating different GPIO implementations
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 */

#pragma once

#include "gpio.h"
#include <memory>

namespace flexhal {
namespace gpio {

/**
 * @brief Implementation type enumeration
 */
enum class ImplType : uint8_t {
  Default = 0,  ///< Default implementation (auto-selected based on platform)
  Arduino,      ///< Arduino implementation
  EspIdf,       ///< ESP-IDF implementation
  Native,       ///< Native implementation (direct register access)
  SDL           ///< SDL implementation (for desktop simulation)
};

/**
 * @brief Factory class for creating GPIO implementations
 */
class Factory {
public:
  /**
   * @brief Create a GPIO implementation
   * @param type Implementation type
   * @return std::shared_ptr<GPIO> Shared pointer to the GPIO implementation
   */
  static std::shared_ptr<GPIO> create(ImplType type = ImplType::Default);
  
  /**
   * @brief Create the default GPIO implementation for the current platform
   * @return std::shared_ptr<GPIO> Shared pointer to the default GPIO implementation
   */
  static std::shared_ptr<GPIO> createDefault();
};

} // namespace gpio
} // namespace flexhal
