/**
 * @file FlexHAL.cpp
 * @brief FlexHAL (Flexible Hardware Abstraction Layer) main implementation
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 */

#include "FlexHAL.hpp"
#include "flexhal/platform_detect.h"
#include "flexhal/gpio/factory.h"
#include "flexhal/time/delay.h"
#include "flexhal/logger/logger.h"

// Implementation class for FlexHAL
class FlexHAL::Impl {
public:
  Impl() 
    : _initialized(false)
    , _gpio(nullptr)
    , _time(nullptr)
    , _logger(nullptr)
  {}
  
  ~Impl() {
    end();
  }
  
  bool begin() {
    if (_initialized) return true;
    
    // Initialize in the correct order: platform -> framework -> rtos
    if (!initPlatform()) {
      return false;
    }
    
    if (!initFramework()) {
      endPlatform();
      return false;
    }
    
    if (!initRTOS()) {
      endFramework();
      endPlatform();
      return false;
    }
    
    // Now initialize the components
    _logger = flexhal::logger::LoggerFactory::create();
    if (!_logger || !_logger->begin()) {
      endRTOS();
      endFramework();
      endPlatform();
      return false;
    }
    
    _time = createTimeImpl();
    if (!_time || !_time->begin()) {
      _logger->end();
      endRTOS();
      endFramework();
      endPlatform();
      return false;
    }
    
    _gpio = flexhal::gpio::Factory::createDefault();
    if (!_gpio || !_gpio->begin()) {
      _time->end();
      _logger->end();
      endRTOS();
      endFramework();
      endPlatform();
      return false;
    }
    
    _initialized = true;
    _logger->info("FlexHAL initialized successfully");
    return true;
  }
  
  void end() {
    if (!_initialized) return;
    
    // End components in reverse order
    if (_gpio) {
      _gpio->end();
      _gpio.reset();
    }
    
    if (_time) {
      _time->end();
      _time.reset();
    }
    
    if (_logger) {
      _logger->info("FlexHAL shutting down");
      _logger->end();
      _logger.reset();
    }
    
    // End in reverse order: rtos -> framework -> platform
    endRTOS();
    endFramework();
    endPlatform();
    
    _initialized = false;
  }
  
  std::shared_ptr<flexhal::gpio::GPIO> gpio() {
    if (!_initialized) {
      if (!begin()) {
        return nullptr;
      }
    }
    return _gpio;
  }
  
  std::shared_ptr<flexhal::time::Time> time() {
    if (!_initialized) {
      if (!begin()) {
        return nullptr;
      }
    }
    return _time;
  }
  
  std::shared_ptr<flexhal::logger::Logger> logger() {
    if (!_initialized) {
      if (!begin()) {
        return nullptr;
      }
    }
    return _logger;
  }
  
private:
  bool _initialized;
  std::shared_ptr<flexhal::gpio::GPIO> _gpio;
  std::shared_ptr<flexhal::time::Time> _time;
  std::shared_ptr<flexhal::logger::Logger> _logger;
  
  // Platform initialization
  bool initPlatform() {
    // Platform-specific initialization based on detected platform
    switch (flexhal::getPlatform()) {
      case flexhal::Platform::ESP32:
        return initPlatformESP32();
      case flexhal::Platform::Desktop:
        return initPlatformDesktop();
      default:
        // Default initialization for unknown platforms
        return true;
    }
  }
  
  bool initPlatformESP32() {
    // ESP32-specific initialization
    return true;
  }
  
  bool initPlatformDesktop() {
    // Desktop-specific initialization
    return true;
  }
  
  void endPlatform() {
    // Platform-specific cleanup based on detected platform
    switch (flexhal::getPlatform()) {
      case flexhal::Platform::ESP32:
        endPlatformESP32();
        break;
      case flexhal::Platform::Desktop:
        endPlatformDesktop();
        break;
      default:
        // Default cleanup for unknown platforms
        break;
    }
  }
  
  void endPlatformESP32() {
    // ESP32-specific cleanup
  }
  
  void endPlatformDesktop() {
    // Desktop-specific cleanup
  }
  
  // Framework initialization
  bool initFramework() {
    bool success = true;
    
    // Initialize each available framework
    // Arduino framework
    if (flexhal::hasFramework(flexhal::Framework::Arduino)) {
      success &= initFrameworkArduino();
    }
    
    // ESP-IDF framework
    if (flexhal::hasFramework(flexhal::Framework::EspIdf)) {
      success &= initFrameworkEspIdf();
    }
    
    // SDL framework
    if (flexhal::hasFramework(flexhal::Framework::SDL)) {
      success &= initFrameworkSDL();
    }
    
    return success;
  }
  
  bool initFrameworkArduino() {
    // Arduino-specific initialization
    return true;
  }
  
  bool initFrameworkEspIdf() {
    // ESP-IDF-specific initialization
    return true;
  }
  
  bool initFrameworkSDL() {
    // SDL-specific initialization
    return true;
  }
  
  void endFramework() {
    // Cleanup each initialized framework in reverse order
    
    // SDL framework
    if (flexhal::hasFramework(flexhal::Framework::SDL)) {
      endFrameworkSDL();
    }
    
    // ESP-IDF framework
    if (flexhal::hasFramework(flexhal::Framework::EspIdf)) {
      endFrameworkEspIdf();
    }
    
    // Arduino framework
    if (flexhal::hasFramework(flexhal::Framework::Arduino)) {
      endFrameworkArduino();
    }
  }
  
  void endFrameworkArduino() {
    // Arduino-specific cleanup
  }
  
  void endFrameworkEspIdf() {
    // ESP-IDF-specific cleanup
  }
  
  void endFrameworkSDL() {
    // SDL-specific cleanup
  }
  
  // RTOS initialization
  bool initRTOS() {
    bool success = true;
    
    // Initialize each available RTOS
    // FreeRTOS
    if (flexhal::hasRTOS(flexhal::RTOS::FreeRTOS)) {
      success &= initRTOSFreeRTOS();
    }
    
    // Desktop OS
    if (flexhal::hasRTOS(flexhal::RTOS::Desktop)) {
      success &= initRTOSDesktop();
    }
    
    return success;
  }
  
  bool initRTOSFreeRTOS() {
    // FreeRTOS-specific initialization
    return true;
  }
  
  bool initRTOSDesktop() {
    // Desktop OS-specific initialization
    return true;
  }
  
  void endRTOS() {
    // Cleanup each initialized RTOS in reverse order
    
    // Desktop OS
    if (flexhal::hasRTOS(flexhal::RTOS::Desktop)) {
      endRTOSDesktop();
    }
    
    // FreeRTOS
    if (flexhal::hasRTOS(flexhal::RTOS::FreeRTOS)) {
      endRTOSFreeRTOS();
    }
  }
  
  void endRTOSFreeRTOS() {
    // FreeRTOS-specific cleanup
  }
  
  void endRTOSDesktop() {
    // Desktop OS-specific cleanup
  }
  
  // Create platform-specific time implementation
  std::shared_ptr<flexhal::time::Time> createTimeImpl() {
    // This will be implemented in the future with actual implementations
    // For now, return nullptr
    return nullptr;
  }
};

// Singleton instance
FlexHAL& FlexHAL::getInstance() {
  static FlexHAL instance;
  return instance;
}

// Constructor
FlexHAL::FlexHAL() : _impl(new Impl()) {
}

// Public methods
bool FlexHAL::begin() {
  return _impl->begin();
}

void FlexHAL::end() {
  _impl->end();
}

std::shared_ptr<flexhal::gpio::GPIO> FlexHAL::gpio() {
  return _impl->gpio();
}

std::shared_ptr<flexhal::time::Time> FlexHAL::time() {
  return _impl->time();
}

std::shared_ptr<flexhal::logger::Logger> FlexHAL::logger() {
  return _impl->logger();
}

// Global instance
FlexHAL hal;

// Global time functions
namespace flexhal {
namespace time {

uint32_t millis() {
  auto time = hal.time();
  return time ? time->millis() : 0;
}

uint32_t micros() {
  auto time = hal.time();
  return time ? time->micros() : 0;
}

void delay(uint32_t ms) {
  auto time = hal.time();
  if (time) {
    time->delay(ms);
  }
}

void delayMicroseconds(uint32_t us) {
  auto time = hal.time();
  if (time) {
    time->delayMicroseconds(us);
  }
}

} // namespace time
} // namespace flexhal

// Global logger functions
namespace flexhal {
namespace logger {

void setLogLevel(LogLevel level) {
  auto logger = hal.logger();
  if (logger) {
    logger->setLogLevel(level);
  }
}

void error(const std::string& message) {
  auto logger = hal.logger();
  if (logger) {
    logger->error(message);
  }
}

void warning(const std::string& message) {
  auto logger = hal.logger();
  if (logger) {
    logger->warning(message);
  }
}

void info(const std::string& message) {
  auto logger = hal.logger();
  if (logger) {
    logger->info(message);
  }
}

void debug(const std::string& message) {
  auto logger = hal.logger();
  if (logger) {
    logger->debug(message);
  }
}

void verbose(const std::string& message) {
  auto logger = hal.logger();
  if (logger) {
    logger->verbose(message);
  }
}

} // namespace logger
} // namespace flexhal
