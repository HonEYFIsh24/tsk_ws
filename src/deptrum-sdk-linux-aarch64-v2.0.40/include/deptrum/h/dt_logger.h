// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_logger.h
 * @brief Log management interface for configuring global logging behavior.
 *
 * Provides functions to control log output destinations and severity thresholds.
 */

 #ifndef DEPTRUM_H_DT_LOGGER_H_
#define DEPTRUM_H_DT_LOGGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Sets the global minimum severity threshold for all log outputs.
 *
 * This setting affects console, file, and callback outputs simultaneously.
 * Messages below the specified severity level will be discarded globally.
 *
 * @param[in] severity Minimum severity level to enforce globally.
 * @param[out] error Pointer to receive error details. Initialized to NULL before call,
 *                   populated with error object on failure. Caller must release memory.
 */
DT_EXPORT void dt_set_logger_severity(dt_log_severity severity, dt_error** error);

/**
 * @brief Configures logging output to the system console/terminal.
 *
 * Enables real-time log display in the terminal with severity-based filtering.
 * Has no effect if the severity level is below the global minimum threshold.
 *
 * @param[in] severity Minimum severity level for console output.
 * @param[out] error Pointer to receive error details. Initialized to NULL before call,
 *                   populated with error object on failure. Caller must release memory.
 */
DT_EXPORT void dt_set_logger_to_console(dt_log_severity severity, dt_error** error);

/**
 * @brief Configures logging output to disk files in specified directory.
 *
 * Creates daily rotating log files with automatic timestamp naming. Retains
 * existing directory path if empty string is provided.
 *
 * @param[in] severity Minimum severity level for file output.
 * @param[in] directory Target directory for log files. Empty string preserves current setting.
 * @param[out] error Pointer to receive error details. Initialized to NULL before call,
 *                   populated with error object on failure. Caller must release memory.
 */
DT_EXPORT void dt_set_logger_to_file(dt_log_severity severity,
                                     const char* directory,
                                     dt_error** error);

/**
 * @brief Registers a custom callback handler for log processing.
 *
 * The callback receives formatted log messages matching the specified severity.
 * User-defined context data is passed through to the callback invocation.
 *
 * @param[in] severity Minimum severity level to trigger callback.
 * @param[in] callback Function pointer to the log handler implementation.
 * @param[in] user_data Context pointer passed through to callback (may be NULL).
 * @param[out] error Pointer to receive error details. Initialized to NULL before call,
 *                   populated with error object on failure. Caller must release memory.
 */
DT_EXPORT void dt_set_logger_to_callback(dt_log_severity severity,
                                         dt_log_callback callback,
                                         void* user_data,
                                         dt_error** error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_LOGGER_H_