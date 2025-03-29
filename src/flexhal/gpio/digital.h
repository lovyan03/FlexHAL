/**
 * @file digital.h
 * @brief Digital GPIO functionality
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 */

#pragma once

#include "gpio.h"

namespace flexhal {
namespace gpio {

/**
 * @brief Digital GPIO class
 * 
 * This class provides Arduino-compatible digital GPIO functions.
 */
class Digital {
public:
  /**
   * @brief Constructor
   * @param gpio GPIO interface
   */
  explicit Digital(std::shared_ptr<GPIO> gpio) : _gpio(gpio) {}
  
  /**
   * @brief Set the mode of a pin
   * @param pin Pin number
   * @param mode Pin mode
   */
  void pinMode(uint8_t pin, PinMode mode) {
    _gpio->pinMode(pin, mode);
  }
  
  /**
   * @brief Write a digital value to a pin
   * @param pin Pin number
   * @param level Pin level (High or Low)
   */
  void digitalWrite(uint8_t pin, PinLevel level) {
    _gpio->digitalWrite(pin, level);
  }
  
  /**
   * @brief Write a digital value to a pin
   * @param pin Pin number
   * @param value Pin value (0 for Low, non-zero for High)
   */
  void digitalWrite(uint8_t pin, int value) {
    _gpio->digitalWrite(pin, value);
  }
  
  /**
   * @brief Read a digital value from a pin
   * @param pin Pin number
   * @return Pin level (High or Low)
   */
  PinLevel digitalRead(uint8_t pin) {
    return _gpio->digitalRead(pin);
  }
  
  /**
   * @brief Read a digital value from a pin
   * @param pin Pin number
   * @return Pin value (0 for Low, 1 for High)
   */
  int digitalReadValue(uint8_t pin) {
    return _gpio->digitalReadValue(pin);
  }
  
private:
  std::shared_ptr<GPIO> _gpio;
};

} // namespace gpio
} // namespace flexhal
