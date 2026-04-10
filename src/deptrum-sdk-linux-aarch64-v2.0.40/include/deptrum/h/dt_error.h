// Copyright (c) Deptrum Inc. All Rights Reserved.

/**
 * @file dt_error.h
 * @brief Defines error handling functions and structures for the SDK.
 *        Provides interfaces to create, query, and manage error objects.
 */

#ifndef DEPTRUM_H_DT_ERROR_H_
#define DEPTRUM_H_DT_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dt_types.h"

/**
 * @brief Creates a new error object with detailed diagnostic information.
 *
 * @param[in] status Error status code indicating failure type.
 * @param[in] message Human-readable error description.
 * @param[in] function Name of the API function where error occurred.
 * @param[in] args Arguments passed to the function when error occurred.
 * @param[in] exception_type Specific exception category triggering the error.
 * @return Pointer to newly created error object. Caller assumes ownership.
 */
DT_EXPORT dt_error* dt_create_error(dt_status status,
                                    const char* message,
                                    const char* function,
                                    const char* args,
                                    dt_exception_type exception_type);

/**
 * @brief Releases resources associated with an error object.
 *
 * @param[in] error Target error object to destroy. Caller must set pointer to NULL after deletion.
 */
DT_EXPORT void dt_delete_error(dt_error* error);

/**
 * @brief Retrieves the status code from an error object.
 *
 * @param[in] error Target error object to query.
 * @return Error status code indicating failure type.
 */
DT_EXPORT dt_status dt_error_get_status(const dt_error* error);

/**
 * @brief Retrieves the descriptive message from an error object.
 *
 * @param[in] error Target error object to query.
 * @return Human-readable error description (null-terminated string).
 */
DT_EXPORT const char* dt_error_get_message(const dt_error* error);

/**
 * @brief Identifies the function where error originated.
 *
 * @param[in] error Target error object to query.
 * @return Name of the API function where failure occurred.
 */
DT_EXPORT const char* dt_error_get_function(const dt_error* error);

/**
 * @brief Retrieves arguments present during function failure.
 *
 * @param[in] error Target error object to query.
 * @return Arguments passed to the function when error occurred.
 */
DT_EXPORT const char* dt_error_get_args(const dt_error* error);

/**
 * @brief Identifies the exception category causing the error.
 *
 * @param[in] error Target error object to query.
 * @return Specific exception type triggering the failure.
 */
DT_EXPORT dt_exception_type dt_error_get_exception_type(const dt_error* error);

#ifdef __cplusplus
}
#endif

#endif  // DEPTRUM_H_DT_ERROR_H_