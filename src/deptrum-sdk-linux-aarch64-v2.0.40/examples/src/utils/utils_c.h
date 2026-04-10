// Copyright (c) Deptrum Inc. All Rights Reserved.

#pragma once

#include <stdint.h>
#include "utils_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get the current system timestamp in milliseconds.
 *
 */
uint64_t dt_utils_get_current_timestamp_ms();

/**
 * @brief Wait for key press.
 *
 * @param[in] timeout_ms The maximum time to wait for a key press in milliseconds. Set to 0 to wait
 * indefinitely.
 *
 * @return char The key that was pressed.
 */
char dt_utils_wait_for_key_press(uint32_t timeout_ms);

// Macro to check for error and exit program if there is one.
#define CHECK_DT_ERROR_EXIT(error)                            \
  if (*error) {                                               \
    const char* error_message = dt_error_get_message(*error); \
    fprintf(stderr, "Error: %s\n", error_message);            \
    dt_delete_error(*error);                                  \
    *error = NULL;                                            \
    exit(-1);                                                 \
  }                                                           \
  *error = NULL;

#ifdef __cplusplus
}
#endif
