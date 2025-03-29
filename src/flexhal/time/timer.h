/**
 * @file timer.h
 * @brief Timer functionality
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>

namespace flexhal {
namespace time {

/**
 * @brief Timer callback function type
 */
using TimerCallback = std::function<void(void)>;

/**
 * @brief Timer interface class
 * 
 * This class defines the interface for timer operations.
 * Different implementations can be provided for different platforms.
 */
class Timer {
public:
  /**
   * @brief Virtual destructor
   */
  virtual ~Timer() = default;
  
  /**
   * @brief Initialize the timer
   * @return true if initialization was successful
   */
  virtual bool begin() = 0;
  
  /**
   * @brief Shutdown the timer
   */
  virtual void end() = 0;
  
  /**
   * @brief Start a one-shot timer
   * @param callback Function to call when timer expires
   * @param timeout_ms Timeout in milliseconds
   * @return true if timer was started successfully
   */
  virtual bool setTimeout(TimerCallback callback, uint32_t timeout_ms) = 0;
  
  /**
   * @brief Start a periodic timer
   * @param callback Function to call when timer expires
   * @param interval_ms Interval in milliseconds
   * @return true if timer was started successfully
   */
  virtual bool setInterval(TimerCallback callback, uint32_t interval_ms) = 0;
  
  /**
   * @brief Stop the timer
   */
  virtual void stop() = 0;
  
  /**
   * @brief Check if the timer is running
   * @return true if timer is running
   */
  virtual bool isRunning() const = 0;
};

/**
 * @brief Factory class for creating Timer implementations
 */
class TimerFactory {
public:
  /**
   * @brief Create a timer implementation
   * @return std::shared_ptr<Timer> Shared pointer to the timer implementation
   */
  static std::shared_ptr<Timer> create();
};

} // namespace time
} // namespace flexhal
