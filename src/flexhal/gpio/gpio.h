/**
 * @file gpio.h
 * @brief GPIO interface definition
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
namespace gpio {

/**
 * @brief Pin mode definitions
 */
enum class PinMode : uint8_t {
  Input = 0,       ///< Input mode
  Output,          ///< Output mode
  InputPullUp,     ///< Input with pull-up resistor
  InputPullDown,   ///< Input with pull-down resistor
  OutputOpenDrain, ///< Open-drain output
  Analog           ///< Analog input/output
};

/**
 * @brief Pin level definitions
 */
enum class PinLevel : uint8_t {
  Low = 0,         ///< Low level (0)
  High = 1         ///< High level (1)
};

/**
 * @brief GPIO interface class
 * 
 * This class defines the interface for GPIO operations.
 * Different implementations can be provided for different platforms.
 */
class GPIO {
public:
  /**
   * @brief Virtual destructor
   */
  virtual ~GPIO() = default;
  
  /**
   * @brief Initialize the GPIO interface
   * @return true if initialization was successful
   */
  virtual bool begin() = 0;
  
  /**
   * @brief Shutdown the GPIO interface
   */
  virtual void end() = 0;
  
  /**
   * @brief Set the mode of a pin
   * @param pin Pin number
   * @param mode Pin mode
   */
  virtual void pinMode(uint8_t pin, PinMode mode) = 0;
  
  /**
   * @brief Write a digital value to a pin
   * @param pin Pin number
   * @param level Pin level (High or Low)
   */
  virtual void digitalWrite(uint8_t pin, PinLevel level) = 0;
  
  /**
   * @brief Write a digital value to a pin
   * @param pin Pin number
   * @param value Pin value (0 for Low, non-zero for High)
   */
  virtual void digitalWrite(uint8_t pin, int value) = 0;
  
  /**
   * @brief Read a digital value from a pin
   * @param pin Pin number
   * @return Pin level (High or Low)
   */
  virtual PinLevel digitalRead(uint8_t pin) = 0;
  
  /**
   * @brief Read a digital value from a pin
   * @param pin Pin number
   * @return Pin value (0 for Low, 1 for High)
   */
  virtual int digitalReadValue(uint8_t pin) = 0;
};

// Arduino-compatible pin mode definitions for easier migration
constexpr PinMode INPUT = PinMode::Input;
constexpr PinMode OUTPUT = PinMode::Output;
constexpr PinMode INPUT_PULLUP = PinMode::InputPullUp;
constexpr PinMode INPUT_PULLDOWN = PinMode::InputPullDown;
constexpr PinMode OUTPUT_OPEN_DRAIN = PinMode::OutputOpenDrain;

// Arduino-compatible pin level definitions for easier migration
constexpr int LOW = 0;
constexpr int HIGH = 1;

} // namespace gpio
} // namespace flexhal
