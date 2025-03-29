/**
 * @file FlexHAL.hpp
 * @brief FlexHAL (Flexible Hardware Abstraction Layer) main C++ header
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 * 
 * @details
 * This is the main C++ header file for the FlexHAL library.
 * It provides a flexible hardware abstraction layer for multiple platforms.
 */

#pragma once

// Include standard C++ headers
#include <cstdint>
#include <memory>
#include <functional>

// Include platform detection
#include "flexhal/platform_detect.h"

// Forward declarations
namespace flexhal {
  namespace gpio {
    class GPIO;
    class Factory;
  }
  
  namespace time {
    class Time;
  }
  
  namespace logger {
    class Logger;
  }
}

/**
 * @brief Main FlexHAL class that provides access to all HAL features
 */
class FlexHAL {
public:
  /**
   * @brief Get the singleton instance of FlexHAL
   * @return FlexHAL& Reference to the singleton instance
   */
  static FlexHAL& getInstance();
  
  /**
   * @brief Initialize the HAL
   * @return true if initialization was successful
   */
  bool begin();
  
  /**
   * @brief Shutdown the HAL
   */
  void end();
  
  /**
   * @brief Get the GPIO interface
   * @return std::shared_ptr<flexhal::gpio::GPIO> Shared pointer to the GPIO interface
   */
  std::shared_ptr<flexhal::gpio::GPIO> gpio();
  
  /**
   * @brief Get the Time interface
   * @return std::shared_ptr<flexhal::time::Time> Shared pointer to the Time interface
   */
  std::shared_ptr<flexhal::time::Time> time();
  
  /**
   * @brief Get the Logger interface
   * @return std::shared_ptr<flexhal::logger::Logger> Shared pointer to the Logger interface
   */
  std::shared_ptr<flexhal::logger::Logger> logger();
  
private:
  // Private constructor to enforce singleton pattern
  FlexHAL();
  
  // Prevent copy and assignment
  FlexHAL(const FlexHAL&) = delete;
  FlexHAL& operator=(const FlexHAL&) = delete;
  
  // Implementation details
  class Impl;
  std::unique_ptr<Impl> _impl;
};

/**
 * @brief Global instance of FlexHAL for easy access
 */
extern FlexHAL hal;

// Include public API headers
#include "flexhal/gpio/_include.h"
#include "flexhal/time/_include.h"
#include "flexhal/logger/_include.h"
