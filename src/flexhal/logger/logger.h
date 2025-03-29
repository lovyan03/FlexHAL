/**
 * @file logger.h
 * @brief Logger functionality
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 */

#pragma once

#include <cstdint>
#include <string>
#include <memory>

namespace flexhal {
namespace logger {

/**
 * @brief Log level enumeration
 */
enum class LogLevel : uint8_t {
  None = 0,   ///< No logging
  Error,      ///< Error messages
  Warning,    ///< Warning messages
  Info,       ///< Informational messages
  Debug,      ///< Debug messages
  Verbose     ///< Verbose messages
};

/**
 * @brief Logger interface class
 * 
 * This class defines the interface for logging operations.
 * Different implementations can be provided for different platforms.
 */
class Logger {
public:
  /**
   * @brief Virtual destructor
   */
  virtual ~Logger() = default;
  
  /**
   * @brief Initialize the logger
   * @return true if initialization was successful
   */
  virtual bool begin() = 0;
  
  /**
   * @brief Shutdown the logger
   */
  virtual void end() = 0;
  
  /**
   * @brief Set the log level
   * @param level Log level
   */
  virtual void setLogLevel(LogLevel level) = 0;
  
  /**
   * @brief Get the current log level
   * @return Current log level
   */
  virtual LogLevel getLogLevel() const = 0;
  
  /**
   * @brief Log an error message
   * @param message Message to log
   */
  virtual void error(const std::string& message) = 0;
  
  /**
   * @brief Log a warning message
   * @param message Message to log
   */
  virtual void warning(const std::string& message) = 0;
  
  /**
   * @brief Log an informational message
   * @param message Message to log
   */
  virtual void info(const std::string& message) = 0;
  
  /**
   * @brief Log a debug message
   * @param message Message to log
   */
  virtual void debug(const std::string& message) = 0;
  
  /**
   * @brief Log a verbose message
   * @param message Message to log
   */
  virtual void verbose(const std::string& message) = 0;
};

/**
 * @brief Factory class for creating Logger implementations
 */
class LoggerFactory {
public:
  /**
   * @brief Create a logger implementation
   * @return std::shared_ptr<Logger> Shared pointer to the logger implementation
   */
  static std::shared_ptr<Logger> create();
  
  /**
   * @brief Get the default logger instance
   * @return std::shared_ptr<Logger> Shared pointer to the default logger instance
   */
  static std::shared_ptr<Logger> getDefault();
};

// Global logging functions for convenience
void setLogLevel(LogLevel level);
void error(const std::string& message);
void warning(const std::string& message);
void info(const std::string& message);
void debug(const std::string& message);
void verbose(const std::string& message);

} // namespace logger
} // namespace flexhal
