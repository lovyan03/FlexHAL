/**
 * @file delay.h
 * @brief Time and delay functionality
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 */

#pragma once

#include <cstdint>
#include <memory>

namespace flexhal {
namespace time {

/**
 * @brief Time interface class
 * 
 * This class defines the interface for time operations.
 * Different implementations can be provided for different platforms.
 */
class Time {
public:
  /**
   * @brief Virtual destructor
   */
  virtual ~Time() = default;
  
  /**
   * @brief Initialize the time interface
   * @return true if initialization was successful
   */
  virtual bool begin() = 0;
  
  /**
   * @brief Shutdown the time interface
   */
  virtual void end() = 0;
  
  /**
   * @brief Get the number of milliseconds since the program started
   * @return Milliseconds since start
   */
  virtual uint32_t millis() = 0;
  
  /**
   * @brief Get the number of microseconds since the program started
   * @return Microseconds since start
   */
  virtual uint32_t micros() = 0;
  
  /**
   * @brief Delay for a specified number of milliseconds
   * @param ms Milliseconds to delay
   */
  virtual void delay(uint32_t ms) = 0;
  
  /**
   * @brief Delay for a specified number of microseconds
   * @param us Microseconds to delay
   */
  virtual void delayMicroseconds(uint32_t us) = 0;
};

/**
 * @brief Get the number of milliseconds since the program started
 * @return Milliseconds since start
 */
uint32_t millis();

/**
 * @brief Get the number of microseconds since the program started
 * @return Microseconds since start
 */
uint32_t micros();

/**
 * @brief Delay for a specified number of milliseconds
 * @param ms Milliseconds to delay
 */
void delay(uint32_t ms);

/**
 * @brief Delay for a specified number of microseconds
 * @param us Microseconds to delay
 */
void delayMicroseconds(uint32_t us);

} // namespace time
} // namespace flexhal
