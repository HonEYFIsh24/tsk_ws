// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_logger.hpp
 * @brief Logging interface for system-wide logging operations.
 */

#ifndef DEPTRUM_HPP_DT_LOGGER_HPP_
#define DEPTRUM_HPP_DT_LOGGER_HPP_

#include "deptrum/h/dt_logger.h"
#include "dt_error.hpp"
#include "dt_types.hpp"

namespace dt {
class Logger {
 public:
  /**
   * @brief Callback type for custom log message handling.
   *
   * @param[in] severity Log severity level of the message.
   * @param[in] log_msg Log message content.
   */
  typedef std::function<void(DtLogSeverity severity, const char* log_msg)> LogCallback;

  // Internal callback adapter (C-compatible).
  static void log_callback(DtLogSeverity severity, const char* log_msg, void* user_data) {
    auto cb = static_cast<LogCallback*>(user_data);
    if (cb) {
      (*cb)(severity, log_msg);
    }
  }

  // Singleton accessor for callback storage.
  static LogCallback& get_log_callback() {
    static LogCallback log_callback_ = nullptr;
    return log_callback_;
  }

  /**
   * @brief Sets global logging verbosity threshold.
   *
   * Controls log levels for all output targets (console, file, callback).
   * Messages below this severity will be discarded.
   *
   * @param[in] severity Minimum severity level to output.
   */
  static void SetLoggerSeverity(DtLogSeverity severity) {
    dt_error* error = nullptr;
    dt_set_logger_severity(severity, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Configures file-based logging output.
   *
   * @param[in] severity Minimum severity level for file output.
   * @param[in] directory Output directory path for log files.
   *
   * @note Empty directory preserves existing configuration. If no prior
   *       configuration exists, file logging will be disabled.
   */
  static void SetLoggerToFile(DtLogSeverity severity, const char* directory) {
    dt_error* error = nullptr;
    dt_set_logger_to_file(severity, directory, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Configures console-based logging output.
   *
   * @param[in] severity Minimum severity level for console output.
   */
  static void SetLoggerToConsole(DtLogSeverity severity) {
    dt_error* error = nullptr;
    dt_set_logger_to_console(severity, &error);
    Error::Handle(&error);
  }

  /**
   * @brief Configures custom callback for log handling.
   *
   * @param[in] severity Minimum severity level for callback invocation.
   * @param[in] callback User-defined function to process log messages.
   */
  static void SetLoggerToCallback(DtLogSeverity severity, LogCallback callback) {
    dt_error* error = nullptr;
    get_log_callback() = callback;
    dt_set_logger_to_callback(severity, &log_callback, &get_log_callback(), &error);
    Error::Handle(&error);
  }
};
}  // namespace dt

#endif  // DEPTRUM_HPP_DT_LOGGER_HPP_
